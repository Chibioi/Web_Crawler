package crawler

import (
	"math"
	"math/rand"
	"net/url"
	"regexp"
	"strings"
	"sync"
	"time"
)

type Rule struct {
	path    string
	allow   bool
	pattern *regexp.Regexp
}

type Group struct {
	rules      []*Rule
	agent      string
	crawlDelay time.Duration
	// test *Test()
}

type Cacheable interface {
	Set(string, string)
	Contains(string, string) bool
}

type Crawlingrules struct {
	baseDomain   *url.URL
	cache        Cacheable
	robotsGroups *Group
	fixedDelay   time.Duration
	lastDelay    time.Duration
	rwMutex      sync.RWMutex
}

// NewCrawlingRules creates a new CrawlingRules struct
func NewCrawlingRules(baseDomain *url.URL, cache Cacheable,
	fixedDelay time.Duration) *Crawlingrules {
	return &Crawlingrules{
		baseDomain: baseDomain,
		cache:      cache,
		fixedDelay: fixedDelay,
	}
}

func (g *Group) findRule(path string) (ret *Rule) {
	var prefixLen int

	for _, r := range g.rules {
		if r.pattern != nil {
			if r.pattern.MatchString(path) {
				// Consider this a match equal to the length of the pattern.
				// From Google's spec:
				// The order of precedence for rules with wildcards is undefined.
				if l := len(r.pattern.String()); l > prefixLen {
					prefixLen = l
					ret = r
				}
			}
		} else if r.path == "/" && prefixLen == 0 {
			// Weakest match possible
			prefixLen = 1
			ret = r
		} else if strings.HasPrefix(path, r.path) {
			if l := len(r.path); l > prefixLen {
				prefixLen = l
				ret = r
			}
		}
	}
	return
}

func (g *Group) Test(path string) bool {
	if r := g.findRule(path); r != nil {
		return r.allow
	}

	// From Google's spec:
	// By default, there are no restrictions for crawling for the designated crawlers.
	return true
}

func subdomain(domain *url.URL, link *url.URL) bool {
	return (link.Hostname() == domain.Hostname() || link.Hostname() == "")
}

// Allowed tests for eligibility of an URL to be crawled, based on the rules
// of the robots.txt file on the server. If no valid robots.txt is found all
// URLs in the domain are assumed to be allowed, returning true.

func (r *Crawlingrules) Allowed(url *url.URL) bool {
	if r.cache.Contains(r.baseDomain.String(), url.String()) {
		return false
	}

	defer r.cache.Set(r.baseDomain.String(), url.String())

	if r.robotsGroups != nil {
		return r.robotsGroups.Test(url.RequestURI()) && subdomain(r.baseDomain, url)
	}
	return subdomain(r.baseDomain, url)
}

func randDelay(value int64) time.Duration {
	if value == 0 {
		return 0 // No delay
	}

	max, min := 1.5*float64(value), 0.5*float64(value) // intended range for our random delay
	return time.Duration(rand.Int63n(int64(max-min)) + int64(min))
	//This generates a pseudo-random number within the calculated range and converts it to a time.Duration

}

func (r *Crawlingrules) CrawlDelay() time.Duration {

	r.rwMutex.RLock()

	defer r.rwMutex.RUnlock()
	var delay time.Duration

	if r.robotsGroups != nil {
		delay = r.robotsGroups.crawlDelay
	}

	// We calculate a random value: 0.5*fixedDelay < value < 1.5*fixedDelay
	randomDelay := randDelay(int64(r.fixedDelay.Milliseconds())) * time.Millisecond
	baseDelay := time.Duration(
		math.Max(float64(randomDelay.Milliseconds()), float64(delay.Milliseconds())),
	) * time.Millisecond
	// We return the max between the random value calculated and the lastDelay
	return time.Duration(
		math.Max(float64(r.lastDelay.Milliseconds()), float64(baseDelay.Milliseconds())),
	) * time.Millisecond

}

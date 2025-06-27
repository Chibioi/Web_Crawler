// Package crawler containing the crawling logics and utilities to scrape
// remote resources on the web
package crawler

import (
	"context"
	"encoding/json"
	"log"
	"net/http"
	"net/url"
	"os"
	"os/signal"
	"sync"
	"sync/atomic"
	"syscall"
	"time"

	"webcrawler/fetcher"
)

const (
	// Default fetcher timeout before giving up an URL
	defaultFetchTimeout time.Duration = 10 * time.Second
	// Default crawling timeout, time to wait to stop the crawl after no links are
	// found
	defaultCrawlTimeout time.Duration = 30 * time.Second
	// Default politeness delay, fixed delay to calculate a randomized wait time
	// for subsequent HTTP calls to a domain
	defaultPolitenessDelay time.Duration = 500 * time.Millisecond
	// Default depth to crawl for each domain
	defaultDepth int = 16
	// Default number of concurrent goroutines to crawl
	defaultConcurrency int = 8
	// Default user agent to use
	defaultUserAgent string = "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)"
)

// Fetcher is an interface exposing a method to fetch resources, Fetch enable
// raw contents download.
type Fetcher interface {
	// Fetch makes an HTTP GET request to an URL returning a `*http.Response` or
	// any error occured
	Fetch(string) (time.Duration, *http.Response, error)
}

// LinkFetcher is an interface exposing a methdo to download raw contents and
// parse them extracting all outgoing links.
type LinkFetcher interface {
	Fetcher
	// FetchLinks makes an HTTP GET request to an URL, parse the HTML in the
	// response and returns an array of URLs or any error occured
	FetchLinks(string) (time.Duration, []*url.URL, error)
}

// ParsedResult contains the URL crawled and an array of links found, json
// serializable to be sent on message queues
type ParsedResult struct {
	URL   string   `json:"url"`
	Links []string `json:"links"`
}

// CrawlerSettings represents general settings for the crawler and his
// dependencies
type CrawlerSettings struct {
	// FetchTimeout is the time to wait before closing a connection that does not
	// respond
	FetchTimeout time.Duration
	// CrawlTimeout is the number of second to wait before exiting the crawling
	// in case of no links found
	CrawlTimeout time.Duration
	// Concurrency is the number of concurrent goroutine to run while fetching
	// a page. 0 means unbounded
	Concurrency int
	// Parser is a `fetcher.Parser` instance object used to parse fetched pages
	Parser fetcher.Parser
	// MaxDepth represents a limit on the number of pages recursively fetched.
	// 0 means unlimited
	MaxDepth int
	// UserAgent is the user-agent header set in each GET request, most of the
	// times it also defines which robots.txt rules to follow while crawling a
	// domain, depending on the directives specified by the site admin
	UserAgent string
	// PolitenessFixedDelay represents the delay to wait between subsequent
	// calls to the same domain, it'll taken into consideration against a
	// robots.txt if present and against the last response time, taking always
	// the major between these last two. Robots.txt has the precedence.
	PolitenessFixedDelay time.Duration
}

// WebCrawler is the main object representing a crawler
type WebCrawler struct {
	// logger is a private logger instance
	logger *log.Logger
	// linkFetcher is a LinkFetcher object, must expose Fetch and FetchLinks methods
	linkFetcher LinkFetcher
	// settings is a pointer to `CrawlerSettings` containing some crawler
	// specifications
	settings *CrawlerSettings
}

// New create a new Crawler instance, accepting a maximum level of depth during
// crawling all the anchor links inside each page, a concurrency limiter that
// defines how many goroutine to run in parallel while fetching links and a
// timeout for each HTTP call.
func New(userAgent string) *WebCrawler {
	// Default crawler settings
	settings := &CrawlerSettings{
		FetchTimeout:         defaultFetchTimeout,
		Parser:               fetcher.NewGoqueryParser(),
		UserAgent:            userAgent,
		CrawlTimeout:         defaultCrawlTimeout,
		PolitenessFixedDelay: defaultPolitenessDelay,
		Concurrency:          defaultConcurrency,
	}
	crawler := &WebCrawler{
		logger:      log.New(os.Stderr, "crawler: ", log.LstdFlags),
		linkFetcher: fetcher.New(userAgent, settings.Parser, settings.FetchTimeout),
		settings:    settings,
	}
	return crawler
}

// NewFromSettings create a new webCrawler with the settings passed in
func NewFromSettings(settings *CrawlerSettings) *WebCrawler {
	return &WebCrawler{
		logger:      log.New(os.Stderr, "crawler: ", log.LstdFlags),
		linkFetcher: fetcher.New(settings.UserAgent, settings.Parser, settings.FetchTimeout),
		settings:    settings,
	}
}

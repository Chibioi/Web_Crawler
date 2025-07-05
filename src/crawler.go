package crawler

import (
	"net/http"
	"net/url"
	// "packages/src/crawler"
	"packages/src/fetcher"
	"time"
)

const (
	defaultfetchtimeout    time.Duration = 10 * time.Second
	defaultcrawltimeout    time.Duration = 30 * time.Second
	defaultpolitenessdelay time.Duration = 500 * time.Millisecond
	defaultdepth           int           = 16
	defaultconcurrency     int           = 8
	defaultUserAgent       string        = "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)"
)

type Fetcher interface {
	Fetch(string) (time.Duration, *http.Response, error)
}

type Linkfetcher interface {
	Fetcher
	Fetchlinks(string) (time.Duration, []*url.URL, error)
}

type Parsedresults struct {
	URL   string   `json: "URL"`
	Links []string `json: "Links"`
}

type Crawlersettings struct {
	fetchtimeout time.Duration
	crawltimeout time.Duration
	concurrency  int
	parser       fetcher.Parser
}

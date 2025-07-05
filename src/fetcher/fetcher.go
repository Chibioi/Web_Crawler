package fetcher

import (
	"io"
	"net/url"
)

type Parser interface {
	Parse(string, io.Reader) ([]*url.URL, error)
}

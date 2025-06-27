// crawler.h - Header file for the web crawler module

#ifndef CRAWLER_H
#define CRAWLER_H

#include <stdio.h>    // For logging (stderr)
#include <stdlib.h>   // For malloc, free
#include <string.h>   // For string manipulation
#include <time.h>     // For time_t, representing durations
#include <stdbool.h>  // For bool type

// --- Constants ---
// Default fetcher timeout before giving up an URL (in milliseconds)
#define DEFAULT_FETCH_TIMEOUT_MS 10000
// Default crawling timeout, time to wait to stop the crawl after no links are
// found (in milliseconds)
#define DEFAULT_CRAWL_TIMEOUT_MS 30000
// Default politeness delay, fixed delay to calculate a randomized wait time
// for subsequent HTTP calls to a domain (in milliseconds)
#define DEFAULT_POLITENESS_DELAY_MS 500
// Default depth to crawl for each domain
#define DEFAULT_DEPTH 16
// Default number of concurrent threads to crawl
#define DEFAULT_CONCURRENCY 8
// Default user agent to use
#define DEFAULT_USER_AGENT "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)"

// --- Placeholder/Mock Structures for external dependencies ---

// Represents an HTTP response. In a real application, this would contain status code, headers, and body.
typedef struct {
    int status_code;
    // Other fields like headers, body stream etc. would go here.
} http_response_t;

// Represents a URL. In a real application, this would have parsed components (scheme, host, path, etc.).
typedef struct {
    char* full_url; // The original URL string
    // Potentially other parsed components like host, path, etc.
} url_t;

// --- Interface Simulations (using function pointers in structs) ---

// fetcher_parser_t is a placeholder for the fetcher.Parser interface.
// In a real scenario, this would have methods for parsing HTML.
typedef struct {
    // Placeholder for parser specific data
    void* data;
    // Function pointer for a parsing method, e.g., int (*parse_links)(void* parser_data, const char* html_content, url_t** links, int* num_links);
} fetcher_parser_t;

// Fetcher interface equivalent
// The fetch_func would typically return a duration (time taken), a pointer to http_response_t, and an error code.
typedef int (*fetch_func)(const char* url_str, time_t* duration_ms, http_response_t** response);

typedef struct {
    fetch_func fetch;
} Fetcher;

// LinkFetcher interface equivalent
// Extends Fetcher and adds FetchLinks
// The fetch_links_func would typically return a duration (time taken), an array of url_t pointers,
// the number of links found, and an error code.
typedef int (*fetch_links_func)(const char* url_str, time_t* duration_ms, url_t*** links_out, int* num_links_out);

typedef struct {
    Fetcher base_fetcher; // Embedding the base Fetcher
    fetch_links_func fetch_links;
    // Internal data for the LinkFetcher, e.g., user agent, parser.
    char* user_agent;
    fetcher_parser_t* parser;
    time_t fetch_timeout_ms;
} LinkFetcher;


// ParsedResult contains the URL crawled and an array of links found.
// In C, for JSON serialization, you would typically use a library like Jansson or json-c.
typedef struct {
    char* url;
    char** links; // Array of string pointers for links
    int num_links; // Number of links in the array
} ParsedResult;

// CrawlerSettings represents general settings for the crawler and its dependencies
typedef struct {
    // FetchTimeout is the time to wait before closing a connection that does not respond (in milliseconds)
    time_t FetchTimeoutMs;
    // CrawlTimeout is the number of seconds to wait before exiting the crawling
    // in case of no links found (in milliseconds)
    time_t CrawlTimeoutMs;
    // Concurrency is the number of concurrent threads to run while fetching a page. 0 means unbounded
    int Concurrency;
    // Parser is a `fetcher_parser_t` instance object used to parse fetched pages
    fetcher_parser_t* Parser;
    // MaxDepth represents a limit on the number of pages recursively fetched.
    // 0 means unlimited
    int MaxDepth;
    // UserAgent is the user-agent header set in each GET request
    char* UserAgent;
    // PolitenessFixedDelay represents the delay to wait between subsequent
    // calls to the same domain (in milliseconds)
    time_t PolitenessFixedDelayMs;
} CrawlerSettings;

// WebCrawler is the main object representing a crawler
typedef struct {
    // logger is a private logger instance. In C, simply fprintf to stderr.
    // FILE* logger; // Or a function pointer for more complex logging
    // linkFetcher is a LinkFetcher object, must expose Fetch and FetchLinks methods
    LinkFetcher* link_fetcher;
    // settings is a pointer to `CrawlerSettings` containing some crawler specifications
    CrawlerSettings* settings;
} WebCrawler;

// --- Function Declarations (Go equivalent of `New` and `NewFromSettings`) ---

/**
 * @brief Creates a new default WebCrawler instance.
 * @param user_agent The user agent string to use for HTTP requests.
 * @return A pointer to a newly allocated WebCrawler instance, or NULL on error.
 * The caller is responsible for freeing this memory using `free_web_crawler`.
 */
WebCrawler* NewWebCrawler(const char* user_agent);

/**
 * @brief Creates a new WebCrawler instance with custom settings.
 * @param settings A pointer to CrawlerSettings to use. The function will make a copy
 * of the settings.
 * @return A pointer to a newly allocated WebCrawler instance, or NULL on error.
 * The caller is responsible for freeing this memory using `free_web_crawler`.
 */
WebCrawler* NewWebCrawlerFromSettings(const CrawlerSettings* settings);

/**
 * @brief Frees all memory associated with a WebCrawler instance.
 * @param crawler A pointer to the WebCrawler instance to free.
 */
void free_web_crawler(WebCrawler* crawler);

/**
 * @brief Frees all memory associated with CrawlerSettings.
 * @param settings A pointer to the CrawlerSettings instance to free.
 */
void free_crawler_settings(CrawlerSettings* settings);

// --- External Fetcher functions (stubs/placeholders - would be implemented in fetcher.c) ---
// These functions simulate the `fetcher` package.
// In a real application, you'd link against a library like libcurl for HTTP operations.

// Placeholder for fetcher.NewGoqueryParser()
fetcher_parser_t* fetcher_new_goquery_parser();

// Placeholder for fetcher.New() - This would internally set up the actual fetch and fetch_links implementations.
LinkFetcher* fetcher_new_link_fetcher(const char* user_agent, fetcher_parser_t* parser, time_t fetch_timeout_ms);

// Placeholder for freeing the LinkFetcher
void fetcher_free_link_fetcher(LinkFetcher* lf);

#endif // CRAWLER_H


// crawler.c - Implementation file for the web crawler module

#include "crawler.h" // Include our own header
// Other necessary headers for pthreads, networking, etc., would go here.
// For this example, we'll keep it minimal to focus on the structure.

// --- Helper Functions (for placeholders) ---

// Mock implementation of fetcher_new_goquery_parser
fetcher_parser_t* fetcher_new_goquery_parser() {
    fetcher_parser_t* parser = (fetcher_parser_t*)malloc(sizeof(fetcher_parser_t));
    if (parser) {
        parser->data = NULL; // No specific data for this mock parser
    }
    return parser;
}

// Mock implementation of Fetch function
// In a real scenario, this would use libcurl or similar.
int mock_fetch_func(const char* url_str, time_t* duration_ms, http_response_t** response) {
    fprintf(stderr, "MOCK: Fetching %s...\n", url_str);
    if (duration_ms) *duration_ms = 100; // Simulate 100ms fetch time
    if (response) {
        *response = (http_response_t*)malloc(sizeof(http_response_t));
        if (*response) {
            (*response)->status_code = 200; // Simulate success
        } else {
            fprintf(stderr, "Error: MOCK failed to allocate http_response_t\n");
            return -1; // Indicate error
        }
    }
    return 0; // Success
}

// Mock implementation of FetchLinks function
// In a real scenario, this would fetch, parse HTML, and extract links.
int mock_fetch_links_func(const char* url_str, time_t* duration_ms, url_t*** links_out, int* num_links_out) {
    fprintf(stderr, "MOCK: Fetching links for %s...\n", url_str);
    if (duration_ms) *duration_ms = 250; // Simulate 250ms fetch+parse time

    // Simulate finding a couple of links
    if (links_out && num_links_out) {
        *num_links_out = 2;
        *links_out = (url_t**)malloc(sizeof(url_t*) * (*num_links_out));
        if (!(*links_out)) {
            fprintf(stderr, "Error: MOCK failed to allocate links_out array\n");
            return -1;
        }

        (*links_out)[0] = (url_t*)malloc(sizeof(url_t));
        if (!(*links_out)[0]) { free(*links_out); *links_out = NULL; return -1; }
        (*links_out)[0]->full_url = strdup("http://example.com/link1");

        (*links_out)[1] = (url_t*)malloc(sizeof(url_t));
        if (!(*links_out)[1]) { free((*links_out)[0]->full_url); free((*links_out)[0]); free(*links_out); *links_out = NULL; return -1; }
        (*links_out)[1]->full_url = strdup("http://example.com/link2");

        if (!(*links_out)[0]->full_url || !(*links_out)[1]->full_url) {
            // Handle strdup allocation failure
            if ((*links_out)[0]->full_url) free((*links_out)[0]->full_url);
            if ((*links_out)[0]) free((*links_out)[0]);
            if ((*links_out)[1]->full_url) free((*links_out)[1]->full_url);
            if ((*links_out)[1]) free((*links_out)[1]);
            free(*links_out); *links_out = NULL; *num_links_out = 0;
            return -1;
        }
    }
    return 0; // Success
}

// Mock implementation of fetcher_new_link_fetcher
LinkFetcher* fetcher_new_link_fetcher(const char* user_agent, fetcher_parser_t* parser, time_t fetch_timeout_ms) {
    LinkFetcher* lf = (LinkFetcher*)malloc(sizeof(LinkFetcher));
    if (lf) {
        lf->base_fetcher.fetch = mock_fetch_func;
        lf->fetch_links = mock_fetch_links_func;
        lf->user_agent = strdup(user_agent);
        lf->parser = parser; // We assume parser is already allocated and managed elsewhere
        lf->fetch_timeout_ms = fetch_timeout_ms;

        if (!lf->user_agent) {
            fprintf(stderr, "Error: MOCK failed to duplicate user agent string\n");
            free(lf);
            return NULL;
        }
    }
    return lf;
}

// Mock implementation of fetcher_free_link_fetcher
void fetcher_free_link_fetcher(LinkFetcher* lf) {
    if (lf) {
        free(lf->user_agent);
        // Do NOT free lf->parser here, as it's assumed to be managed externally (passed in).
        free(lf);
    }
}


// --- WebCrawler Implementation ---

WebCrawler* NewWebCrawler(const char* user_agent) {
    CrawlerSettings default_settings = {
        .FetchTimeoutMs = DEFAULT_FETCH_TIMEOUT_MS,
        .CrawlTimeoutMs = DEFAULT_CRAWL_TIMEOUT_MS,
        .Concurrency = DEFAULT_CONCURRENCY,
        .Parser = fetcher_new_goquery_parser(), // Create a new parser for defaults
        .MaxDepth = DEFAULT_DEPTH, // Go code had 16 here, 0 in comment, clarified to match defaultDepth
        .UserAgent = (char*)user_agent, // This will be copied internally by NewWebCrawlerFromSettings
        .PolitenessFixedDelayMs = DEFAULT_POLITENESS_DELAY_MS,
    };

    // Ensure the parser was created successfully
    if (!default_settings.Parser) {
        fprintf(stderr, "Error: Failed to create default parser in NewWebCrawler.\n");
        return NULL;
    }

    WebCrawler* crawler = NewWebCrawlerFromSettings(&default_settings);

    // Free the parser created for default settings, as NewWebCrawlerFromSettings will create its own LinkFetcher.
    // The parser pointer is effectively transferred to the LinkFetcher.
    // So, we only need to free it if NewWebCrawlerFromSettings fails.
    if (crawler == NULL) {
        // If NewWebCrawlerFromSettings fails, the default_settings.Parser might not have been
        // transferred/copied/used, so we free it here.
        // This assumes fetcher_parser_t doesn't have internal malloced data that needs deep freeing here.
        free(default_settings.Parser);
    }

    return crawler;
}

WebCrawler* NewWebCrawlerFromSettings(const CrawlerSettings* settings) {
    if (!settings) {
        fprintf(stderr, "Error: Settings cannot be NULL for NewWebCrawlerFromSettings.\n");
        return NULL;
    }

    WebCrawler* crawler = (WebCrawler*)malloc(sizeof(WebCrawler));
    if (!crawler) {
        fprintf(stderr, "Error: Failed to allocate memory for WebCrawler.\n");
        return NULL;
    }

    // Allocate and copy settings
    crawler->settings = (CrawlerSettings*)malloc(sizeof(CrawlerSettings));
    if (!crawler->settings) {
        fprintf(stderr, "Error: Failed to allocate memory for CrawlerSettings.\n");
        free(crawler);
        return NULL;
    }
    memcpy(crawler->settings, settings, sizeof(CrawlerSettings)); // Copy struct members

    // Copy user agent string separately as it's a char*
    crawler->settings->UserAgent = strdup(settings->UserAgent ? settings->UserAgent : DEFAULT_USER_AGENT);
    if (!crawler->settings->UserAgent) {
        fprintf(stderr, "Error: Failed to duplicate user agent string.\n");
        free(crawler->settings);
        free(crawler);
        return NULL;
    }

    // IMPORTANT: The parser from the input settings is shallow copied.
    // If the parser itself contains dynamically allocated memory, it should be deep copied here
    // or its lifetime managed carefully by the caller. For this example, we assume it's either
    // stack-allocated or managed by `fetcher_new_goquery_parser`.
    // If the parser was created by fetcher_new_goquery_parser(), it's likely a simple struct.
    crawler->settings->Parser = settings->Parser; // Shallow copy of parser pointer

    // Initialize the logger (simple fprintf to stderr)
    // crawler->logger = stderr; // If using FILE*

    // Create the LinkFetcher
    crawler->link_fetcher = fetcher_new_link_fetcher(
        crawler->settings->UserAgent,
        crawler->settings->Parser, // Pass the parser to the fetcher
        crawler->settings->FetchTimeoutMs
    );

    if (!crawler->link_fetcher) {
        fprintf(stderr, "Error: Failed to create LinkFetcher.\n");
        free(crawler->settings->UserAgent);
        free(crawler->settings);
        free(crawler);
        return NULL;
    }

    fprintf(stderr, "crawler: WebCrawler created with user agent: %s\n", crawler->settings->UserAgent);
    return crawler;
}

void free_web_crawler(WebCrawler* crawler) {
    if (crawler) {
        if (crawler->link_fetcher) {
            fetcher_free_link_fetcher(crawler->link_fetcher);
        }
        if (crawler->settings) {
            free_crawler_settings(crawler->settings);
        }
        free(crawler);
    }
}

void free_crawler_settings(CrawlerSettings* settings) {
    if (settings) {
        free(settings->UserAgent);
        // IMPORTANT: If settings->Parser was dynamically allocated *and* ownership
        // is meant to be here, it should be freed.
        // Assuming fetcher_new_goquery_parser() creates a simple struct and its
        // lifetime is managed by the LinkFetcher, or it's a singleton.
        // For this example, we'll assume it's handled by LinkFetcher's lifecycle
        // or is not dynamically allocated with a deep free required here.
        // If it was allocated by fetcher_new_goquery_parser() in NewWebCrawler,
        // it was freed there if NewWebCrawlerFromSettings failed.
        // Otherwise, the LinkFetcher is responsible for it.
        free(settings);
    }
}


// --- Example Usage (main function for testing) ---
#ifdef CRAWLER_MAIN_EXAMPLE
#include <unistd.h> // For sleep

int main() {
    fprintf(stderr, "--- Creating default crawler ---\n");
    WebCrawler* default_crawler = NewWebCrawler(DEFAULT_USER_AGENT);
    if (default_crawler) {
        fprintf(stderr, "Default crawler created successfully!\n");

        time_t duration;
        url_t** links = NULL;
        int num_links = 0;
        int result = default_crawler->link_fetcher->fetch_links(
            "http://example.com",
            &duration,
            &links,
            &num_links
        );

        if (result == 0) {
            fprintf(stderr, "Fetched links in %ld ms. Found %d links:\n", duration, num_links);
            for (int i = 0; i < num_links; ++i) {
                if (links[i] && links[i]->full_url) {
                    fprintf(stderr, "  - %s\n", links[i]->full_url);
                    free(links[i]->full_url); // Free strdup'd memory
                    free(links[i]);
                }
            }
            if (links) free(links);
        } else {
            fprintf(stderr, "Failed to fetch links.\n");
        }

        free_web_crawler(default_crawler);
        fprintf(stderr, "Default crawler freed.\n\n");
    } else {
        fprintf(stderr, "Failed to create default crawler.\n\n");
    }

    fprintf(stderr, "--- Creating custom crawler ---\n");
    CrawlerSettings custom_settings = {
        .FetchTimeoutMs = 5000,
        .CrawlTimeoutMs = 60000,
        .Concurrency = 4,
        .Parser = fetcher_new_goquery_parser(),
        .MaxDepth = 5,
        .UserAgent = "MyCustomCrawler/1.0",
        .PolitenessFixedDelayMs = 1000,
    };

    if (!custom_settings.Parser) {
        fprintf(stderr, "Error: Failed to create parser for custom settings.\n");
        return 1;
    }

    WebCrawler* custom_crawler = NewWebCrawlerFromSettings(&custom_settings);
    if (custom_crawler) {
        fprintf(stderr, "Custom crawler created successfully!\n");
        // Perform some operations with custom_crawler if needed
        free_web_crawler(custom_crawler);
        fprintf(stderr, "Custom crawler freed.\n");
    } else {
        fprintf(stderr, "Failed to create custom crawler.\n");
        free(custom_settings.Parser); // Free if the crawler creation failed
    }

    return 0;
}
#endif // CRAWLER_MAIN_EXAMPLE


#ifndef CRAWLER_H
#define CRAWLER_H

#include <stdio.h>
#include <time.h>

#define DEFAULT_FETCH_TIMEOUT_MS 10000 // Default fetcher timeout before givingup an URL
                                       //
#define DEFAULT_CRAWL_TIMEOUT_MS 30000 // default crawling timeout, time to wait and stop the crawl if no links are found
                                       //
#define DEFAULT_POLITENESS_DELAY_MS 500 
// Default politeness delay, fixed delay to calculate a randomized wait time
// for subsequent HTTP calls to a domain (in milliseconds)
//
#define DEFAULT_DEPTH 16
// default depth to crawl for each domain

#define DEFAULT_CONCURRENCY 8
// default number of concurrent threads to crawl 

#define DEFAULT_USER_AGENT "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)"
// Default user agent to use

typedef struct {
    int status_code;
} http_response_t;

typedef struct {
    char* full_url;
} url_t;

typedef struct {
    void* data;
} fetcher_parser_t;


typedef int (*fetch_func)(const char* url_str, time_t* duration_ms, http_response_t** response);

typedef struct {
    fetch_func fetch;
} fetcher;

typedef int (*fetch_links_func)(const char* url_str, time_t* duration_ms, url_t*** links_out, int* num_links_out);

typedef struct {
    fetcher base_fetcher; // Embedding the base Fetcher
    fetch_links_func fetch_links;
    // Internal data for the LinkFetcher, e.g., user agent, parser.
    char* user_agent;
    fetcher_parser_t* parser;
    time_t fetch_timeout_ms;
} LinkFetcher;







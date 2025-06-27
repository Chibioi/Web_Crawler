#ifndef CRAWLER_H
#define CRAWLER_H

#include <stdio.h>

#define DEFAULT_FETCH_TIMEOUT_MS 10000 // Default fetcher timeout before givingup an URL
#define DEFAULT_CRAWL_TIMEOUT_MS 30000 // default crawling timeout, time to wait and stop the crawl if no links are found
#define DEFAULT_POLITENESS_DELAY_MS 500

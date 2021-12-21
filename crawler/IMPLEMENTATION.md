# Implementation Specifications

## Crawler, CS50 Lab 4

#### Elizabeth Frey, Winter 2021

The crawler.c file contains a crawler that goes through links in a webpage, starting at a `seedURL`. It explores links up to a `maxDepth`, printing their depth in a unique file in the given `pageDirectory`. It will output each time it fetches the HTML of a URL, finds a new URL, and adds the URL to a list of URLs to visit (or doesn't add if a duplicate or external URL).

#### Vocabulary

**Internal**: an *internal* URL is one that stays within the CS50 playground. 

**Normalize**: to *normalize* the URL means to convert it to a clean, canoical form. For example, converting `Http://Hello.com` to `http://hello.com`.

### Functions

`crawler.c` has five main functions:

1. `main`
2. `crawler`
3. `pagefetcher`
4. `pagescanner`
5. `pagesaver`

#### 1. `main`

This method will run the bulk of the work for the Crawler. First, it will check to see if the arguments given from the command line are correct. The user must use the correct usage syntax: `./crawler seedURL pageDirectory maxDepth`. 

It will validate the inputs as specified:

- `seedURL`, the initial URL, must be "internal."
- `pageDirectory`, the directory in which to write downloaded webpages, must be the pathname for an *existing* directory. 
- `maxDepth` must be an integer between 0 and 10, inclusive.

Finally, it will call `crawler`, which will loop over pages to explore (see `crawler` section for more detailed pseudocode.)

After `crawler` is completed, `main` will free all memory it allocated in the beginning. 

#### 2. `crawler`

First, it will initialize the modules. It will initialize a `bag` of pages we have yet to explore and a `hashtable` of URLs that we have seen so far. It will also initialize a `webpage` for the seedURL, marked with depth = 0. It will call `bag_add` to add that `webpage` to the bag we created, and will add that URL to the `hashtable` as the *key*, with the *item* being a random string.

This function loops over the `bag` of URLs to crawl as long as the `bag` isn't empty. 

First, it will call `bag_extract` to extract a `webpage` from the `bag` of webpages to be crawled. It then pauses to prevent overloading the server.

Next, it will call `pagefetcher` to retrieve the HTML for the page at that URL (see `pagefetcher` for more detailed pseudocode on this). It will save this HTML in a `webpage` (allocating space as needed). 

Third, it will use `pagesaver` to write the `webpage` to the `pageDirectory` given by the user. Each `webpage` will be written to the `pageDirectory` with a unique ID (see `pagesaver` for more detailed pseudocode)

Fourth, if the `webpage`'s depth is less than the `maxDepth` given by the user, we will explore the `webpage` to find links using `pagescanner`. As long as `pagescanner` returns a new URL from the `webpage`, we will check to make sure the URL is internal, not a duplicate using our `hashtable`, and make a new `webpage` for that URL with the current depth + 1. If there was any heap memory allocated to the URL, we will free this memory (if it does not get put into a `webpage`)

Finally, we will free any heap memory that we allocated to the `webpage` after it has been fully examined, and continue looping through the `bag` until completion. At the end, we will delete the `bag` and the `hashtable`.

#### 3. `pagefetcher`

`pagefetcher` is used to fetch the HTML from the URL. It uses `webpage_fetch` from `webpage` (see the `webpage` section below) to get the HTML of a URL from a `webpage`. 

#### 4. `pagescanner`

`pagescanner ` takes in an `int` to represent the current *position* and uses the `webpage_getNextURL` method from `webpage` (see the `webpage` section below) to return the next URL in the HTML. `pagescanner` will save the URL as a pointer to a `char` and return an `int` representing the place in the HTML directly after the URL found. This `int` is returned so that `pagescanner` can keep track of where it needs to continue searching.

#### 5. `pagesaver`

`pagesaver` is used to write the `webpage` to the `pageDirectory`. It creates a new page with the filename of the form `pageDirectory/id`, where the  document `id` is an `int` that starts at 1 and increments by 1 for each new page. It then writes each explored page to `pageDirectory` with the format:

Line 1: the URL
Line 2: an `int` representing the depth of the `webpage`
The rest of the file: the HTML of the `webpage`, using the result from `pagefetcher`.

### Data Structures

`crawler.c` uses three main modules: `bag`, `hashtable` (which indirectly uses `set`), and `webpage`. `bag` and `webpage` were provided in the starter package, and `set` and `hashtable` were written by me as a part of Lab 3. 

####  `bag`

A `bag` is the data structure we use to hold `webpages` to track the pages that we need to explore. `bag` has many different fucntions, but we will only be using four of them:

1. `bag_new` is used to to create the bag
2. `bag_insert` is used to insert `webpages` into the `bag`
3. `bag_extract` is used to remove URLs from the `bag` as the `crawler` processes them. The user is responsible for freeing any item returned by this
4. `bag_delete` is used at the end to delete the bag

####  `hashtable`

A `hashtable` is the data structure we use to keep track of URLs that we have already visited. We will create a hashtable of `char *key` *keys* with a pointer to a `" "` string as a placeholder. This is used to ensure we do not visit a URL twice. `hashtable` has multiple functions, but we will only be using three of them:

1. `hashtable_new` is used to make the hashtable. In this, we have to specify the size of the `hashtable` we create.
2. `hashtable_insert` is used to add URLs into the `hashtable` after they are scanned by `page_scanner`
3. `hashtable_delete` is used at the end to delete the hashtable

####  `webpage`

A `webpage` is the module that we use to hold a URL, its depth (distance from the seed URL), and its HTML (optional). The main functions we use are:

1. `webpage_new` is used to create a new `webpage` object to hold the URL and depth. Sometimes, we include the HTML. 
2. `webpage_fetch` is used to download the HTML for the page at a HTML at a given URL, and it saves it to the `webpage` `struct`
3. `webpage_getNextURL` is used to start or continue a scan of the HTML for the given URL returning the next URL in the page. 
4. `normalizeURL` is used to normalize the URL. See the vocabulary section for the definition of noramlize. 
5. `IsInternalURL` is used to check to see whether a given URL is internal. It also normalizes the URL in the process of checking. See the vocabulary section for the definition of internal. 
6. Getter methods `webpage_getDepth`, `webpage_getURL`, and `webpage_getHTML` are used to get the depth, URL, and HTML of a webpage, respectively.
7. `webpage_delete` is used to delete the webpage at the end

### Security and Privacy Properties

There are no significant steps taken to ensure security and privacy for the user. 

### Error Handling and Recovery

There are a few possible errors that this code needs to address. 

First of all, it needs to recognize when the user inputs the wrong arguments on the command line. There's no logical way to recover from this, so in this case, the program will print an error message and quit with a non-zero exit status. 

Secondly, if a page cannot be fetched, it needs to throw an error. In this case, the program can continue with other `webpages` in the `bag`.

### Resource Management

In order to prevent websites from blocking the dartmouth.edu network, care was taken to ensure that this program was only ever run on an internal URL. 

### Peristant Storage

There is no persistant storage used. 
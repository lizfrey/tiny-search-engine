# Crawler

## CS50 Lab 4

### Elizabeth Frey, Winter 2021

The crawler.c file contains a crawler that goes through links in a webpage, starting at a `seedURL`. It explores links up to a `maxDepth`, printing their depth in a unique file in the given `pageDirectory`. It will output each time it fetches the HTML of a URL, finds a new URL, and adds the URL to a list of URLs to visit (or doesn't add if a duplicate or external URL).

#### Compiling and Usage

To compile, use `make` command. 
To test, use `make test` command. This will handle creating and deleting the dir directory used in `testing.sh`
To clean, use `make clean` command. 

Usage: 
```bash
./crawler seedURL pageDirectory maxDepth
```

#### Assumptions
All assumptions are stated in the specifications. If the directory already exists, any existing files with the same name as files written will be overwritten.

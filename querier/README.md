# Querier

## CS50 Lab 6

### Elizabeth Frey, Winter 2021

The TSE **querier** is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Crawler, and answers search queries submitted via stdin. The **fuzzquery** file was provided by CS50, and is used to generate random queries to give to the querier.

#### Compiling and Usage

To compile, use `make` command. 
To test, use `make test` command. This will call `make clean` once complete.
To clean, use `make clean` command. 

Usage: 
```bash
./querier crawlerDirectory indexFilename
```

```bash
./fuzzquery indexFile numQueries randomSeed
```

#### Assumptions
All assumptions are stated in the specifications.

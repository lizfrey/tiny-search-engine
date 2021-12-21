# Indexer

## CS50 Lab 5

### Elizabeth Frey, Winter 2021

The TSE *indexer* is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file.
Its companion, the *index tester,* loads an index file produced by the indexer and saves it to another file.

#### Compiling and Usage

To compile, use `make` command. 
To test, use `make test` command. This will handle creating and deleting the dir directory used in `testing.sh`
To clean, use `make clean` command. 

Usage: 
```bash
./indexer crawlerDirectory newIndexFilename
```

```bash
./indextest oldIndexFilename newIndexFilename
```

#### Assumptions
All assumptions are stated in the specifications. If the `newIndexFilename` already exists, any existing files with the same name as files written will be overwritten.

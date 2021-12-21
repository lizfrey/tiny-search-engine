# Testing file for querier
#
# Elizabeth Frey
# Winter 2021
# CS 50 Lab 6

# testing setup
export tseoutput="/thayerfs/home/d29265d/shared/tse-output"


#### TESTING INDEXER ####
## ARGUMENT VALIDATION:
#   wrong number of arguments
./querier 

./querier hi

./querier hi hi hi

#   directory does not exist
./querier fakeDir indexFile

#   directory is not a crawler directory
mkdir notCrawlerDirectory
./querier notCrawlerDirectory indexFile
rm -r -f notCrawlerDirectory

#   index file does not exist
./querier $tseoutput/crawler/crawler.data/data1 indexFile

## INVALID QUERIES
# non alphabetic character
echo hi* > file
./querier $tseoutput/crawler/crawler.data/data0 $tseoutput/indexer/indexer.data/index.0 < file

# connectors at the beginning and end
echo and hi > file
./querier $tseoutput/crawler/crawler.data/data0 $tseoutput/indexer/indexer.data/index.0 < file
echo hello or > file
./querier $tseoutput/crawler/crawler.data/data0 $tseoutput/indexer/indexer.data/index.0 < file

# multiple connectors in a row
echo hi and and no > file
./querier $tseoutput/crawler/crawler.data/data0 $tseoutput/indexer/indexer.data/index.0 < file

## TESTING WITH GIVEN CRAWLER AND INDEX FILES
# this will use the crawler and index files from the TSE output folder given
# in /thayerfs/home/d29265d/shared/tse-output/crawler to 
# test out different queries useing fuzz testing
# I will test each one with two different random sets of data

# test 0
./fuzzquery $tseoutput/indexer/indexer.data/index.0  5 1 > testfile
./querier $tseoutput/crawler/crawler.data/data0 $tseoutput/indexer/indexer.data/index.0 < testfile

./fuzzquery $tseoutput/indexer/indexer.data/index.0  5 5 > testfile
./querier $tseoutput/crawler/crawler.data/data0 $tseoutput/indexer/indexer.data/index.0 < testfile

# test 1
./fuzzquery $tseoutput/indexer/indexer.data/index.1  5 1 > testfile
./querier $tseoutput/crawler/crawler.data/data1 $tseoutput/indexer/indexer.data/index.1 < testfile

./fuzzquery $tseoutput/indexer/indexer.data/index.1  5 5 > testfile
./querier $tseoutput/crawler/crawler.data/data1 $tseoutput/indexer/indexer.data/index.1 < testfile

# test 2
./fuzzquery $tseoutput/indexer/indexer.data/index.2  5 1 > testfile
./querier $tseoutput/crawler/crawler.data/data2 $tseoutput/indexer/indexer.data/index.2 < testfile

./fuzzquery $tseoutput/indexer/indexer.data/index.2  5 5 > testfile
./querier $tseoutput/crawler/crawler.data/data2 $tseoutput/indexer/indexer.data/index.2 < testfile

rm -f testfile
rm -f file

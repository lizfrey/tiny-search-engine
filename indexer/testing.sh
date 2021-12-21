# testing file for indexer
#
# Elizabeth Frey
# Winter 2021
# CS 50 Lab 5

#### TESTING INDEXER ####
## ARGUMENT VALIDATION:
#   wrong number of arguments
./indexer 

./indexer hi

./indexer hi hi hi

#   directory does not exist
./indexer fakeDir testfile

#   directory is not a crawler directory
mkdir notCrawlerDirectory
./indexer notCrawlerDirectory testfile
rm -r -f notCrawlerDirectory

## TESTING WITH GIVEN CRAWLER FILES
# this will use the crawler files from the TSE output folder given
# in /thayerfs/home/d29265d/shared/tse-output/crawler, and 
# compare the testfiles
# that my indexer makes with the index testfiles in 
# /thayerfs/home/d29265d/shared/tse-output/indexer
export tseoutput="/thayerfs/home/d29265d/shared/tse-output"

#   test 0
./indexer $tseoutput/crawler/crawler.data/data0 testfile
# i will need to sort the testfiles to compare them
gawk -f indexsort.awk testfile > testfile.sorted
gawk -f indexsort.awk $tseoutput/indexer/indexer.data/index.0 > legitfile.sorted
diff testfile.sorted legitfile.sorted
# if nothing prints after diff, no differences found

# test 1
./indexer $tseoutput/crawler/crawler.data/data1 testfile
# i will need to sort the testfiles to compare them
gawk -f indexsort.awk testfile > testfile.sorted
gawk -f indexsort.awk $tseoutput/indexer/indexer.data/index.1 > legitfile.sorted
diff testfile.sorted legitfile.sorted
# if nothing prints after diff, no differences found

#   test 2
./indexer $tseoutput/crawler/crawler.data/data2 testfile
# i will need to sort the testfiles to compare them
gawk -f indexsort.awk testfile > testfile.sorted
gawk -f indexsort.awk $tseoutput/indexer/indexer.data/index.2 > legitfile.sorted
diff testfile.sorted legitfile.sorted
# if nothing prints after diff, no differences found

#### TESTING INDEXTEST ####
# ARGUMENT VALIDATION
#   number of arguments

./indextest 

./indextest hi

./indextest hi hi hi

#   oldFilename (for reading) doesn't exist
./indextest fakefile newIndexFilename

## TESTING WITH GIVEN INDEX FILES
# this will use the index files given, 
# found in /thayerfs/home/d29265d/shared/tse-output/indexer
# and sort them, and compare the differences

#   test 0
./indextest $tseoutput/indexer/indexer.data/index.0 newfile
gawk -f indexsort.awk newfile > newfile.sorted
gawk -f indexsort.awk $tseoutput/indexer/indexer.data/index.0 > oldfile.sorted
diff newfile.sorted oldfile.sorted
# if nothing prints after diff, no differences found

#   test 1
./indextest $tseoutput/indexer/indexer.data/index.1 newfile
gawk -f indexsort.awk newfile > newfile.sorted
gawk -f indexsort.awk $tseoutput/indexer/indexer.data/index.1 > oldfile.sorted
diff newfile.sorted oldfile.sorted
# if nothing printed after diff, no differences found

#   test 2
./indextest $tseoutput/indexer/indexer.data/index.2 newfile
gawk -f indexsort.awk newfile > newfile.sorted
gawk -f indexsort.awk $tseoutput/indexer/indexer.data/index.2 > oldfile.sorted
diff newfile.sorted oldfile.sorted
# if nothing printed after diff, no differences found

## delete all the random files made ##
rm -f testfile newfile
rm -f testfile.sorted legitfile.sorted
rm -f newfile.sorted oldfile.sorted

exit 0

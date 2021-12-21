# testing file for crawler
#
# Elizabeth Frey
# Winter 2021
# CS 50 Lab 4

# Testing File for Crawler
# Written by Elizabeth Frey, Winter 2021, for CS50 Lab 4


## ARGUMENT VALIDATION:
./crawler

./crawler hi

./crawler hi hi

./crawler hi hi hi hi

#   URL is not internal:
./crawler http://google.com hi hi

#   page directory does not exist:
./crawler http://cs50tse.cs.dartmouth.edu/tse/index.html fakeDir 0

#   number is not between 0 and 10 inclusive:
./crawler http://cs50tse.cs.dartmouth.edu/tse/index.html dir -1

./crawler http://cs50tse.cs.dartmouth.edu/tse/index.html dir 20




## TESTING WITH DEPTH 0
# For the first one, I will output a file to show that the pagesaver works. 
# For the rest of the test, I will just output the names of the files in the 
# final directory to see if they are correct. 
# I will also clear the directory each time.


./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html dir 0
# Files created: 
ls dir/
# Example file dir/1:
cat dir/1
# **Clearing dir**
rm -f dir/*



./crawler http://cs50tse.cs.dartmouth.edu/tse//wikipedia/index.html dir 0
# Files created: 
ls dir/
# **Clearing dir**
rm -f dir/*



./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html dir 0
# Files created: 
ls dir/
# **Clearing dir**
rm -f dir/*




## TESTING WITH DEPTH 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html dir 1
# Files created: 
ls dir/
# **Clearing dir**
rm -f dir/*



./crawler http://cs50tse.cs.dartmouth.edu/tse//wikipedia/index.html dir 1
# Files created: 
ls dir/
# **Clearing dir**
rm -f dir/*




## TESTING WITH DEPTH 3
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html dir 3
# Files created: 
ls dir/
# **Clearing dir**
rm -f dir/*



## TESTING WITH DEPTH 6
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html dir 6
# Files created: 
ls dir/
# **Clearing dir**
rm -f dir/*


exit 0

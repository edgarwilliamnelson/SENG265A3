Final C programming assignment for SENG265 at the University of Victoria.
This submission is written in C99 and achieved a grade of 100%. 

Tasked to create a text processing program that generates a concordance for a given text file. (kwoc3.c)

The focus of the assignment was the correct usage and management of dynamic memory in C. 
Although runtime was not an integral part of the grading criteria, the scale of the larger tests produced some noticeable slowdown when the suggested dynamic array or linked-list data structures were used.
Seeking to improve the efficiency of the program I implemented a hashtable data structure (wordHashTable.c) in C99 and was able to improve the runtime of the program from quadratic to linear.

The program can be run with just the input file name as the sole argument, or include an argument flagged with -e to indicate a file containing words to exclude from the concordance:

Example invocations: 

./kwoc3 inputLatin.txt

./kwoc3 -e latin.text inputLatin.txt

./kwoc3 inputEnglish.txt -e english.txt


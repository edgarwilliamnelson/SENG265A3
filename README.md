Final C programming assignment for SENG265 at the University of Victoria.
This project is written in C99 and achieved a grade of 100%. 

Tasked to create a text processing program that generates a concordance for a given text file. (kwoc3.c)

On my first reading of the assignment concordance was a new word to me, it is essentially simply an alphabetical list of keywords in a peice of text. An example of the programs output can be viewed in exampleInvocation.txt.

The focus of the assignment was the correct usage and management of dynamic memory in C. 
Although runtime was not an integral part of the grading criteria, the scale of the larger tests produced some noticeable slowdown when the suggested dynamic array or linked-list data structures were used.

Seeking to improve the efficiency of the program I implemented a hashtable data structure (wordHashTable.c) in C99 and was able to improve the runtime of the program from quadratic to linear.

Once the requisite data structures had been written the implementation was very straightforward. The algorithm proceeds in four steps:

1) Parse the file of words to be excluded and store them in a HashTable (being used as a set in this circumstance).  

2) Two tasks are accomplished in this step:

	a: Scan over the text file and determine what words should be in included in the concordance, this is simply the words that are not present in the excluded words file. Having stored the words to be excluded in a hashtable structure allows us to query if a word from the text is to be excluded from the concordance in constant time.

	The collection of keywords should be sorted alphabetically, and we will have to linerally scan over the collection anyway when producing the output, thus storing the keywords in a linked-list data structure seemed to be a reasonable choice. 

	b: Every line is stored in its own hashtable data structure, the keys in the table are the words that are present in the line and the values are the number of occurences of the word.
	These hashtables are then stored in order in a linked-list data structure. 

	Doing both of these tasks concurrently allows us to only require a single scan of the text to gather all the information we need.

3) Sort the collection of keywords.

4) Produce the output. 

	For each keyword we query each of the hashtables that are maintaing the text that we set up in step 2. Having used a hashtable structure for each line allows us to check if a word is present in a line in constant time rather than having to scan the line again. If the hashtable returns that a word is present in that line, we simply print the word, the line, and the line number. Allowing a reader the see context of each keyword present in the text.

This project was an enjoyable assignment that allowed a lot of freedom in the design decisions. It was a lot of fun to write a hashtable structure that I had often used a library implementation of, and see the efforts pay off via improvements in the programs runtime.

The program may be run with just the input file name as the sole argument, or include an argument flagged with -e that contains words to be excluded from the concordance.

Example invocations: 

./kwoc3 inputLatin.txt

./kwoc3 -e latin.text inputLatin.txt

./kwoc3 inputEnglish.txt -e english.txt


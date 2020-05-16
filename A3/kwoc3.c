/*
 * kwoc3.c
 *
 * Starter file provided to students for Assignment #3, SENG 265,
 * Spring 2020.
 */

#define _GNU_SOURCE
#include <sys/types.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "listy.h"
#include "wordHashTable.h"
#include "htList.h"
void parseExcluded();
void generateKwoc();
void produceOutput();
char *lowerString(char*);
node_t* parseInput();
node_t* parseLine();


/*
 * Concordance generation program.
 *
 * The excluded words are maintained in a "wordTable" hashtable for O(1) lookup.
 * The keywords are maintained in a sorted "node_t" linkedlist.
 *
 * The text is parsed and stored in a linkedlist of hashtable "ht_node" nodes, 
 * the i'th node in the list contains a reference to a "wordTable" hashtable with the i'th line of the text.
 * Each hashtable maintaing a line of the text contains a copy of the line for printing, 
 * and a hashtable of the keywords in the line for O(1) lookup. 
 * 
 */

int main(int argc, char *argv[]){
    char *exception_file = NULL;
    char *input_file = NULL;
    
    //set up of data structures
    struct wordTable *excludedWords = NULL;
    node_t *keywordsHead = NULL;
    ht_node *textHead = NULL;
    int *longestword = emalloc(sizeof(int));
        *longestword = 0;

    //finding input files
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-e") == 0 && i+i <= argc) {
            exception_file = argv[++i];
        } else {
            input_file = argv[i];
        }
    }
    
    //parsing excluded words if given a file
    excludedWords = initTable();
    if(exception_file != NULL)
        parseExcluded(exception_file, excludedWords);
    
    //parsing the input file 
    keywordsHead = parseInput(input_file, longestword, excludedWords, &textHead);

    //generating the kwoc
    generateKwoc(keywordsHead, textHead, longestword);

    
    //freeing allocated memory
    if(excludedWords != NULL)
        freeTable(excludedWords);
    freeHtList(textHead);
    free(longestword);
    deleteList(keywordsHead);
    exit(0);
}



/*
Function to generate the concordance after all input parsing has been done,
The outer loop iterates over the keywords, for every keyword we iterate over every line of the text.
For every line of the text the hastable is queried for the current keyword, and if there is a hit the line is printed.
*/
void generateKwoc(node_t *keywordsHead, ht_node *textHead, int *longestword){
    node_t *currWord = keywordsHead;
    while(currWord != NULL){
        int lineNum = 1;
        ht_node *currLine = textHead;
        while(currLine != NULL){
            wordNum *inLine = query((char *)&(currWord->text), 0, currLine->table);
            if(inLine != NULL){  
                produceOutput(inLine, currLine, *longestword, lineNum);
            }
            lineNum = lineNum + 1;
            currLine = currLine->next;
        }
        currWord = currWord->next;
    }
}

/*
Function to properly format the output once a word has been found in a line 
A local char[] the size of the longest keyword + 2 is declared and filled with an uppercase copy of the
current keyword, the line is then printed with format dependent on the number of occurences in the line.
*/
void produceOutput(wordNum *inLine, ht_node *currLine, int lw, int lineNum){
    char output[lw + 2]; 
    for(int i = 0; i < lw + 4; i++)
        output[i] = ' ';
    char *outputptr = (char*) &output;
    strncpy(outputptr, inLine->word, strlen(inLine->word));                   
    int k = 0;
    
    while(k <= lw + 2){     
        output[k] = toupper(output[k]);                 
        if(k >= strlen(inLine->word)){       
            output[k] = ' ';
        }
    if(k == lw + 2)
        output[k] = '\0';    
    k++;
    }

    if(inLine->occurences >= 2)                                      
        printf("%s%s (%d*)\n", output,currLine->table->line, lineNum);
    else 
        printf("%s%s (%d)\n", output,currLine->table->line, lineNum);    
}

/*
Function to parse the file of excluded words,
to cover the cases of uppercase/lowercase letters, all letters of a word are set to be lowercase 
the word is then inserted in the excluded words hashtable.
*/
void parseExcluded(char *exception_file, struct wordTable *excludedWords){
    char *line = NULL;
    size_t len = 40;
    ssize_t read;
    FILE *fp;
    char delim[] = "  \n";

    fp = fopen(exception_file, "r");
    if(fp == NULL)
      exit(0);
    
    
    while((read = getline(&line, &len, fp)) != -1){
        line = strtok(line, delim);
        char *lowercase = lowerString(line);
        query(lowercase, 1, excludedWords);  
        free(lowercase);
       }
     
     if(line)
        free(line);
    fclose(fp);
}

/*
Function to parse the input file, to avoid having to scan twice, two operations are done here:
1. For every line we store the text of the line in a hashtable "ht_node",
   this node has a reference to a hashtable that maintains the text line, and a hashtable of the words in the line.

2. Every unique keyword in the file is found and stored in a sorted linked list, the head of this list is returned.
*/
node_t* parseInput(char *input_file, int *longestword, struct wordTable *excludedWords, ht_node **textHead){
    char *line = NULL;
    size_t len = 40;
    ssize_t read;
    FILE *fp = fopen(input_file, "r");
     if(fp == NULL)
      exit(0);
    
    //set up of the seenKeywords hashtable and ht_node linked list for the text.
    struct wordTable *seenkeywords = initTable();
    node_t *keywordsHead = NULL;
    ht_node *lineNode;
    
    //iterates over the lines in the text
    while((read = getline(&line, &len, fp)) != -1){

        //create a hashtable for the words and text of this line.
        struct wordTable *lineTable = initTable();
        tableAddLine(line, lineTable);

        //make a ht_node to store this lines hashtable
        lineNode = initHtNode(lineTable);

        //call to a helper function to parse this line.
        keywordsHead = parseLine(line, excludedWords, seenkeywords, lineTable, longestword, keywordsHead);
        
        //add the ht_node representing this line to the list
        if(*textHead == NULL){
            *textHead = lineNode;
        } else {
            ht_node_add_back(*textHead, lineNode); 
        }
    }

    free(line);
    freeTable(seenkeywords);
    fclose(fp);
    return keywordsHead;
}

/*
Helper function to split up the work done in parseInput(),
Handles the job of parsing each individual line. 
*/
node_t *parseLine(char *line, wordTable *excludedWords, 
        wordTable *seenkeywords, wordTable* lineTable, int* longestword, node_t* keywordsHead){

        char delim[] = "  \n";
        char *curr = strtok(line, delim);

        //iterates over the words in the line
        while(curr != NULL){
            char* lowercase = lowerString(curr);

            //add the word to the hashtable of the words in the line.
            //done outside the guard as we count all occurences here, not just unique keywords.
            query(lowercase, 1, lineTable); 
            
            //check if the word is excluded or has been seen.
            if(query(lowercase, 0, excludedWords) == NULL && query(lowercase, 0, seenkeywords) == NULL){
                if(strlen(lowercase) > *longestword)
                    *longestword = strlen(lowercase);
                          
                //add the keyword to the hashtable for seen keywords
                query(lowercase, 1, seenkeywords);
                
                //add the keyword to the sorted list of keywords
                if(keywordsHead == NULL){
                    keywordsHead = new_node(lowercase);
                } else {
                node_t *toAdd = new_node(lowercase);  
                keywordsHead = insert_sorted(keywordsHead, toAdd);
                }
            }
                free(lowercase);
                curr = strtok(NULL, delim);
        }

        return keywordsHead;
    }




/*
Returns a reference to a lowercase copy of a *char.
*/
char *lowerString(char *word){
    char *result = emalloc(sizeof(char) * ((strlen(word)) + 1));
    
    for(int i = 0; i < strlen(word); i++){
    *(result + i) = tolower(*(word + i));
    }
    *(result + strlen(word)) = '\0';
    return result;
}






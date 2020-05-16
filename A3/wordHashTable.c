/*
Hashtable implementation for UVIC SENG265 A3, 
Credit for the hash function and overall design structure is owed
to the class lecture slides.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "wordHashTable.h"

//Tried to pick a good table size for the assignment scope.
int WORDTABLESIZE = 300;

/*
Instantiates a new hashtable.
*/
struct wordTable* initTable(){
    struct wordTable *table = emalloc(sizeof(struct wordTable));
    table->size = 0;
    table->array = emalloc(sizeof(wordNum) * WORDTABLESIZE);

    //scanning over and setting the allocated memory to NULL here, 
    //this is done to fix an error indicated by valgrind for using an uninitialized value.
    for(int i = 0; i < WORDTABLESIZE; i++){   
        wordNum* curr = &(table->array[i]);
        curr->word = NULL;
        curr->occurences = 0;
        curr->next = NULL;
    }

    return table;
}


/*
Adding a line of the text to a hashtable
*/
void tableAddLine(char *text, struct wordTable *table){
    strncpy(table->line, text, strlen(text));
    table->line[strlen(text) - 1] = '\0';
}

/*
Used if there is no collision, directly inserts a wordNum structure into a table's array.
The new* reference should be a reference directly to a table's array
*/
void insertWord(char *str, struct wordNum *new){
    new->word = emalloc(sizeof(char) * (strlen(str) + 1));
    
    for(int i = 0; i < strlen(str) + 1; i++){
    new->word[i] = 0;
    }

    strncpy(new->word, str, strlen(str));
    new->next = NULL;
    new->occurences = 1;
}

/*
Used if there is a collison, instantiates a wordnum struct and returns it,
this is then appended to the end of an array indices chain.
*/
struct wordNum* appendWord(char *str){
    struct wordNum *new = emalloc(sizeof(struct wordNum));
    new->word = emalloc(sizeof(char) * (strlen(str) + 1));

    strncpy(new->word, str, strlen(str) + 1);
    new->occurences = 1;
    new->next = NULL;
    return new;


}


/*
Free function for a wordTable.
We scan over the tables array, freeing any chains of wordNums found
we then free the allocated array, and finally the table structure itself.
*/
void freeTable(struct wordTable *table){

    for(int i = 0; i < WORDTABLESIZE; i++){
        wordNum *curr = &(table->array[i]);
        int listnode = 0;
        while(curr->word != NULL){
            if(strlen(curr->word) > 0){
                free(curr->word);
            }
            if(curr->next != NULL){
                wordNum *next = curr->next;
                if(listnode)
                    free(curr);
                curr = next;
                listnode = 1;
                continue;
            }
            if(listnode)
                free(curr);
        break;
        }
    }

    free(table->array);
    free(table);
}


/*
All credit for the hashing method goes to the course slides.
the paramater word is hashed and then taken mod (table size) to ensure we get a valid 
array index.
*/
unsigned int hash(char *word){
    unsigned int h = 0;
    for (unsigned char *p = (unsigned char *) word; *p != '\0'; p++) 
        h = 37 * h + *p;

   return h % WORDTABLESIZE;   //mod size of table
}



/*
Function to lookup or insert a value in the hashtable,
if insert is given as 1 the word will be added to the table, or the number of occurences of the word will be incremented.
if insert is 0 and the word is present in the table, the wordNum representing that word will be returned
if insert is 0 and the word is not in the table, then NULL is returned.
*/
wordNum *query (char *word, int insert, struct wordTable *table){
   if(table == NULL){
        return NULL;
    }

    wordNum *curr;
    wordNum *last;
    int index = hash(word);
    
    for (curr = &(table->array[index]); curr->word != NULL; curr = curr->next) {
        if (strcmp(word, curr->word) == 0){
            if(insert){
                curr->occurences = curr->occurences + 1;  
            }
            return curr;
        } 
        last = curr;
        if(curr->next == NULL)
            break;        
    }
    
    if(insert) {
        if(table->array[index].word == NULL){
            insertWord(word, &(table->array[index]));
            return &(table->array[index]);
        } else {
            wordNum *new = appendWord(word);
            last->next = new;
            return new;
        }
    }
    return NULL;
}



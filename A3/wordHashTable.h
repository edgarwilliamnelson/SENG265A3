#ifndef _wordHashTable_H_
#define _wordHashTable_H_


/*
Data type for a pair of a word and its occurences in a line, for the hashtable structure's array.
Contains a reference to the next wordNum to facilitate seperate chaining collision resolution.
*/
typedef struct wordNum wordNum;
struct wordNum {
    char *word;
    int occurences;
    struct wordNum *next;
};

/*
Hashtable structure to store words.
*/
typedef struct wordTable wordTable;
struct wordTable {
    int size;
    char line[100];
    wordNum *array;
};



struct wordNum* appendWord(char *str);
struct wordNum *query (char *word, int insert, struct wordTable *table);
struct wordTable * initTable();
unsigned int hash(char *word);
void freeTable(struct wordTable *table);
void insertWord(char *str, struct wordNum *new);
void tableAddLine(char *text, struct wordTable *table);




#endif

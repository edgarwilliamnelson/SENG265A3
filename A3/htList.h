#ifndef _htList_H_
#define _htList_H_
//
/*
Structure for a linkedlist of hashtables, used to maintain the text.
*/
typedef struct ht_node ht_node;
struct ht_node {
    struct wordTable *table;
    struct ht_node  *next;
};    

void freeHtList();
struct ht_node* initHtNode();
struct ht_node* ht_node_add_back();

#endif


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "wordHashTable.h"
#include "htList.h"

/*
Instantiates a htNode.
*/
struct ht_node* initHtNode(struct wordTable *table){
       struct ht_node *new = emalloc(sizeof(struct ht_node));
       new->table = table;
       new->next = NULL;
       return new;
   }

/*
Adds a htNode to the back of an htList
*/
struct ht_node *ht_node_add_back(ht_node *head, ht_node *new) {
    ht_node *curr;
    if (head == NULL) {
        new->next = NULL;
        return new;
    }
    
    for (curr = head; curr->next != NULL; curr = curr->next);
    curr->next = new;
    new->next = NULL;
    return new;    
}


/*
Free function for an htList
*/
void freeHtList(ht_node *head){
     ht_node *curr = head;
     while(curr != NULL){
        ht_node *toDelete = curr;
        curr = curr->next;
        freeTable(toDelete->table);
        free(toDelete);
    }
}


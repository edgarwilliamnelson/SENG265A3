/*
 * linkedlist.c
 *
 * Based on the implementation approach described in "The Practice 
 * of Programming" by Kernighan and Pike (Addison-Wesley, 1999).
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "listy.h"

node_t *new_node(char *text) {
    assert( text != NULL);
    node_t *temp = (node_t *)emalloc(sizeof(node_t));
    strncpy(temp->text, text, LIST_MAXLEN_TEXT);
    temp->next = NULL;
    return temp;
}

node_t *add_front(node_t *list, node_t *new) {
    new->next = list;
    return new;
}

node_t *add_end(node_t *list, node_t *new) {
    node_t *curr;
    if (list == NULL) {
        new->next = NULL;
        return new;
    }
    for (curr = list; curr->next != NULL; curr = curr->next);
    curr->next = new;
    new->next = NULL;
    return new;    
}


node_t *peek_front(node_t *list) {
    return list;
}


node_t *remove_front(node_t *list) {
    if (list == NULL) {
        return NULL;
    }

    return list->next;
}


void apply(node_t *list,
           void (*fn)(node_t *list, void *),
           void *arg)
{
    for ( ; list != NULL; list = list->next) {
        (*fn)(list, arg);
    }
}


/*
Free function for a char* linked list,
*/
void deleteList(node_t *head){
    node_t *curr = head;
    while(curr != NULL){
        node_t *toDelete = curr;
        curr = curr->next;
        free(toDelete);
    }
}


/*
O(n) insertion algorithm, given a sorted list, this function iterates through the list
until the first element larger than the element to be inserted is found,
the given element is then inserted just before the larger element.
*/
node_t* insert_sorted(node_t *head, node_t *toAdd){
    node_t *curr;
    node_t *before;
    int ishead = 1;
    for (curr = head; curr != NULL; curr = curr->next){
        if(strcmp(curr->text, toAdd->text) > 0){
            if(ishead){
                toAdd->next = curr;
                return toAdd;     
            }     
            before->next = toAdd;
            toAdd->next = curr;
            return head;
        }
        ishead = 0;
        before = curr;

    }
    before->next = toAdd;
    return head;
}
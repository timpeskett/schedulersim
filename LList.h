/* LList.h
 * A simple linked list implementation.
 * The nodes contain void pointers so as to allow
 * some degree of type genericity.
 * The nodes in the list are indexed (where applicable)
 * starting at 1. This is important but highlights a conceptual
 * difference between lists and arrays.
 * By Tim Peskett
 * 21/4/14
 */

#ifndef LLIST_H
#define LLIST_H

#include <stdlib.h>
#include "common.h"


/* A structure to represent a single node of the list.
 * Contains a user-supplied pointer to some data and a pointer
 * to the next node in the list.*/
typedef struct LLNode
{
	void *data;
	struct LLNode *next;
} LLNode;


/* A structure to represent a linked list. Contains a pointer to
 * the head of the list (NULL if list is empty) and a count of the
 * number of nodes (or the size) of the list.*/
typedef struct
{
	LLNode *head;
	size_t count;
} *LList;


/* LIST CREATION/DESTRUCTION */

/****
 * listCreate
 * Description: Creates a new empty linked list. The list is singly linked.
 * Function takes no arguments.
 * Returns a new list. This list should be freed with either listDestroy
 * or listDestroyFree.
 */
LList listCreate( void );


/****
 *  listCopy
 *  Description: Creates a shallow copy of a list. Simply copies the data
 *  	inside the nodes. Does not copy what the data may point towards.
 *  	The caller should be careful about whether a list created with listCopy
 *  	should be freed with listDestroy or listDestroyFree.
 *
 *  LList inList - The list to be copied. Must be non-null.
 *
 *  Returns a new list that is a shallow copy of listCopy.
 */
LList listCopy( LList inList );


/* Will not free any of the data inside the nodes */
/****
 * listDestroy
 * Description: Destroys a linked list. Does not free the data INSIDE the
 * nodes, just the nodes themselves. Any allocated data inside the nodes would
 * need to be free by the caller or by calling listDestroyFree.
 *
 *  LList list - The list to be destroy
 *
 * Returns nothing.
 */
void listDestroy( LList list );


/* Will attempt to free all data inside nodes */
/****
 * listDestroyFree
 * Description: Destroys a linked list. Attempts to free the data inside the 
 * nodes. This should NOT be used when the data inside the nodes has not been
 * dynamically allocated. Do not mix dynamic and static memory in your list and
 * then use this function.
 *
 *  LList list - The list to destroy
 *  
 * Returns nothing.
 */
void listDestroyFree( LList list );


/* LIST ACCESS */

/* Position is between 1 and count */
/****
 * listAdd
 * Description: Adds an element to a linked list. Note that linked list indexes
 * are 1-based rather than zero based.
 *
 *  LList list - The list to add the data to. Must be non-null.
 *  void *data - The data to add to the list.
 *  int position - The position in the list to add the data. Must be between
 *  	1 and listSize( list ).
 *
 * Returns nothing.
 */
void listAdd( LList list, void *data, int position );


/***
 * *listGet
 * Description: Retrieves the data in the specified list node.
 *
 * LList list - The list that the data resides in. Must be non-null.
 * int position - The position of the data. Must be between 1 and
 * 	listSize( list ).
 *
 * Returns a void * to the data inside the node.
 */
void *listGet( LList list, int position );


/****
 *  *listRemove 
 *  Description: Removes a node from the list.
 *  LList list - The list to remove the node from. Must be non-null.
 *  int position - The position from which to remove the node. Must be
 *  	between 1 and listSize( list ).
 *
 *  Returns the data that was inside the removed node.
 */
void *listRemove( LList list, int position );


/* OTHER LINKED LIST FUNCTIONS */

/****
 *  listSize
 *  Description: The number of nodes in the linked list.
 *
 *  LList list - The list whose size is desired. Must be non-null.
 *
 *  Returns the size (number of nodes) of the linked list.
 */
size_t listSize( LList list );


/* Sorts the list by comparing the elements using the caller-supplied
 * callback 'cmp'. cmp should return -1 when arg1 < arg2, 0 when arg1 = arg2,
 * and 1 when arg1 > arg2 */
/****
 *  listSort
 *  Description: Sorts the list by comparing the elements using the caller-supplied
 *  	callback 'cmp'. cmp should return -1 when arg1 < arg2, 0 when arg1 = arg2,
 * 	and 1 when arg1 > arg2 
 *
 *  LList list - The list to sort. Must be non-null.
 *  int (*cmp)( const void *, const void* ) - The comparison function supplied by
 *  	the caller. Must be non-null.
 *
 *  Returns nothing however the list will now be sorted.
 */
void listSort( LList list, int (*cmp)( const void *arg1, const void *arg2 ) );


#endif

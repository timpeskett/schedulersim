#include "LList.h"


LList listCreate( void )
{
	LList newList;

	/* Simply create list and initialise all to zero */
	newList = malloc( sizeof *newList );

	newList->head = NULL;
	newList->count = 0;

	return newList;
}


/* A copied list should NOT be freed with listDestroyFree */
LList listCopy( LList inList )
{
	int i;
	LList outList;

	assert( inList != NULL );

	/* Create new list */
	outList = listCreate();
	/* Do a shallow copy of all the list data */
	for( i = 1; i <= listSize( inList ); i++ )
	{
		listAdd( outList, listGet( inList, i ), i );
	}

	return outList;
}


void listDestroy( LList list )
{
	while( listSize( list ) > 0 )
	{
		/* Not required to free data here.*/
		listRemove( list, 1 );
	}
	free( list );
}



void listDestroyFree( LList list )
{
	while( listSize( list ) > 0 )
	{
		/* Assuming that all data in nodes is dynamically
		 * allocated memory. */
		free( listRemove( list, 1 ) );
	}
	free( list );
}



void listAdd( LList list, void *data, int position )
{
	/* Double pointers are used here to collapse the "add to head" case
	 * and the "add to middle" cases into one.*/
	LLNode **newPos, *newNode;
	int i;

	assert( 1 <= position && position <= list->count + 1 );

	/* Create new node */
	newNode = malloc( sizeof *newNode );
	newNode->data = data;

	/* Find node currently at 'position' and point new node at it */
	newPos = &list->head;
	for( i = 1; i < position; i++ )
	{
		newPos = &(*newPos)->next;
	}
	newNode->next = *newPos;

	/* Place the new node at correct position in list */
	*newPos = newNode;
	list->count++;
}



void *listGet( LList list, int position )
{
	LLNode *curNode;
	int i;

	assert( 1 <= position && position <= list->count );

	/* Start at head and 'loop' through our list */
	curNode = list->head;
	for( i = 1; i < position; i++ )
	{
		curNode = curNode->next;
	}

	return curNode->data;
}
		


void *listRemove( LList list, int position )
{
	/* Double pointers used again here to make deleting the
	 * 'head' node much easier */
	LLNode **delPos, *delNode;
	void *data;
	int i;

	assert( 1 <= position && position <= list->count );

	/* Find the address of the pointer to our desired node */
	delPos = &list->head;
	for( i = 1; i < position; i++ )
	{
		delPos = &(*delPos)->next;
	}
	
	/* Store the node for deletion */
	delNode = *delPos;
	/* Change the preceding node to point to the succeeding node */
	*delPos = delNode->next;
	list->count--;

	data = delNode->data;
	free( delNode );

	return data;
}



size_t listSize( LList list )
{
	assert( list != NULL );

	return list->count;
}


/* A most inefficient yet simple list sorting algorithm. Essentially
 * a selection sort. Finds the max element in the subarray and places
 * it at the front of the list */
void listSort( LList list, int (*cmp)( const void *arg1, const void *arg2 ) )
{
	int maxIndex, subArrayStart;
	int i;

	assert( list != NULL );
	assert( cmp != NULL );

	/* Subarray will shrink by 1 each time as we move max element of subarray to
	 * front of the list */
	for( subArrayStart = 1; subArrayStart <= listSize( list ); subArrayStart++ )
	{
		/* Loop through our subarray and find the max element */
		maxIndex = subArrayStart;
		for( i = subArrayStart + 1; i <= listSize( list ); i++ )
		{
			if( cmp( listGet( list, maxIndex ), listGet( list, i ) ) <= 0 )
			{
				maxIndex = i;
			}
		}
		/* Max element found. Move it out of the subarray and to the start of
		 * our list */
		listAdd( list, listRemove( list, maxIndex ), 1 );
	}
}



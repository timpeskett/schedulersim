#include "Gantt.h"


Gantt ganttCreate( int startTime )
{
	Gantt newGantt;

	/* Allocates the gantt chart and creates the empty
	 * list to hold the blocks. */
	newGantt = malloc( sizeof *newGantt );
	newGantt->blockList = listCreate();
	newGantt->startTime = startTime;

	return newGantt;
}


void ganttDestroy( Gantt inGantt )
{
	assert( inGantt != NULL );

	/* Using listDestroyFree will free all of the
	 * struct block * inside the list for us. */
	listDestroyFree( inGantt->blockList );
	free( inGantt );
}


/* Always adds a block to the  end */
void ganttAddBlock( Gantt inGantt, int id, int totalTime )
{
	struct block *newBlock;

	assert( inGantt != NULL );
	assert( totalTime >= 0 );

	/* Create and initialise new block */
	newBlock = malloc( sizeof *newBlock );
	newBlock->id = id;
	
	/* Find the appropriate start time for this new block */
	if( listSize( inGantt->blockList ) != 0 )
	{
		struct block *lastBlock;

		lastBlock = listGet( inGantt->blockList, listSize( inGantt->blockList ) );
		/* The startTime is the same as the endTime of the previous block */
		newBlock->startTime = lastBlock->endTime;
	}
	else
	{
		/* This is our first block. The start time is equal to the start
		 * time of our gantt chart */
		newBlock->startTime = inGantt->startTime;
	}

	newBlock->endTime = newBlock->startTime + totalTime;

	/* Add the new block to the END of our list */
	listAdd( inGantt->blockList, newBlock, listSize( inGantt->blockList ) + 1 );
}



int ganttNumBlocksById( Gantt inGantt, int id )
{
	int numBlocks = 0;
	int i;

	assert( inGantt != NULL );

	if( listSize( inGantt->blockList ) != 0 )
	{
		/* Loop through our list of blocks and compare block IDs with
		 * id */ 
		for( i = 1; i <= listSize( inGantt->blockList ); i++ )
		{
			struct block *curBlock;

			curBlock = listGet( inGantt->blockList, i );
			if( curBlock->id == id )
			{
				numBlocks++;
			}
		}
	}
	
	return numBlocks;
}


struct block *ganttGetBlockById( Gantt inGantt, int id, int n )
{
	int numBlocks = 0;
	struct block *outBlock;
	int i;

	assert( 1 <= n && n <= ganttNumBlocksById( inGantt, id ) );

	/* Loop through the entire list and increment numBlocks if
	 * the block ID is equal to ids. Once numBlocks reaches n we will
	 * have our desired block. */
	i = 1;	
	while( numBlocks < n )
	{
		struct block *curBlock;

		curBlock = listGet( inGantt->blockList, i );
		if( curBlock->id == id )
		{
			numBlocks++;
			outBlock = curBlock;
		}
		i++;
	}

	return outBlock;
}



int ganttNumBlocks( Gantt inGantt )
{
	return listSize( inGantt->blockList );
}	


struct block *ganttGetBlock( Gantt inGantt, int n )
{
	assert( 1 <= n && n <= ganttNumBlocks( inGantt ) );

	return listGet( inGantt->blockList, n );
}


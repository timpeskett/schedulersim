/* Gantt.h
 * An implementation of a gantt chart
 * data structure. 'Blocks' are added
 * to the chart. Blocks take a certain amount
 * of time and have a block id. Blocks can only
 * be added to the end as that is all that is
 * required here.
 * By Tim Peskett
 * 23/4/14
 */

#ifndef GANTT_H
#define GANTT_H

#include <stdlib.h>
#include "common.h"
#include "LList.h"


/* A structure to represent a 'block' of time in a 
 * gantt chart. A block is any contiguous allocation
 * of time to a single entity.
 *
 * e.g
 * 
 * |____1____|_2_|___3___|__2__|___3___|
 * 0	     6   7       9    11      15
 *
 * This example gantt chart contains 5 "blocks".
 * There is: 1 block with id 1, 2 blocks with id 2
 * and 2 blocks with id 3.
 * The id is just a number to identify the block. It is
 * up to the caller to ensure that these ids are meaningful.
 * The startTime is the time at which the block starts. For example,
 * the first block with ID 2 in our above diagram begins at time 6.
 * The finishTime is the time at which the block ends. Note that this
 * will be the start of the second AFTER the block has finished rather
 * than the final second BEFORE the block finishes. For example, the first
 * block with ID 3 in our above example finishes at 9 rather than 8.
 */
struct block
{
	int id;
	int startTime;
	int endTime;
};


/* A structure to represent a Gantt chart. A Gantt chart is simply
 * a collection of blocks (see above). For simplicity we have used a
 * linked list but any representation would do.
 * startTime is the startTime of the first block in the chart. This is set
 * to allow our gantt chart to start at a time other than 0.*/
typedef struct Gantt
{
	LList blockList;
	int startTime;
} *Gantt;


/* CREATION/DESTRUCTION */


/****
 *  ganttCreate
 *  Description: Creates an empty gantt chart.
 *
 *  int startTime - The start time of the first block in the gantt
 *  	chart. Note that the first block need not be added straight away.
 *  
 *  Returns a new empty Gantt chart.
 */
Gantt ganttCreate( int startTime );


/****
 *  ganttDestroy
 *  Description: Destroys a gantt chart and all data associated with it.
 *  	The caller need not worry about the gantt chart or any of the blocks
 *  	in it after this function has been called on the chart.
 *
 *  Gantt inGantt - The gantt chart to be destroyed. Must be non-null.
 *
 *  Returns nothing.
 */
void ganttDestroy( Gantt inGantt );


/* BLOCK ACCESS/ADDING */

/****
 *  ganttAddBlock
 *  Description: Adds a new block of time to the end of the chart.
 *
 *  Gantt inGantt - The gantt chart to add the block to. Must be non-null.
 *  int id - The id of the new block.
 *  int totalTime - The total time allocated to the new block. Must be non-negative.
 *
 *  Returns nothing.
 */
void ganttAddBlock( Gantt inGantt, int id, int totalTime );


/****
 *  ganttNumBlocksById
 *  Description: Returns the number of blocks in the chart with the given id.
 *
 *  Gantt inGantt - The gantt chart to search. Must be non-null.
 *  int id  - The id to search for in the chart.
 *
 *  Returns the number of blocks found with the given id.
 */
int ganttNumBlocksById( Gantt inGantt, int id );


/****
 *  ganttGetBlockById
 *  Description: Returns the nth block in the chart that has
 *  	ID of id. Can be viewed as taking the subset of blocks
 *  	with id of ID and then finding the nth member of that 
 *  	subset. Note that indexing of blocks is 1-based rather
 *  	than 0-based.
 *
 *  Gantt inGantt - The gantt chart to search. Must be non-null.
 *  int id - The id to search for. The id must be present somewhere
 *  	in the chart.
 *  int n - The index of the desired block. Must be between 1 and 
 *  	ganttNumBlocksById( inGantt, id ) inclusive.
 *
 *  Returns a struct block * that points to the desired block. This block
 *  should NOT be freed by the caller.
 */
struct block *ganttGetBlockById( Gantt inGantt, int id, int n );


/****
 *  ganttNumBlocks
 *  Description: Retrieve the number of blocks in the chart. 
 *  
 *  Gantt inGantt - The gantt chart to find the size of. Must be non-null.
 *
 *  Returns the number of blocks in the chart.
 */
int ganttNumBlocks( Gantt inGantt );


/****
 *  *ganttGetBlock
 *  Description: Retrieves the nth block in the chart. Indexes into
 *  	the chart are 1-based rather than 0-based.
 *
 *  Gantt inGantt - The chart to search. Must be non-null.
 *  int n - The index of the desired block. Must be between 1 and
 *  	ganttNumBlocks( inGantt) inclusive.
 *
 *  Returns a struct block * that points to the required block. This block
 *  should NOT be freed by the caller.
 */
struct block *ganttGetBlock( Gantt inGantt, int n );


#endif

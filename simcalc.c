#include "simcalc.h"


int cmp( int arg1, int arg2 )
{
	int result;

	if( arg1 == arg2 )
	{
		result = 0;
	}
	else
	{
		/* Dividing the difference between the absolute
		 * value of the difference will give us 1 with either
		 * + or - sign.*/
		result = ( arg1 - arg2 ) / abs( arg1 - arg2 );	
	}

	return result;
}


double calcAveWait( Gantt gantt, LList inProcList )
{
	int procIdx;
	int waitTime = 0;

	assert( gantt != NULL );
	assert( inProcList != NULL && listSize( inProcList ) > 0 );

	/* Loop through all of the processes in our list and for each
	 * calculate the waiting time from the gantt chart */
	for( procIdx = 1; procIdx <= listSize( inProcList ); procIdx++ )
	{
		struct proc *curProc;
		int numBlocks, blockIdx;
		int procWaitTime, lastTime;

		curProc = listGet( inProcList, procIdx );
		/* The last time that the process was running.*/
		lastTime = curProc->arrivalTime;
		procWaitTime = 0;

		/* Loop through blocks that correspond to current process
		 * and calculate time that they are NOT running */
		numBlocks = ganttNumBlocksById( gantt, curProc->pNo );
		for( blockIdx = 1; blockIdx <= numBlocks; blockIdx++ )
		{
			struct block *gBlock;

			gBlock = ganttGetBlockById( gantt, curProc->pNo, blockIdx );
			procWaitTime += gBlock->startTime - lastTime;
			lastTime = gBlock->endTime;
		}
		waitTime += procWaitTime;
	}

	return waitTime / (double)listSize( inProcList );
}


double calcAveTurn( Gantt gantt, LList inProcList )
{
	int procIdx;
	int turnTime = 0;

	assert( gantt != NULL );
	assert( inProcList != NULL && listSize( inProcList ) > 0 );

	/* Loop through all of the processes in our list and find the final
	 * block that belongs to that process in our gantt chart */
	for( procIdx = 1; procIdx <= listSize( inProcList ); procIdx++ )
	{
		struct proc *curProc;
		struct block *gBlock;
		int numBlocks;

		/* Get next process */	
		curProc = listGet( inProcList, procIdx );

		/* Find last block that belongs to process curProc */
		numBlocks = ganttNumBlocksById( gantt, curProc->pNo );
		gBlock = ganttGetBlockById( gantt, curProc->pNo, numBlocks );

		/* Calculate turnaround time */
		turnTime += gBlock->endTime - curProc->arrivalTime;
	}

	return turnTime / (double)listSize( inProcList );
}

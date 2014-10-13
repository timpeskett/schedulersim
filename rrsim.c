#include "rrsim.h"


int rrCmp( const void *arg1, const void *arg2 )
{
	const struct proc *p1, *p2;

	p1 = arg1;
	p2 = arg2;
	
	return cmp( p1->arrivalTime, p2->arrivalTime );
}


void rrSim( LList inProcList, int quantum, double *outAveWait, double *outAveTurn )
{
	LList procList, rdyQueue;
	struct rrData *runningProc;
	Gantt gantt;
	int timer;

	assert( inProcList != NULL );
	assert( quantum > 0 );
	assert( outAveWait != NULL && outAveTurn != NULL );

	/* Create the ready queue */
	rdyQueue = listCreate();

	/* Create a copy of inProcList so that we can sort it
	 * by arrival time.*/
	procList = listCopy( inProcList );		
	listSort( procList, rrCmp );

	/* Create our gantt chart. The start time will be the start time of the
	 * first arrivaing process. Need to ensure that it will work if our
	 * list size is 0 */
	if( listSize( inProcList ) > 0 )
	{
		gantt = ganttCreate( ( (struct proc*)listGet( procList, 1 ) )->arrivalTime );
	}
	else
	{
		gantt = ganttCreate( 0 );
	}


	/* There is no currently running process and we are at time 0 */
	runningProc = NULL;
	timer = 0;
	/* Terminate when both ready queue and list of processes is empty and when
	 * there is no currently running process */
	while( !( listSize( rdyQueue ) == 0 && listSize( procList ) == 0 && runningProc == NULL )  )
	{
		/* Add any arrived processes to the ready queue */
		while( listSize( procList ) > 0 && ((struct proc*)listGet( procList, 1 ))->arrivalTime == timer )
		{
			struct rrData *procData;
			procData = malloc( sizeof( *procData ) );
			procData->proc = (struct proc*)listRemove( procList, 1 );
			procData->runTime = 0;
			procData->remaining = procData->proc->burstTime;
			
			/* Add process (with housekeeping data) to our ready queue */
			listAdd( rdyQueue, procData, listSize( rdyQueue ) + 1 );
		}

		/* Check to see if a process is ready to run */
		if( runningProc == NULL && listSize( rdyQueue ) > 0 )
		{
			/* Remove process from ready queue and begin
			 * executing it */
			runningProc = listRemove( rdyQueue, 1 );
			runningProc->runTime = 0;
		}

		/* Check to see whether process should be
		 * swapped out*/
		if( runningProc != NULL )
		{
			runningProc->runTime++;
			runningProc->remaining--;

			/* Process's burst time is completed */
			if( runningProc->remaining == 0 )
			{
				ganttAddBlock( gantt, runningProc->proc->pNo, runningProc->runTime );
				/* Free our data here because it has been
				 * removed from the ready queue already */
				free( runningProc );
				runningProc = NULL;
			}
			/* Process's quantum is up. Pre-empt. */
			else if( runningProc->runTime >= quantum )
			{
				ganttAddBlock( gantt, runningProc->proc->pNo, runningProc->runTime );
				listAdd( rdyQueue, runningProc, listSize( rdyQueue ) + 1 );
				runningProc = NULL;
			}
		}
		timer++;
	}

	/* Make sure list is non-empty to avoid divide by zero errors */
	if( listSize( inProcList ) > 0 )
	{
		*outAveWait = calcAveWait( gantt, inProcList );
		*outAveTurn = calcAveTurn( gantt, inProcList );
	}
	else
	{
		*outAveWait = *outAveTurn = 0.0;
	}

	/* Clean up our used memory */
	listDestroy( procList );
	listDestroyFree( rdyQueue );
	ganttDestroy( gantt );
}

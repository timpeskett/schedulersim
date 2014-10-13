#include "sjfsim.h"


int sjfBurstCmp( const void *arg1, const void *arg2 )
{
	const struct sjfData *p1, *p2;

	p1 = arg1;
	p2 = arg2;

	return cmp( p1->proc->burstTime, p2->proc->burstTime );
}


int sjfCmp( const void *arg1, const void *arg2 )
{
	const struct proc *p1, *p2;
	int result;

	p1 = arg1;
	p2 = arg2;

	if( cmp( p1->arrivalTime, p2->arrivalTime ) == 0 )
	{
		result = cmp( p1->burstTime, p2->burstTime );
	}
	else
	{
		result = cmp( p1->arrivalTime, p2->arrivalTime );
	}

	return result;	
}


void sjfSim( LList inProcList, double *outAveWait, double *outAveTurn )
{
	LList procList, rdyQueue;
	struct sjfData *runningProc;
	Gantt gantt;
	int timer;

	assert( inProcList != NULL );
	assert( outAveWait != NULL && outAveTurn != NULL );

	/* Create the ready queue */
	rdyQueue = listCreate();

	/* Create a copy of inProcList so that we can sort it by arrival and
	 * burst time */
	procList = listCopy( inProcList );		
	listSort( procList, sjfCmp );

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
			struct sjfData *procData;
			procData = malloc( sizeof( *procData ) );
			procData->proc = (struct proc*)listRemove( procList, 1 );
			procData->remaining = procData->proc->burstTime;
			
			/* Add process (with housekeeping data) to our ready queue */
			listAdd( rdyQueue, procData, listSize( rdyQueue ) + 1 );
			/* Resort our ready queue to make sure the shortest job is
			 * first */
			listSort( rdyQueue, sjfBurstCmp );
		}

		/* Check to see if a process is ready to run */
		if( runningProc == NULL && listSize( rdyQueue ) > 0 )
		{
			runningProc = listRemove( rdyQueue, 1 );
		}

		/* Check to see whether process has finished running */
		if( runningProc != NULL )
		{
			runningProc->remaining--;

			/* Process's burst time has completed */
			if( runningProc->remaining == 0 )
			{
				ganttAddBlock( gantt, runningProc->proc->pNo, runningProc->proc->burstTime );

				/* Need to free here because we have already removed
				 * from the ready queue */
				free( runningProc );
				runningProc = NULL;
			}
		}
		timer++;
	}

	/* Ensure that list is not empty so as to avoid divide by zero errors */
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

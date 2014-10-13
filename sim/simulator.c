#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "../rrsim.h"
#include "../sjfsim.h"

/* The maximum filename length */
#define FILENAME_LEN 10

#define NUMWORKERS 2
#define NONE -1
#define SJF 0
#define RR 1


/* A mutex used to control access to the fNameRead state variable and
 * buffer1.
 * These variables should only be read/written when this mutex is held */
static pthread_mutex_t readMut;
/* Condition variables to signal when fNameRead[0] and fNameRead[1] are ready
 * to be read/written. */
static pthread_cond_t sjfReadCond = PTHREAD_COND_INITIALIZER, rrReadCond = PTHREAD_COND_INITIALIZER;
/* A state variable to signal when the workers have finished reading the filename
 * from buffer1. This is set to 1 when the filename HAS been read and set to 0
 * when the main thread writes a new filename and it HAS NOT yet been read. */
static int fNameRead[NUMWORKERS] = { TRUE, TRUE };
/* The buffer to hold the filename to be read by the worker threads. This is
 * named buffer1 as per the assignment specification. A more appropriate name
 * would be fNameBuffer or something of that sort.*/
static char buffer1[FILENAME_LEN + 1];

/* A mutex used to control access to the writer, buffer2 and fError 
 * variables. These variables should only be read/written when this mutex is
 * held. */
static pthread_mutex_t writeMut;
/* A condition variable to signal when the value of state variable writer has
 * changed and writing/reading is ready to be done on buffer2 */
static pthread_cond_t bufWriteCond = PTHREAD_COND_INITIALIZER;
/* A state variable to signal whose turn it is to write to buffer2. When
 * writer = NONE, any worker thread is allowed to write to buffer2. If writer
 * = SJF then the main thread must read buffer2 and set writer = NONE. If
 * writer = RR then the main thread must read buffer2 and set writer = NONE.*/
static int writer = NONE;
/* The buffer to hold the average waiting time and average turnaround time
 * for the simulations. writeMut must be held when this is written to. */
static double buffer2[2];
/* A state variable to signal when there has been an error reading the file.
 * This could be avoided by having the thread itself output an error message but
 * in this situation I felt it better to do it in the main thread. */
static int fError;



/****
 *  sjfThread
 *  Description: A thread to simulate a shortest job first cpu scheduling
 *  algorithm on a list of processes. The name of the file containing the 
 *  processes should be written to global varable buffer1 and then read
 *  using mutal exclusion. The result will be written to buffer2 also 
 *  inside a critical section.
 */
void *sjfThread( void *attr )
{
	/* local variable to copy the filename into */
	char inFileName[FILENAME_LEN + 1];

	/* Loop until our fileName is QUIT */
	do
	{
		/* Wait for a lock to read fNameRead */
		pthread_mutex_lock( &readMut );
		/* Need to loop to prevent spurious wakeups */
		while( fNameRead[SJF] != FALSE )
		{
			/* Wait until sjfReadCond has been met */
			pthread_cond_wait( &sjfReadCond, &readMut );
		}
		strcpy( inFileName, buffer1 );
		/* Filename has now been read */
		fNameRead[SJF] = TRUE;
		/* Let the mutex go and tell the main thread that we have
		 * finished reading */
		pthread_mutex_unlock( &readMut );
		pthread_cond_signal( &sjfReadCond );

		if( strcmp( "QUIT", inFileName ) != 0 )
		{
			LList procList;
			int quantum;
			int fileError;
			double aveTurn, aveWait;

			/* Read processes from file */
			procList = listCreate();
			fileError = procReadFile( inFileName, procList, &quantum );

			/* Compute sjf times before entering critical section */
			if( fileError == PROC_NO_ERROR )
			{
				sjfSim( procList, &aveWait, &aveTurn );
			}

			/* Obtain a lock on writeMut so we can write result to
			 * buffer2 */
			pthread_mutex_lock( &writeMut );
			while( writer != NONE )
			{
				/* Someone else has written. Wait for main to
				 * read buffer2 before we can write */
				pthread_cond_wait( &bufWriteCond, &writeMut );
			}
			/* Write results to buffer if no error */
			if( fileError == PROC_NO_ERROR )
			{
				buffer2[0] = aveTurn;
				buffer2[1] = aveWait;
			}
			writer = SJF;
			fError = fileError;
			/* Unlock writeMut and tell all threads waiting on
			 * bufWriteCond to wake up */
			pthread_mutex_unlock( &writeMut );
			pthread_cond_broadcast( &bufWriteCond );

			/* Free our memory */
			listDestroyFree( procList );
		}
	} while( strcmp( "QUIT", inFileName ) != 0 );

	return NULL;
}



/****
 *  rrThread
 *  Description: A thread to simulate a round robin cpu scheduling
 *  algorithm on a list of processes. The name of the file containing the 
 *  processes should be written to global varable buffer1 and then read
 *  using mutal exclusion. The result will be written to buffer2 also 
 *  inside a critical section.
 */
void *rrThread( void *attr )
{
	/* local variable to copy the filename into */
	char inFileName[FILENAME_LEN + 1];

	/* Loop until our fileName is QUIT */
	do
	{
		/* Wait for a lock to read fNameRead */
		pthread_mutex_lock( &readMut );
		/* Need to loop to prevent spurious wakeups */
		while( fNameRead[RR] != FALSE )
		{
			/* Wait until rrReadCond has been met */
			pthread_cond_wait( &rrReadCond, &readMut );
		}
		strcpy( inFileName, buffer1 );
		/* Filename has now been read */
		fNameRead[RR] = TRUE;
		/* Let the mutex go and tell the main thread that we have
		 * finished reading */
		pthread_mutex_unlock( &readMut );
		pthread_cond_signal( &rrReadCond );

		
		if( strcmp( inFileName, "QUIT" ) != 0 )
		{
			LList procList;
			int quantum;
			int fileError;
			double aveWait, aveTurn;

			/* Read processes from file */
			procList = listCreate();
			fileError = procReadFile( inFileName, procList, &quantum );

			/* Compute rr times before entering critical section */
			if( fileError == PROC_NO_ERROR )
			{
				rrSim( procList, quantum, &aveWait, &aveTurn );
			}

			/* Obtain a lock on writeMut so we can write result to
			 * buffer2 */
			pthread_mutex_lock( &writeMut );
			while( writer != NONE )
			{
				/* Someone else has written. Wait for main to
				 * read buffer2 before we can write */
				pthread_cond_wait( &bufWriteCond, &writeMut );
			}
			/* Write results to buffer if no error */
			if( fileError == PROC_NO_ERROR ) 	
			{
				buffer2[0] = aveTurn;
				buffer2[1] = aveWait;
			}
			writer = RR;
			fError = fileError;
			/* Unlock writeMut and tell all threads waiting on
			 * bufWriteCond to wake up */
			pthread_mutex_unlock( &writeMut );
			pthread_cond_broadcast( &bufWriteCond );

			/* Free our memory */
			listDestroyFree( procList );
		}
	} while( strcmp( "QUIT", inFileName ) != 0 );

	return NULL;
}

int main( void )
{
	pthread_t sjf, rr;
	int success;

	/* Attempt to initialise our mutexes */
	if( pthread_mutex_init( &readMut, NULL ) > 0 || pthread_mutex_init( &writeMut, NULL ) > 0 )
	{
		printf( "Could not create reading mutex!\nExiting!\n" );
		success = 1;
	}
	else
	{
		/* Mutex initialisation successful. Initialise threads */
		if( pthread_create( &sjf, NULL, sjfThread, NULL ) != 0 )
		{
			printf( "Could not create shortest job first thread!\nExiting!\n" );
			success = 1;
		}
		else if( pthread_create( &rr, NULL, rrThread, NULL ) != 0 )
		{
			printf( "Could not create round robin thread!\nExiting\n" );
			success = 1;
			/* Only used here for error handling. Elsewhere thread is
			 * terminated properly */
			pthread_cancel( sjf );
		}
		else
		{
			/* All threads and mutexes created */
			do
			{
				/* Try to obtain lock on readMut so we can write buffer1*/
				pthread_mutex_lock( &readMut );
				while( fNameRead[SJF] == FALSE || fNameRead[RR] == FALSE )
				{
					/* Wait on both conditions to make sure that both
					 * threads have read the filename */
					if( fNameRead[SJF] == FALSE )
					{
						pthread_cond_wait( &sjfReadCond, &readMut );
					}
					if( fNameRead[RR] == FALSE )
					{
						pthread_cond_wait( &rrReadCond, &readMut );
					}
				}
				/* Read filename into buffer variable. Ideally the I/O wouldn't be in the
				 * critical section here but it doesn't matter because our threads can only
				 * be sleeping here anyway */
				printf( "Scheduling simulation:" );
				scanf( "%s", buffer1 );
				/* Set state variables to un-read. Release lock and signal both conditions */
				fNameRead[SJF] = fNameRead[RR] = FALSE;
				pthread_mutex_unlock( &readMut );
				pthread_cond_signal( &sjfReadCond );
				pthread_cond_signal( &rrReadCond );

				if( strcmp( "QUIT", buffer1 ) != 0 )
				{
					int answersRcvd;
					/* Make sure that we read the answers from BOTH threads */
					for( answersRcvd = 0; answersRcvd < 2; answersRcvd++ )
					{
						/* Obtain a lock on writeMut */
						pthread_mutex_lock( &writeMut );
						while( writer == NONE )
						{
							pthread_cond_wait( &bufWriteCond, &writeMut );
						}
						
						/* Print the writer so that we know which result is which */
						if( writer == SJF )
						{
							printf( "SJF: " );
						}
						else if( writer == RR )
						{
							printf( "RR: " );
						}
						
						if( fError != PROC_NO_ERROR )
						{
							printf( "Error occurred reading file!\n" );
						}
						else
						{
							printf("Average Turnaround time = %f, Average Waiting Time = %f\n", buffer2[0], buffer2[1] );
						}
						/* Unlock the mutex and signal bufWriteCond to wake up a thread */
						writer = NONE;
						pthread_mutex_unlock( &writeMut );
						pthread_cond_signal( &bufWriteCond );
					}
				}
			} while( strcmp( "QUIT", buffer1 ) != 0 );

			/* Wait for threads to complete */
			pthread_join( sjf, NULL );
			pthread_join( rr, NULL );
			success = 0;
		}
		/* Clean up our mutexes */
		pthread_mutex_destroy( &readMut );
		pthread_mutex_destroy( &writeMut );
	}

	return success;
}


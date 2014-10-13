/* rr.c
 * Simulation of a round robin scheduling
 * algorithm.
 * By Tim Peskett
 * 24/4/14
 */

#include <stdio.h>
#include <string.h>
#include "../LList.h"
#include "../common.h"
#include "../rrsim.h"

#define FILENAME_LEN 10

int main( void )
{
	/* Filenames are assumed to be no longer that 10 characters */
	char fileName[FILENAME_LEN + 1];
	int fileError;

	/* Loop until the user enters QUIT */
	do
	{
		printf( "RR simulation:" );
		scanf( "%s", fileName );

		if( strcmp( fileName, "QUIT" ) != 0 )
		{
			LList procList;
			int quantum;

			/* Attempt to read the list of processes from the file */
			procList = listCreate();
			fileError = procReadFile( fileName, procList, &quantum );

			/* Check to see if an error occurred while reading file */
			if( fileError == PROC_NO_OPEN )
			{
				printf( "Can not open file: %s\n", fileName );
			}
			else if( fileError == PROC_BAD_FORMAT )
			{
				printf( "File is not formatted properly!" );
			}
			else
			{
				double aveWait, aveTurn;

				/* Processes read successfully. Run simulation */
				rrSim( procList, quantum, &aveWait, &aveTurn );

				/* Free our list of processes */
				printf( "Average Waiting Time: %f\nAverage Turnaround Time: %f\n", aveWait, aveTurn );
			}

			listDestroyFree( procList );
		}
	} while( strcmp( fileName, "QUIT" ) != 0 );

	return 0;
}

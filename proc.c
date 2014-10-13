#include "proc.h"


int procReadFile( const char *inFileName, LList outList, int *outQuantum )
{
	FILE *inFile;
	int arrTime, bstTime;
	int procNum;
	int error = PROC_NO_ERROR;

	assert( outList != NULL );
	assert( outQuantum != NULL );

	/* Attempt to open the given gile */
	inFile = fopen( inFileName, "r" );
	if( inFile != NULL )
	{
		/* Quantum is always at the beginning of our file. Read it in */
		if( fscanf( inFile, "%d\n", outQuantum ) == 1 )
		{
			/* Loop through our file line by line and read the data
			 * for each process */
			procNum = 1;
			while( fscanf( inFile, "%d %d\n", &arrTime, &bstTime ) == 2 )
			{
				struct proc *newProc;

				/* Create/initialise process and add to our list */
				newProc = malloc( sizeof *newProc );
				newProc->pNo = procNum;
				newProc->arrivalTime = arrTime;
				newProc->burstTime = bstTime;

				listAdd( outList, newProc, listSize( outList ) + 1 );
				procNum++;
			}

			/* If fscanf has failed but we are NOT at the end of our file
			 * then an error has occurred */
			if( !feof( inFile ) ) /* Fields not formatted properly */
			{
				error = PROC_BAD_FORMAT;
			}
		}
		else /* Could not read quantum */
		{
			error = PROC_BAD_FORMAT;
		}

		fclose( inFile );
	}
	else /* inFile is NULL */
	{
		error = PROC_NO_OPEN;
	}

	return error;
}
			

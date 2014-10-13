/* proc.h
 * A structure to hold process information.
 * Also declares a function (defined in proc.c)
 * to read a table of processes from a file. The read
 * processes are added into a linked list.
 * The structure here is not typedef-ed because
 * it is not meant to act as an abstract type.
 * By Tim Peskett
 * 22/4/14
 */

#ifndef PROC_H
#define PROC_H

#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "LList.h"

/* Error codes for our procReadFile function. These assist in giving
 * more detailed error messages to the user */
#define PROC_NO_ERROR 0
#define PROC_NO_OPEN -1
#define PROC_BAD_FORMAT -2


/* A structure to hold information about a process. The data will
 * be read from the specified file and the process number will simply
 * be the (line of the file that the data occurs on) - 1.*/
struct proc
{
	int pNo;
	int arrivalTime;
	int burstTime;
};


/****
 *  procReadFile
 *  Description: Reads a list of processes from a file. An example of
 *  the file format with explanation is as follows.
 *
 *  10			<--Quantum
 *  1	4		<--1 = Arrival Time. 4 = Burst Time.
 *  3	7		<--3 = Arrival Time. 7 = Burst Time.
 *  2	8		<--2 = Arrival Time. 8 = Burst Time.
 *
 * The quantum is located on the first line of the file. It is the running time
 * 	that a process should have before it is pre-empted by the CPU and context
 * 	switched out.
 * The arrival time is the time (in ticks) that the process arrived at. The burst
 * 	time is the length of time (in ticks) that the process will run until
 * 	it either completes of blocks.
 *  Note that processes do not need to be ordered in any fashion.
 *
 *  const char *inFileName - The name of the file from which to
 *  	read the list of processes.
 *  LList outList - The list to place the processes in to. This list should be
 *  	initialised (with listCreate()) before calling procReadFile. Must be non-null.
 *  int *outQuantum - A variable where the quantum read from the file will
 *  	be placed. Must be non-null.
 *
 *  Returns an error code as follows:
 *  	PROC_NO_ERROR - No error occurred.
 *  	PROC_NO_OPEN  - Could not open file. Possibly not found or no permissions.
 *  	PROC_BAD_FORMAT - The format of the file was not as specified above.
 */
int procReadFile( const char *inFileName, LList outList, int *outQuantum );


#endif

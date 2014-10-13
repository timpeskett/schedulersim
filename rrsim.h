/* rrsim.h
 * A file that contains functions
 * to simulate a round robin scheduling
 * algorithm on a list of processes.
 * By Tim Peskett
 * 16/5/14
 */
#ifndef RRSIM_H
#define RRSIM_H

#include "Gantt.h"
#include "LList.h"
#include "proc.h"
#include "common.h"
#include "simcalc.h"


/* A small structure used to record round robin
 * housekeeeping data for a process.
 * struct proc *proc - A pointer to the process's information.
 * int runTime - The amount of time that the process has CONTINUOUSLY
 * 	been running for. This resets every time the quantum is reached.
 * int remaining - The remaining amount of time until all of the process's
 * 	burst time is exceeded.
 */
struct rrData
{
	struct proc *proc;
	int runTime;
	int remaining;
};


/****
 *  rrCmp
 *  Description: A callback function used to sort the list of
 *  	processes in an appropriate order for round robin. We
 *  	would like our initial list to be sorted by arrival time
 *  	to make it much easier to add processes to the ready queue
 *  	when it is their turn.
 *
 *  const void *arg1 - The first struct proc *.
 *  const void *arg2 - The second struct proc *.
 *
 *  Returns -1 if arg1->arrivalTime < arg2->arrivalTime, 0 if equal, 1 if
 *  	arg1->arrivalTime > arg2->arrivalTime.
 */
int rrCmp( const void *arg1, const void *arg2 );


/****
 *  rrSim
 *  Description: A function to simulate a round robin scheduling algorithm on
 *  	a list of processes.
 *
 *  LList inProcList - The list of processes to simulate round robin for.
 *  	Must be non-null.
 *  int quantum - The quantum for our round robin algorithm. The amount of time
 *  	to wait before pre-empting the currently running process and swapping it
 *  	out. Must be non-negative.
 *  double *outAveWait - A pointer to a variable to hold the average waiting time
 *  	for a round robin simulation on inProcList. Must be non-null. Will be zero
 *  	if inProcList is empty.
 *  double *outAveTurn - A pointer to a double to hold the average turnaround time
 *  	for a round robin simulation on inProcList. Must be non-null. Will be zero
 *  	if inProcList is empty.
 *  Returns nothing. outAveWait and outAveTurn will reflect the average waiting time
 *  	and turnaround time (respectively) after execution of this function.
 */
void rrSim( LList inProcList, int quantum, double *outAveWait, double *outAveTurn );

#endif

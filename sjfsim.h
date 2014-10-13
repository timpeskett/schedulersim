/* sjfsim.h
 * A file that contains functions
 * to simulate a shortest job first
 * scheduling algorithm on a list
 * of processes.
 * By Tim Peskett
 * 17/5/14
 */
#ifndef SJFSIM_H
#define SJFSIM_H

#include "Gantt.h"
#include "common.h"
#include "LList.h"
#include "proc.h"
#include "simcalc.h"


/* A small structure used to record shortest job
 * first housekeeping data for a process.
 * struct proc *proc - A pointer to the process's information.
 * int remaining - The remaining amount of time until all of the process's
 * 	burst time is exceeded.
 */
struct sjfData
{
	struct proc *proc;
	int remaining;
};


/****
 *  sjfBurstCmp
 *  Description: A callback function used to sort the list of processes.
 *  	This function sorts ONLY by burst time. It is useful for sorting
 *  	our ready queue so that the shortest process is always at the start
 *  	of the queue.
 *
 *  const void *arg1 - A struct sjfData *
 *  const void *arg2 - A struct sjfData *
 *
 *  Returns -1 if arg1->proc->burstTime < arg2->proc->burstTime, 0 if equal,
 *  	1 otherwise.
 */
int sjfBurstCmp( const void *arg1, const void *arg2 );


/****
 *  sjfCmp
 *  Description: A callback function used to sort the list of processes.
 *  	Sorts by arrival time first, and if arrival times are equal it will
 *  	sort by burst time. This is used initially to sort the list of processes
 *  	so that adding processes to the ready queue is much easier.
 *
 *  const void *arg1 - A struct proc *
 *  const void *arg2 - A struct proc *
 *
 *  Returns -1 if arg1->arrivalTime < arg2->arrivalTime, 1 if arg1->arrivalTime >
 *  	arg2->arrivalTime. If arg1->arrivalTime == arg2->arrivalTime then will perform
 *  	the comparison on burstTime instead.
 */
int sjfCmp( const void *arg1, const void *arg2 );

/****
 *  sjfSim
 *  Description: Performs a simulation of a shortest job first scheduling
 *  	algorithm on the list of processes.
 *
 *  LList inProcList - The list of processes to simulate SJF for. Must be non-null.
 *  double *outAveWait - A pointer to a double to hold the average waiting time
 *  	computed for the input. Will be zero if inProcList empty
 *  double *outAveTurn - A pointer to a double to hold the average turnaround time
 *  	computed for the input. Will be zero if inProcList empty
 *
 *  Returns nothing. After execution outAveWait and outAveTurn will contain the
 *  	average waiting time and average turnaround time (respectively) for a
 *  	sjf simulation on inProcList.
 */
void sjfSim( LList inProcList, double *outAveWait, double *outAveTurn );


#endif

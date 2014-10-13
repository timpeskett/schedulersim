/* simcalc.h
 * Contains some functions to perform calculations
 * needed for our simulation. Essentially this is the
 * "functions that aren't necessary but make the code
 * much cleaner" file. Contains functions for
 * calculating average waiting/turnaround time and a
 * comparison helper function.
 * By Tim Peskett 13/5/14
 */

#ifndef SIMCALC_H
#define SIMCALC_H


#include "common.h"
#include "LList.h"
#include "Gantt.h"
#include "proc.h"


/****
 *  cmp
 *  Description: A basic comparison function. Will return -1 if 
 *  arg1 < arg2, 0 if arg1 == arg2 and 1 if arg1 > arg2. This is used
 *  by functions that are supplied as callbacks to listSort.
 *
 *  int arg1 - First integer
 *  int arg2 - Second integer
 *
 *  Returns -1 if arg1 < arg2, 0 if arg1 == arg2, 1 if arg1 > arg2.
 */
int cmp( int arg1, int arg2 );


/****
 *  calcAveWait
 *  Description: Calculates the average waiting  time for a given gantt
 *  	chart. The list of processes is also required here just to match
 *  	the IDs of the blocks in the gantt chart.
 *
 *  Gantt gantt - The gantt chart whose average waiting time is to be
 *  	calculated. Must be non-null.
 *  LList inProcList - The process list that corresponds to the given
 *  	gantt chart. Must be non-null AND non-empty.
 *
 *  Returns a double that represents the average waiting time.
 */
double calcAveWait( Gantt gantt, LList inProcList );


/****
 *  calcAveTurn
 *  Description: Calculates the turnaround waiting  time for a given gantt
 *  	chart. The list of processes is also required here just to match
 *  	the IDs of the blocks in the gantt chart.
 *
 *  Gantt gantt - The gantt chart whose average turnaround time is to be
 *  	calculated. Must be non-null.
 *  LList inProcList - The process list that corresponds to the given
 *  	gantt chart. Must be non-null AND non-empty.
 *
 *  Returns a double that represents the average turnaround time.
 */
double calcAveTurn( Gantt gantt, LList inProcList );


#endif

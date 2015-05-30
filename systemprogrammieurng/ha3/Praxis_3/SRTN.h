/*
 * Shortest Remaining Time Next Scheduler.
 *
 * In this scheduler, the task with the shortest remaining execution time (length)
 * is scheduled immediately, displacing the currently running process.
 *
 * Tasks with the same execution time are ordered according to their arrival.
 *
 * If the arrival of new tasks happens to be at the same time (tick)
 * a running task finishes, it is guaranteed that the arrive
 * function is called before the actual tick.
 *
 * In addition, if a task is completed, it is guaranteed that the finish
 * function is called before the next tick.
 */

#ifndef _SRTN_H
#define _SRTN_H

/*
 * Initialize data structures if needed.
 *
 * returns: 1 if this scheduler should be considered in the tests, 0 otherwise.
 */
int init_SRTN();

/*
 * Frees all allocated memory used.
 */
void free_SRTN();

/*
 * Is called when a new task arrives that has to be scheduled.
 *
 * id    : the id of the task
 * length: the time the task runs
 */
void arrive_SRTN(int id, int length);

/*
 * Is called when the next tick is reached.
 */
void tick_SRTN();

/*
 * Is called when a task is finished.
 * 
 * Can be implemented, but is not necessarily needed for implementing this schedule.
 */
void finish_SRTN();

#endif /* _SRTN_H */

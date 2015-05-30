/*
 * Shortest Job Next scheduler.
 *
 * In this scheduler, the task with the shortest execution time (length)
 * currently available is scheduled when another task finishes.
 *
 * Tasks with the same execution time are ordered according to their arrival.
 *
 * This scheduler is non preemptive, meaning the tick function
 * has no effect, thus is unused.
 *
 * If the arrive of new tasks happens to be at the same time
 * a running task finishes, it is guaranteed that the arrive
 * function is called first.
 */

#ifndef SJN_H_
#define SJN_H_

/*
 * Initialize data structures if needed.
 *
 * returns: 1 if this scheduler should be considered in the tests, 0 otherwise.
 */
int init_SJN();

/*
 * Frees all allocated memory used.
 */
void free_SJN();

/*
 * Is called when a new task arrives that has to be scheduled.
 *
 * id    : the id of the task
 * length: the time the task runs
 */
void arrive_SJN(int id, int length);

/*
 * Is called when the next tick is reached.
 *
 * This function has no effect in this non preemptive SJN scheduling.
 */
void tick_SJN();

/*
 * Is called when a task is finished.
 */
void finish_SJN();

#endif /* SJN_H_ */

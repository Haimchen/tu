/*
 * LCFS scheduler.
 *
 * Schedules tasks in the inverse order of their arrival.
 *
 * This scheduler is non preemptive, meaning the tick function
 * has no effect, thus is unused.
 *
 * If the arrive of new tasks happens to be at the same time
 * a running task finishes, it is guaranteed that the arrive
 * function is called first.
 */

#ifndef LCFS_H_
#define LCFS_H_

/*
 * Initialize data structures if needed.
 *
 * returns: 1 if this scheduler should be considered in the tests, 0 otherwise.
 */
int init_LCFS();

/*
 * Frees all allocated memory used.
 */
void free_LCFS();

/*
 * Is called when a new task arrives that has to be scheduled.
 *
 * id    : the id of the task
 * length: the time the task runs
 */
void arrive_LCFS(int id, int length);

/*
 * Is called when the next tick is reached.
 *
 * This function has no effect in this non preemptive LCFS scheduling.
 */
void tick_LCFS();

/*
 * Is called when a task is finished.
 */
void finish_LCFS();

#endif /* LCFS_H_ */

/*
 * Multilevel-Feedback Scheduler.
 *
 * Depending on the time_step defined for the first
 * queue and the number of queues specified by num_queues
 * the intermediate queues 2 ... n - 1 have always a time_step 
 * as long as the time_step of the previous queue plus the 
 * time_step of the first queue. The last queue is FIFO without
 * preemption.
 *
 * For simplicity, arriving tasks are always enqueued in the first queue
 * (with the smallest time_step). Preempted tasks are always enqueued in the next
 * queue with larger time_step (and finally in the FIFO queue).
 *
 * If the arrival of new tasks happens to be at the same time (tick)
 * a running task finishes, it is guaranteed that the arrive
 * function is called before the actual tick.
 *
 * In addition, if a task is completed, it is guaranteed that the finish
 * function is called before the next tick.
 */
#ifndef MLF_H_
#define MLF_H_

/*
 * Initialize data structures if needed.
 *
 * time_step: the number of ticks a task can run before it gets preempted, if another task is ready.
 *
 * returns: 1 if this scheduler should be considered in the tests, 0 otherwise.
 */
int init_MLF(int time_step, int num_queues);

/*
 * Frees all allocated memory used.
 */
void free_MLF();

/*
 * Is called when a new task arrives that has to be scheduled.
 *
 * id    : the id of the task
 * length: the time the task runs
 */
void arrive_MLF(int id, int length);

/*
 * Is called when the next tick is reached.
 */
void tick_MLF();

/*
 * Is called when a task is finished.
 * 
 * Can be implemented, but is not necessarily needed for implementing this schedule.
 */
void finish_MLF();

#endif /* MLF_H_ */

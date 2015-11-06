#ifndef TASK_H_
#define TASK_H_

/* Task ID if no task is ready or all task are finished */
static const int IDLE = -1;

/*
 * Causes the system to switch to the task with the supplied id.
 *
 * id: the id of the task
 */
void switch_task(int id);

/*
 * The id of the currently running task.
 */
int running_task;

/*
 * This container can be used to represent a task 
 * in the different scheduling implementations. 
 */
typedef struct task_s
{
	int id;
	int length;
} def_task;

#endif /* TASK_H_ */

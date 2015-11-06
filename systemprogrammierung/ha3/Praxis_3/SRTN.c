#include "SRTN.h"
#include "task.h"
#include "PrioQueue.h"

PrioQueue* queue;
int remainingTime;

int init_SRTN()
{	remainingTime = 0;
	running_task = IDLE;
	queue = pqueue_new();
	return 1;
}

void free_SRTN()
{
	pqueue_free(queue);
}

void arrive_SRTN(int id, int length)
{
  pqueue_offer(queue, -length, id);
}

void tick_SRTN()
{
  // tick
  if (running_task != IDLE && remainingTime > 0) {
    remainingTime --;
  }
  // check if switch is necessary -> get length of next task in queue
  int queuedTask = - pqueue_peek_prio(queue);
  // Current task is finished
  if (remainingTime == 0) {
    // There is a new task in the queue
    if ( queuedTask > 0) {
      switch_task(pqueue_poll(queue));
      remainingTime = queuedTask;
      // There is no task in the queue
    } else {
      switch_task(IDLE);
    }
    // current task not finished, check if switch necessary
  } else {
    if (queuedTask > 0 && queuedTask < remainingTime) {
      // write current task in queue and switch to next
      int nextTask = pqueue_poll(queue);
      pqueue_offer(queue, -remainingTime, running_task);
      switch_task(nextTask);
      remainingTime = queuedTask;
    }
  }
}

void finish_SRTN()
{
	// TODO (optional)
}

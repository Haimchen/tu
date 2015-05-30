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
	if (running_task == -1) {
		switch_task(id);
		remainingTime = length;
	} else if (length < remainingTime) { 
		pqueue_offer(queue, -remainingTime, running_task);
		switch_task(id);
		remainingTime = length;
	} else {
		pqueue_offer(queue, -length, id);
	}

}

void tick_SRTN()
{
	if (remainingTime > 0) {
		remainingTime--;
	} else {
		remainingTime = -pqueue_peek(queue)-1;
		switch_task(pqueue_poll(queue));
	}
}

void finish_SRTN()
{
	// TODO (optional)
}

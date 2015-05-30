#include "MLF.h"
#include "task.h"
#include "PrioQueue.h"

int remainingTime;
int lastQueue; = -1;

int init_MLF(int time_step, int num_queues)
{	
	running_task = IDLE;
	PrioQueue * queues[num_queues];
	for (int i = 0; i < num_queues; i++) {
		queues[i] = pqueue_new();
	}
	return 1;
}

void free_MLF()
{
	for (int i = 0; i < num_queues; i++) {
		pqueue_free(queues[i]);
	}
}

void arrive_MLF(int id, int length)
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

void tick_MLF()
{
	if (remainingTime > 0) {
		remainingTime--;
	} else {
		remainingTime = -pqueue_peek(queue)-1;
		switch_task(pqueue_poll(queue));
	}
}

void finish_MLF()
{
	// TODO (optional)
}

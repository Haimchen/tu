#include "SJN.h"
#include "task.h"
#include "PrioQueue.h"

PrioQueue* queue;

int init_SJN()
{
	running_task = IDLE;
	queue = pqueue_new();
	return 1;
}

void free_SJN()
{
	pqueue_free(queue);
}

void arrive_SJN(int id, int length)
{
	if (running_task == -1) {
		switch_task(id);
	} else {
		pqueue_offer(queue, -length, id);
	}
}

void tick_SJN()
{
	// UNUSED
}

void finish_SJN()
{
	switch_task(pqueue_poll(queue));
}

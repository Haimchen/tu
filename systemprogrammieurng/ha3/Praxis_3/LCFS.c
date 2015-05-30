#include "LCFS.h"
#include "task.h"
#include "PrioQueue.h"

PrioQueue* queue;

int init_LCFS()
{
	running_task = IDLE;
	queue = pqueue_new();
	return 1;
}

void free_LCFS()
{
	pqueue_free(queue);
}

void arrive_LCFS(int id, int length)
{
	length = length -1;
	if (running_task == -1) {
		switch_task(id);
	} else {
		pqueue_offer(queue, pqueue_size(queue), id);
	}

}

void tick_LCFS()
{
	// UNUSED
}

void finish_LCFS()
{
  int task = pqueue_poll(queue);
  if (task > -1) {
	switch_task(task);
  } else {
    switch_task(IDLE);
  }
}

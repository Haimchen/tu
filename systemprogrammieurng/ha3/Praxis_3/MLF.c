#include "MLF.h"
#include "task.h"
#include "TaskQueue.h"
#include <stdlib.h>
#include <stdio.h>

int nextQueue;
int currentTimeslot;
TaskQueue **queues;
int numQueues;
int timestep;
def_task *current_task;

int init_MLF(int time_step, int num_queues)
{
  // initialize variables
  running_task = IDLE;
  nextQueue = -1;
  currentTimeslot = -1;
  current_task = NULL;
  numQueues = num_queues;
  timestep = time_step;
  // create all queues
  queues = (TaskQueue **) malloc((sizeof (TaskQueue*)) * num_queues);
  for (int i = 0; i < num_queues; i++) {
    queues[i] = tqueue_new();
  }
  return 1;
}

void free_MLF()
{
  for (int i = 0; i < numQueues; i++) {
    tqueue_free(queues[i]);
  }
}

void arrive_MLF(int id, int length){
  def_task* new_task = (def_task*) malloc(sizeof(def_task));
  new_task->length = length;
  new_task->id = id;
  tqueue_offer(queues[0], new_task);
}

void tick_MLF()
{
  if (current_task != NULL) {
    // reduce all time-values
    currentTimeslot --;
    current_task->length --;
    // debug output: print current task
    // printf("Task %i is running\n", current_task->id);

    if (currentTimeslot > 0 && current_task->length > 0) {
      return;
    }

    if (current_task->length <= 0) {
      // free memory
      free(current_task);
      current_task = NULL;
    } else {
      // to next queue
      tqueue_offer(queues[nextQueue], current_task);
    }
  }

  // find next Task
  for (int i = 0; i < numQueues; i++){
    def_task* task = tqueue_poll(queues[i]);
    if (task == NULL) continue;
    // found task -> switch
    current_task = task;
    switch_task(task->id);
    nextQueue = i + 1;
    // if task is from last queue, timeslot is the size of its remaining time
    if (i == (numQueues -1)) {
      currentTimeslot = current_task->length;
    } else {
      currentTimeslot = (i + 1) * timestep;
    }
    return;
  }
  // if no task was found switch to idle
  current_task = NULL;
  switch_task(IDLE);
}


void finish_MLF()
{
  // TODO (optional)
}

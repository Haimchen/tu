#include "MLF.h"
#include "task.h"
#include "TaskQueue.h"
#include <stdlib.h>

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
  // TODO? free memory for task structs?
{
  for (int i = 0; i < numQueues; i++) {
    tqueue_free(queues[i]);
  }
}

void arrive_MLF(int id, int length){
  def_task* new_task = (def_task*) malloc(sizeof(def_task));
  new_task->length = length;
  new_task->id = id;
  // TODO ? check if memory was allocated successfully
  tqueue_offer(queues[0], new_task);
}

void tick_MLF()
{
  // check if process is running
  // if yes, goto tick, if no check queues
  // check if running process is finished
  // if yes, check queues and find next process
  // assign next process, but no tick!
  //if no process in queues, idle

  if (current_task != NULL) {
    // reduce all time-values
    currentTimeslot --;
    current_task->length --;

    if (currentTimeslot <= 0) {
      if (current_task->length <= 0) {
        // free memory
        free(current_task);
      } else {
        // to next queue
        tqueue_offer(queues[nextQueue], current_task);
      }
    } else {
      return;
    }
  }
  // get next task,
  // switch task, idle if task is NULL

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

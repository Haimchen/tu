#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/syscall.h>

#include "resources.h"
#include "deadlock.h"
#include "print.h"

const char LABEL[] = "ABCD";

// returns 2 for Deadlock/unsafe or 1 for safe
// Task: 0 = check for unsafe state, 1 = check for deadlock
int banker_algorithm(Matrix matrix, char* out, int task, unsigned t, unsigned r, unsigned a) {

  int answer = -1;
  int exists = 0;
  char threadname[4];
  int resources = 0;

  // Copy state
  Vector f = g_state.f;
  Matrix matrix_copy = matrix;
  Matrix b_copy = g_state.B;

  int threads[NUM_THREADS];
  int size = NUM_THREADS;
  for(int t=FIRST_THREAD; t < NUM_THREADS; t++){
    threads[t] = t;
  }

  if (task == 0) {
    // set state
    f.resource[r] -= a;
    matrix_copy.thread[t].resource[r] -= a;
    b_copy.thread[t].resource[r] += a;
  }

  while(answer < 0){
    // iterate over all threads
    for(int i = 0; i < NUM_THREADS; i++){
      resources = 0;
      // skip deleted threads
      if(threads[i] == -1) continue;
      // compare requested resources with available resources for all resources
      for(int k=0; k < NUM_RESOURCES; k++){

        if(matrix_copy.thread[threads[i]].resource[k] <= f.resource[k]) {
          resources ++;
        }
      }
      if (resources == NUM_RESOURCES) {
        for(int j = 0; j < NUM_RESOURCES; j++){
          f.resource[j] = f.resource[j] + b_copy.thread[threads[i]].resource[j];
        }
        exists = 1;
        if (task == 0) {
          sprintf(threadname, "T%i ", threads[i] + 1);
          strcat(out, threadname);
        }
        threads[i] = -1;
        size --;
        if (size == 0) {answer = 1;}
      }
    }

    if (exists <= 0) {
      answer = 2;
      if (task == 1) {
        for (int l = 0; l < NUM_THREADS; l++) {
          if (threads[l] != -1) {
            sprintf(threadname, "T%i ", threads[l] + 1);
            strcat(out, threadname);
          }
        }
      } else if (task == 0) {
        sprintf(threadname, "[no]");
        strcat(out, threadname);
      }
    }
    exists = 0;
  }
  return answer;
}

void init_globals(){

  /* initialize resource state */
  Matrix tmp;
  tmp.thread[T1] = (Vector) {{1,1,1,2}};
  tmp.thread[T2] = (Vector) {{3,3,0,0}};
  tmp.thread[T3] = (Vector) {{3,0,0,0}};
  tmp.thread[T4] = (Vector) {{0,3,3,0}};
  g_state.G = tmp;
  g_state.v = (Vector) {{3,3,3,3}};
  g_state.f = g_state.v;
  g_state.R = g_state.G;


  /* initialize mutexes/signals */
  for(unsigned r=FIRST_RESOURCE; r<NUM_RESOURCES; r++){
    if( pthread_cond_init (&(g_state.resource_released[r]), NULL) ){
      handle_error("cond_init");
    }
  }
  if( pthread_mutex_init(&(g_state.mutex), NULL) ){
    handle_error("mutex_init");
  }
  if( pthread_mutex_init(&g_cfd_mutex, NULL) ){
    handle_error("mutex_init");
  }

  g_checkForDeadlocks = false;
# if DETECTION
  g_checkForDeadlocks = true;
# endif
}

bool isSafe(unsigned t, unsigned r, unsigned a){

  bool answer = UNDEFINED;

  char out[57];
  sprintf(out, "[%d] T%u: is \"allocate(%c, %u)\" safe?",
      gettid(), t+1, LABEL[r], a);

  /* do we have enough resources to grant request? */
  if (g_state.f.resource[r] < a) {
    answer = UNSAFE;
  } else {

# if AVOIDANCE
    // implement banker algorithm
    int result = banker_algorithm(g_state.R, out, 0, t, r, a);
    switch(result) {
      case 1:
        answer = SAFE;
        break;
      case 2:
        answer = UNSAFE;
        break;
      default:
        printf("ERROR: Could not complete banker algorithm.\n");
        break;
    }
# endif
  }

  char tmp[65];
  sprintf(tmp, "%s : %s\n", out, answer? "yes" : "no" );
  printc(tmp, t);

#ifdef DEBUG
  print_State();
#endif

  return answer;
}

bool isDeadlocked(unsigned t){

  bool answer = UNDEFINED;

  char out[57];
  sprintf(out, "[%d] T%u: Deadlock detected?", gettid(), t+1);

# if DETECTION
  // TODO: implement Deadlock detection
  int result = banker_algorithm(g_state.A, out, 1, 0, 0, 0);
  switch(result) {
    case 1:
      answer = SAFE;
      break;
    case 2:
      answer = UNSAFE;
      break;
    default:
      printf("ERROR: Could not complete banker algorithm.\n");
      break;
  }

# endif

  char tmp[60];
  sprintf(tmp, "%s : %s\n", out, answer? "no" : "yes" );
  printc(tmp, t);

#ifdef DEBUG
  print_State();
#endif

  return answer? false : true;
}

void lock_state(unsigned t){
  printd("about to lock state");
  pthread_mutex_lock(&(g_state.mutex));
  printd("state locked");
}

void unlock_state(unsigned t){
  printd("state unlocked");
  pthread_mutex_unlock(&(g_state.mutex));
}

void allocate_r(unsigned t, unsigned r, unsigned a){

  char tmp[50];
  struct timespec ts;

  lock_state(t);

  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += 2;
  int alreadyWaited = 0;

  /* wait if request wasn't granted */
  while( (isSafe(t, r, a) == UNSAFE) ){
    g_state.A.thread[t].resource[r] = a;
    sprintf(tmp, "[%d] T%u: waiting to allocate(%c, %u)\n",
        gettid(), t+1, LABEL[r], a);
    printc(tmp, t);

    /* first wait is a timed wait */
    if( !alreadyWaited )
      alreadyWaited = pthread_cond_timedwait(&(g_state.resource_released[r]),
          &(g_state.mutex), &ts);
    else
      pthread_cond_wait(&(g_state.resource_released[r]),
          &(g_state.mutex));
  }
  g_state.A.thread[t].resource[r] = 0;

  /* TODO: update resource state */
  g_state.f.resource[r] -= a;
  g_state.R.thread[t].resource[r] -= a;
  g_state.B.thread[t].resource[r] += a;


  printd("%u unit(s) of resource %c allocated", a, LABEL[r]);

  sprintf(tmp, "[%d] T%u: allocate(%c, %u)\n", gettid(), t+1,
      LABEL[r], a);
  printc(tmp, t);

#ifdef DEBUG
  print_State();
#endif

  unlock_state(t);
}

void release_r(unsigned t, unsigned r, unsigned a){

  char tmp[30];
  printd("[%d] T%u: about to release(%c, %u)\n",
      gettid(), t+1, LABEL[r], a);

  lock_state(t);

  /* TODO: update resource state */
  g_state.f.resource[r] += a;
  // g_state.R.thread[t].resource[r] = a;
  g_state.B.thread[t].resource[r] -= a;

  printd("%u unit(s) of resource %c released", a, LABEL[r]);

  pthread_cond_signal(&(g_state.resource_released[r]));

  sprintf(tmp, "[%d] T%u: release(%c, %u)\n", getpid(), t+1,
      LABEL[r], a);
  printc(tmp, t);

#ifdef DEBUG
  print_State();
#endif

  unlock_state(t);
}

void *thread_work(void *thread_number){

  long t = (long)thread_number;
  char tmp[20];

  sprintf(tmp, "[%u] T%ld: started\n", gettid(), t+1);
  printc(tmp, t);
  switch(t) {
    case T1:
      usleep(10000);
      allocate_r(t, A, 1);
      allocate_r(t, D, 2);
      usleep(10000);
      release_r(t, A, 1);
      allocate_r(t, B, 1);
      usleep(10000);
      allocate_r(t, C, 1);
      usleep(50000);
      release_r(t, B, 1);
      usleep(10000);
      release_r(t, C, 1);
      release_r(t, D, 2);
      break;
    case T2:
      usleep(10000);
      allocate_r(t, A, 2);
      allocate_r(t, B, 3);
      usleep(10000);
      allocate_r(t, A, 1);
      usleep(50000);
      release_r(t, A, 3);
      usleep(10000);
      release_r(t, B, 3);
      break;
    case T3:
      usleep(10000);
      allocate_r(t, A, 3);
      usleep(50000);
      release_r(t, A, 3);
      break;
    case T4:
      usleep(10000);
      allocate_r(t, B, 2);
      allocate_r(t, C, 3);
      usleep(10000);
      allocate_r(t, B, 1);
      usleep(50000);
      release_r(t, B, 3);
      usleep(10000);
      release_r(t, C, 3);
      break;
    case NUM_THREADS:
      /* DL-WatchDog */
      /* poll resource state to check for deadlocks */
      pthread_mutex_lock(&g_cfd_mutex);
      while( g_checkForDeadlocks ){
        pthread_mutex_unlock(&g_cfd_mutex);
        usleep(1000000);
        if( isDeadlocked(t) ){
          char tmp[35];
          sprintf(tmp, "[%d] T%ld: Deadlock detected!\n",
              gettid(), t+1);
          printc(tmp, t);
          pthread_exit((void*)EXIT_FAILURE);
        }
        pthread_mutex_lock(&g_cfd_mutex);
      }
      pthread_mutex_unlock(&g_cfd_mutex);
      break;
    default:
      printf("unexpected!");
      exit(EXIT_FAILURE);
      break;
  }

  pthread_exit(EXIT_SUCCESS);
}

int main(){

  init_globals();

  printf("Total resources are v:\n");
  print_Vector(&(g_state.v), true);
  printf("\n");
  printf("Currently available resources are f:\n");
  print_Vector(&(g_state.f), true);
  printf("\n");
  printf("Maximum needed resources are G:\n");
  print_Matrix(&(g_state.G), true);
  printf("\n");
  printf("Currently allocated resources are B:\n");
  print_Matrix(&(g_state.B), true);
  printf("\n");
  printf("Still needed resources are R:\n");
  print_Matrix(&(g_state.R), true);
  printf("\n");

  fflush(stdout);
  setbuf(stdout, NULL);

  /* spawn threads */
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_t thread[NUM_THREADS+1] = { 0 };
  for(long t=FIRST_THREAD; t<=NUM_THREADS; t++){
    if( pthread_create(&thread[t], &attr, thread_work, (void *)t) ){
      handle_error("create");
    }
  }
  pthread_attr_destroy(&attr);

  /* wait for threads */
  void *status;
  unsigned finished = 0;
  for(unsigned t=FIRST_THREAD; t<=NUM_THREADS; t++){
    if( pthread_join(thread[t], &status) ){
      handle_error("join");
    }
    if( status ){
      printf("\n[%d] T%u exited abnormally. Status: %ld\n",
          gettid(), t+1, (long)status);
    } else {
      printf("\n[%d] T%u exited normally.\n", gettid(), t+1);
    }
    if( t<NUM_THREADS ) finished++;
    /* tell DL-WatchDog to quit*/
    if(finished==NUM_THREADS){
      pthread_mutex_lock(&g_cfd_mutex);
      g_checkForDeadlocks = false;
      pthread_mutex_unlock(&g_cfd_mutex);
    }
  }

  /* Clean-up */
  if( pthread_mutex_destroy(&g_cfd_mutex) ){
    handle_error("mutex_destroy");
  }
  if( pthread_mutex_destroy(&(g_state.mutex)) ){
    handle_error("mutex_destroy");
  }
  for(unsigned r=FIRST_RESOURCE; r<NUM_RESOURCES; r++){
    if( pthread_cond_destroy(&(g_state.resource_released[r])) ){
      handle_error("cond_destroy");
    }
  }

  printf("Main thread exited normally.\nFinal resource state:\n");

  print_State();

  exit(EXIT_SUCCESS);
}

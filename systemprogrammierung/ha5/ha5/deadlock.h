#ifndef INC_DEADLOCK_H
#define INC_DEADLOCK_H

#ifdef SYS_gettid
#define gettid() (pid_t) syscall (SYS_gettid)
#else
#define gettid() getpid()
#endif

#define DETECTION 1 // Deadlock Detection
#define AVOIDANCE 1 // Deadlock Avoidance

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

/* t: thread_number, r: resource_number, a: amount */
void allocate_r(unsigned t, unsigned r, unsigned a);
void release_r(unsigned t, unsigned r, unsigned a);
bool isSafe(unsigned t, unsigned r, unsigned a);
bool isDeadlocked(unsigned t);
void *thread_work(void *thread_number);
void lock_state(unsigned t);
void unlock_state(unsigned t);
void init_globals();

/* globally accessible variables, mutexes and signals */
bool g_checkForDeadlocks;
pthread_mutex_t g_cfd_mutex;

#endif  /* INC_DEADLOCK_H */

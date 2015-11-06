#ifndef INC_RESOURCES_H
#define INC_RESOURCES_H

#include <pthread.h>

/* set of resources */
enum {FIRST_RESOURCE, A=FIRST_RESOURCE, B, C, D, NUM_RESOURCES};
/* set of threads */
enum {FIRST_THREAD, T1=FIRST_THREAD, T2, T3, T4, NUM_THREADS};
/* states for deadlock algorithms */
enum {UNSAFE, SAFE, UNDEFINED};

/* row vector */
typedef struct {
  unsigned resource[NUM_RESOURCES];
} Vector;

typedef struct {
  Vector thread[NUM_THREADS];
} Matrix;


/* Wir haben den State so veraendert, dass nun auch die aktuell angeforderten Ressourcen
 * in der Matrix A gespeichert werden. Dort stehen alle Anforderungen, auf welche
 * die Prozesse gerade warten.
 * Grund: Um Deadlocks erkennen zu koennen, muessen die angeforderten, aber noch nicht belegten
 * Ressourcen global zugaenglich sein. Der Banker-Algorithmus verwendet diese neue Matrix um zu erkennen,
 * welcher Prozess worauf wartet.
 * Es waere auch moeglich gewesen, einen neue globale Variable dafuer zu erstellen, allerdins waere dann
 * auch ein zusaetzlicher Mutex erforderlich um diese zu sichern. Deswegen erschien es uns als bessere
 * Loesung, den State zu erweitern.
 */
typedef struct {
  pthread_mutex_t mutex;
  pthread_cond_t resource_released[NUM_RESOURCES];
  Matrix G; /* Gesamtanforderung */
  Matrix B; /* Belegt - Allocation */
  Matrix R; /* Restanforderung - Need */
  Matrix A; /* Aktuelle Anforderungen */
  Vector f; /* frei - Available */
  Vector v; /* insgesamt vorhanden */
} State;

/* globally accessible variables, mutexes and signals */
extern const char LABEL[]; /* used to label resources */
State g_state; /* used for keeping track of resource state */

#endif /* INC_RESOURCES_H */

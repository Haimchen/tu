#include <stdbool.h>
#include <stdio.h>

#include "resources.h"
#include "print.h"

void print_Vector(Vector *v, bool label){
  if(label) printf("A B C D\n");
  for(unsigned i=FIRST_RESOURCE; i<NUM_RESOURCES; i++)
    printf("%u ", v->resource[i]);
  printf("\n");
}

void print_Matrix(Matrix *m, bool label){
  if(label) printf("    A B C D\n");
  for(unsigned i=FIRST_THREAD; i<NUM_THREADS; i++){
    if(label) printf("T%d: ", i+1);
    print_Vector(&(m->thread[i]), false);
  }
}

void print_State(){
  printf("B)    A B C D   R)    A B C D   f) A B C D\n");
  for(unsigned t=FIRST_THREAD; t<NUM_THREADS; t++){
    printf("   T%u ", t+1);
    for(unsigned r=FIRST_RESOURCE; r<NUM_RESOURCES; r++)
      printf("%u ", g_state.B.thread[t].resource[r]);
    printf("     T%u ", t+1);
    for(unsigned r=FIRST_RESOURCE; r<NUM_RESOURCES; r++)
      printf("%u ", g_state.R.thread[t].resource[r]);
    if(t==T1){
      printf("     ");
      for(unsigned r=FIRST_RESOURCE; r<NUM_RESOURCES; r++)
        printf("%u ", g_state.f.resource[r]);
    }
    printf("\n");
  }
}

void printc(char *str, unsigned t){
  #define KNRM  "\x1B[0m"
  #define KRED  "\x1B[31m"
  #define KGRN  "\x1B[32m"
  #define KYEL  "\x1B[33m"
  #define KBLU  "\x1B[34m"
  #define KMAG  "\x1B[35m"
  #define KCYN  "\x1B[36m"
  #define KWHT  "\x1B[37m"
  #define RESET "\033[0m"
  switch(t) {
    # ifdef COLOR
    case T1: 
      printf(KCYN "%s" RESET, str);
      break;
    case T2:
      printf(KBLU "%s" RESET, str);
      break;
    case T3:
      printf(KGRN "%s" RESET, str);
      break;
    case T4:
      printf(KMAG "%s" RESET, str);
      break;
    case NUM_THREADS:
      printf(KRED "%s" RESET, str);
      break;
    # endif
    default:
      printf("%s", str);
      break;
  } 
}

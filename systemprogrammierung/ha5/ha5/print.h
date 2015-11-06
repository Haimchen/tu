#ifndef INC_PRINT_H
#define INC_PRINT_H

#ifdef DEBUG
  #define DBGP 1
#else
  #define DBGP 0
#endif
#define printd(msg, ...) \
    do { if (DBGP) printf("[%d] T%u: (%s) " msg "\n", \
    gettid(), t+1, __func__, ##__VA_ARGS__); } while (0)

void print_Vector(Vector *v, bool label);
void print_Matrix(Matrix *m, bool label);
void print_State();
void printc(char *str, unsigned t);

#endif  /* INC_PRINT_H */

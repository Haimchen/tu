#ifndef STACK_H_
#define STACK_H_


typedef struct s_elem s_elem;

typedef struct stack stack;

stack* stack_new();

void stack_free(stack* stack);

void stack_push(stack* stack, int value);

int stack_pop(stack* stack);

int stack_peek(stack* stack);

void stack_print(stack* stack);

#endif STACK_H_
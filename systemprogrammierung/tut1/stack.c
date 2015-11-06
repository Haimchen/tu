#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

typedef struct s_elem{
	int value;
	struct s_elem* next;
} s_elem;

typedef struct stack{
	s_elem* first;
	int length;
} stack;

stack* stack_new(){
	stack* stack = (stack*) malloc(Sizeof(stack));
	if(stack==NULL){
		printf("Could not allocate memory\n");
		exit();
	}
	stack->head = NULL;
	stack->length = 0;
	return stack;
}

void stack_free(stack* stack){
	s_elem* first = pop(stack);
	if(first == NULL){
		free(stack);
		return;
	} else {
		stack_free(stack);
	}
}

void stack_free(stack* stack){
	s_elem* current = stack -> first;		// statische Variable, wird am Ende der Funktion freigegeben 
	s_elem* toDelete = NULL;
	while(current != NULL){
		toDelete = current;
		current = current->next;
		free(toDelete);
	}
	free(stack);
}

void stack_push(stack* stack, int value){
	s_elem* new = (s_elem*) malloc(Sizeof (s_elem));
	new -> value = value;

	new -> next = stack -> first;
	stack -> first = new;
	return;
}

int stack_pop(stack* stack){
	if(stack -> first == NULL) return -1;
	int value = (stack -> first) -> value;
	s_elem* second = stack -> first;
	stack -> first = stack -> first -> next;
	
	free(second);
	return value;
}

int stack_peek(stack* stack){
	if(stack -> first == NULL) return -1;
	return stack -> first -> value;
}

void stack_print(stack* stack){
	s_elem * current;
	for(current = stack -> first; current != NULL; current = current -> next){
		printf("%i ->", current -> value);
	}
	printf("NULL\n");
}

int main(int argc, char *argv[]){
	
	return 0;
}
#ifndef __STACK_H__
#define __STACK_H__

typedef struct element {
    void *data; 
    struct element *next;
} element;

void stack_create();
void stack_push(void *data);
void stack_pop();
int stack_empty();
int stack_size();
void *stack_top();

#endif

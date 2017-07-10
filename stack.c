#include <stdlib.h>

#include "stack.h"

element *top_element;
int count;

void stack_create() {
    top_element = NULL;
    count = 0;
}

void stack_push(void *data) {
    if (top_element == NULL) {
        top_element = (element*)malloc(sizeof(element));
        top_element->data = data;
        top_element->next = NULL;
    } else {
        element *temp = (element*)malloc(sizeof(element));
        temp->data = data;
        temp->next = top_element;
        top_element = temp;
    }
    count++;
}

void stack_pop() {
    if (top_element != NULL) {
        element *temp = top_element;
        temp = temp->next;
        free(top_element);
        top_element = temp; 
        count--;
    }
}

int stack_empty() {
    return count == 0 ? 1 : 0;
}

int stack_size() {
    return count;
}

void *stack_top() {
    return top_element->data;
}

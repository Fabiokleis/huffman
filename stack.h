#ifndef STACK_H
#define STACK_H
#include <stdbool.h>
#include "arvore.h"

typedef struct stack_node {
    Arvore* arv;
    struct stack_node *next;
} StackNode;

typedef struct stack {
    StackNode *top;
} Stack;

Stack* create_stack();
void stack_push(Stack* s, Arvore* arv);
bool stack_is_empty(Stack* s);
Arvore* stack_pop(Stack* s);

#endif /* STACK_H */

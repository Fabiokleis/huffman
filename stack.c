#include "stack.h"
#include <stdlib.h>

Stack* create_stack() {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->top = NULL;
    return s;
}

void stack_push(Stack* s, Arvore* arv) {
    StackNode *sn = (StackNode*)malloc(sizeof(StackNode));
    sn->arv = arv;
    sn->next = s->top;
    s->top = sn;
}

bool stack_is_empty(Stack* s) {
    return s->top == NULL;
}

Arvore* stack_pop(Stack* s) {
    if (stack_is_empty(s)) return NULL;
    StackNode* sn = s->top;
    Arvore* arv = sn->arv;
    s->top = sn->next;
    free(sn);
    return arv;
}


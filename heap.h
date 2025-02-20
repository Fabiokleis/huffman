#ifndef HEAP_H
#define HEAP_H
#include <stdbool.h>
#include <stdint.h>
#include "arvore.h"

typedef struct heap {
  uint32_t size;
  uint32_t capacity;
  Arvore** vetor;
} Heap;

Heap* criar_heap(uint32_t capacity);
void trocar(Arvore** V, uint32_t a, uint32_t b);
void imprimir(Arvore** V, uint32_t size);
void min_heapify(Arvore** V, uint32_t size, uint32_t i); 
void build_min_heap(Arvore** V, uint32_t size);
void push(Heap* heap, Arvore* arv);
Arvore* pop(Heap* heap);

#endif /* HEAP_H */

#ifndef HEAP_H
#define HEAP_H
#include <stdbool.h>
#include <stdint.h>

typedef struct arvore {
  uint32_t color;
  uint32_t freq;
  uint32_t altura;
  struct arvore *esq;
  struct arvore *dir;
} Arvore;

Arvore* constroi_arv(uint32_t color, uint32_t freq, Arvore* e, Arvore* d);
bool verifica_arv_vazia(Arvore* a);
void arv_libera(Arvore* a);

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

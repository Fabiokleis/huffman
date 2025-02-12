#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

Arvore* constroi_arv(char byte, uint32_t freq, Arvore* e, Arvore* d) {
  Arvore* no = (Arvore*)malloc(sizeof(Arvore));
  no->byte = byte;
  no->freq = freq;
  no->esq = e;
  no->dir = d;
  return no;
}

bool verifica_arv_vazia (Arvore* a) {
  return (a == NULL);
}

void arv_libera (Arvore* a) {
  if (!verifica_arv_vazia(a)) {
    arv_libera(a->esq);
    arv_libera(a->dir);
      free(a);
  }
}

Heap* criar_heap(uint32_t capacity) {
  //printf("capacity: %d\n", capacity);
  Heap* heap = (Heap*)malloc(sizeof(Heap));
  heap->vetor = (Arvore**)malloc(capacity * sizeof(Arvore*));
  heap->size = 0;
  heap->capacity = capacity;
  return heap;
}

void trocar(Arvore** V, uint32_t a, uint32_t b) {
    Arvore* aux = V[a];
    V[a] = V[b];
    V[b] = aux;
}

void min_heapify(Arvore** V, uint32_t size, uint32_t i) {
  uint32_t e = (i * 2) + 1; // esquerda 
  uint32_t d = (i + 1) * 2; // direita
  uint32_t menor = i;
    
  if (e < size && V[e]->freq < V[i]->freq) {
    menor = e;
  }
  
  if (d < size && V[d]->freq < V[menor]->freq) {
    menor = d;
  }
  
  if (menor != i) {
    trocar(V, i, menor);
    min_heapify(V, size, menor);
  }
}

void build_min_heap(Arvore** V, uint32_t size) {
  uint32_t n_ = size / 2 - 1;
  for (int i = n_; i >= 0; i--) {
    min_heapify(V, size, i);
  }
}

void push(Heap* heap, Arvore* arv) {
  //printf("byte %c freq %d\n", arv->byte, arv->freq);
  if (heap->size == heap->capacity) {
    printf("heap cheio!\n");
    return;
  }
  
  heap->vetor[heap->size] = arv;
  uint32_t i = heap->size;
  heap->size++;

  while (i > 0 && heap->vetor[(i - 1) / 2]->freq > heap->vetor[i]->freq) {
    trocar(heap->vetor, i, (i - 1) / 2);
    i = (i - 1) / 2;
  }
}

Arvore* pop(Heap* heap) {
  if (heap->size == 0) {
    printf("heap vazio!\n");
    return NULL;
  }

  Arvore* menor = heap->vetor[0];
  heap->vetor[0] = heap->vetor[heap->size - 1];
  heap->size--;

  min_heapify(heap->vetor, heap->size, 0);

  return menor;
}

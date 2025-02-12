#include "fila.h"
#include <stdlib.h>
#include <stdio.h>

Fila* criar_fila (uint32_t tamanho) {
  Fila *f = (Fila*) malloc(sizeof(Fila));
  
  f->ini = 0;
  f->fim = 0;
  f->tam = tamanho;
  f->vetor = (uint32_t*) malloc(sizeof(uint32_t) * tamanho);

  return f;
}

void destruir_fila (Fila *f) {
  free(f->vetor);
  free(f);
}

void enqueue (Fila *f, uint32_t elemento) {
  if (f->fim < f->tam) {
    f->vetor[f->fim] = elemento;
    f->fim++;
  } else {
    printf("fila cheia!\n");
  }
}

int dequeue (Fila *f) {
  if (f->ini < f->fim) {
    uint32_t atender = f->vetor[f->ini];
    (f->ini)++;
    return atender;
  } else { 
    printf("fila vazia!\n");
    exit(1);
  }
}  

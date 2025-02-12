#ifndef FILA_H
#define FILA_H
#include <stdint.h>

typedef struct fila {
   uint32_t ini;
   uint32_t fim;
   uint32_t tam;
   uint32_t *vetor;
} Fila;

Fila* criar_fila (uint32_t tamanho);
void destruir_fila (Fila *f);
void enqueue (Fila *f, uint32_t elemento);
int dequeue (Fila *f);

#endif /* FILA_H */

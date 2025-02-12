#ifndef ARVORE_H
#define ARVORE_H
#include <stdint.h>
#include <stdbool.h>
#include "fila.h"
#include "hashmap.h"

typedef struct arvore {
  char info;
  uint32_t freq;
  struct arvore *esq;
  struct arvore *dir;
} Arvore;

typedef struct huffman {
  char* bytes;
  Fila *stack;
  Hash *hash;
  Arvore *tree;
} Huffman;

Huffman* constroi_huff(char* bytes);
Arvore* constroi_arv (char c, Arvore *e, Arvore *d);
bool verifica_arv_vazia (Arvore *a);
void arv_libera (Arvore* a);

#endif /* ARVORE_H */

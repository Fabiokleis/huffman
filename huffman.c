#include "huffman.h"
#include <stdlib.h>

Arvore* constroi_arv (char c, Arvore *e, Arvore *d) {
  Arvore *no = (Arvore*)malloc(sizeof(Arvore));
  no->info = c;
  no->esq = e;
  no->dir = d;
  return no;
}

bool verifica_arv_vazia (Arvore *a) {
  return (a == NULL);
}

void arv_libera (Arvore* a) {
  if (!verifica_arv_vazia(a)) {
    arv_libera (a->esq);
    arv_libera (a->dir);
      free(a);
  }
}

Huffman* constroi_huff(char* bytes) {
  Huffman* huff = (Huffman*) malloc(sizeof(Huffman));
  huff->bytes = bytes;
  huff->stack = NULL;
  huff->hash = NULL;
  huff->tree = NULL;

  return huff;
}

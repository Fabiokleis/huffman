#include "huffman.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define EMPTY_CHAR '\0'

Hash* bytes_freqs(char *bytes) {
  uint32_t size = (uint32_t)strlen(bytes);
  Hash* hash = create_hash(size);
  for (uint32_t i = 0; i < size; ++i) {
    int idx = hash_search(hash, size, (int)bytes[i]);
    if (EMPTY == idx) {
      hash_insert(hash, size, (int)bytes[i]);
    } else {
      hash[idx].freq = hash[idx].freq + 1;
    }
  }
  return hash;
}

void freq_sum(Heap* heap) {
  while (heap->size > 1) {
    Arvore* arv1 = pop(heap);
    Arvore* arv2 = pop(heap);
    Arvore* arv = constroi_arv(EMPTY_CHAR, arv1->freq + arv2->freq, NULL, NULL);
    arv->esq = arv1;
    arv->dir = arv2;
    push(heap, arv);
  }
}

void cria_huffman_code(Arvore *raiz, char byte) {}

Huffman* constroi_huff(char* bytes) {
  Huffman* huff = (Huffman*) malloc(sizeof(Huffman));
  huff->bytes = bytes;
  Hash *freqs = bytes_freqs(bytes);
  //hash_print(freqs, (uint32_t)strlen(bytes));

  huff->heap = criar_heap(strlen(bytes));

  printf("****************push**************\n");
  for (uint32_t i = 0; i < strlen(bytes); ++i) {
    push(huff->heap, constroi_arv((char)freqs[i].key, freqs[i].freq, NULL, NULL));
  }
  
  printf("****************soma**************\n");
  freq_sum(huff->heap);
  printf("heap size: %d\n", huff->heap->size);

  cria_huffman_code(pop(huff->heap), '\0');

  return huff;
}

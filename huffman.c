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
      uint32_t n_idx = hash_insert(hash, size, (int)bytes[i]);
      hash[n_idx].type = FREQ;
      hash[n_idx].data.freq = 1;
    } else {
      hash[idx].type = FREQ;
      hash[idx].data.freq = hash[idx].data.freq + 1;      
    }
  }
  return hash;
}

void freq_sum(Heap* heap) {
  while (heap->size > 1) {
    Arvore* arv1 = pop(heap);
    if (0 == arv1->freq) continue; /* ignore empty */
    Arvore* arv2 = pop(heap);
    Arvore* arv = constroi_arv(EMPTY_CHAR, arv1->freq + arv2->freq, NULL, NULL);
    arv->esq = arv1;
    arv->dir = arv2;
    push(heap, arv);
  }
}

void cria_huffman_code(uint32_t size, Arvore* arv, char* bits, uint32_t altura, Hash* lut) {
  if (arv == NULL) return;
  if (arv->esq == NULL && arv->dir == NULL) {
    bits[altura] = '\0';
    uint32_t idx = hash_insert(lut, size, arv->byte);
    lut[idx].type = BITS;
    lut[idx].data.bits = strdup(bits);
    printf("[%s] bits %c\n", lut[idx].data.bits, (char)lut[idx].key);
    return;
  }

  bits[altura] = '0';
  cria_huffman_code(size, arv->esq, bits, altura + 1, lut);

  bits[altura] = '1';
  cria_huffman_code(size, arv->dir, bits, altura + 1, lut);
}

void cria_huffman_lut(uint32_t size, Hash* lut, Arvore *raiz) {
  char bits[raiz->freq];
  cria_huffman_code(size, raiz, bits, 0, lut);
}

void encoda_huffman(uint32_t lut_size, Hash* lut, char* out, char* bytes) {
  uint32_t size = strlen(bytes);
  uint32_t j = 0;
  
  for (uint32_t i = 0; i < size; ++i) {
    int idx = hash_search(lut, lut_size, bytes[i]);
    if (EMPTY == idx) return;

    printf("%c: %s\n", (char)lut[idx].key, lut[idx].data.bits);
    memcpy(out+j, lut[idx].data.bits, strlen(lut[idx].data.bits));
    j += strlen(lut[idx].data.bits);
  }
}

Huffman* constroi_huff(char* bytes) {
  Huffman* huff = (Huffman*) malloc(sizeof(Huffman));
  huff->bytes = bytes;
  Hash *freqs = bytes_freqs(bytes);
  //hash_print(freqs, (uint32_t)strlen(bytes));

  huff->heap = criar_heap(strlen(bytes));

  //printf("****************push**************\n");
  for (uint32_t i = 0; i < strlen(bytes); ++i) {
    push(huff->heap, constroi_arv((char)freqs[i].key, freqs[i].data.freq, NULL, NULL));
  }
  
  //printf("****************soma**************\n");
  freq_sum(huff->heap);
  //printf("heap size: %d\n", huff->heap->size);

  huff->lut = create_hash(strlen(bytes));
  Arvore* root = pop(huff->heap);
  cria_huffman_lut(strlen(bytes), huff->lut, root);
  //printf("root freq: %d\n", root->freq);

  uint32_t encodado_size = (strlen(bytes) + 1) * root->freq;
  //printf("encoded size: %d\n", encodado_size);
  huff->code = (char*)malloc(sizeof(char)*encodado_size);
  encoda_huffman(strlen(bytes), huff->lut, huff->code, huff->bytes);
  huff->code[encodado_size] = '\0';
  
  return huff;
}

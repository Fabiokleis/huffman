#include "hashmap.h"
#include <stdlib.h>
#include <stdio.h>
/*
 * linear_probing: 2 colisoes
 * quadratic_probing: 3 colisoes
 * double_hash: 4 colisoes
 */

uint32_t hash_(uint32_t chave, uint32_t M) {
  return chave % M;
}

uint32_t hash__(uint32_t chave, uint32_t M) {
  return (1 + (chave % M));
}

uint32_t linear_probing(uint32_t chave, uint32_t iter, uint32_t M) {
  return (hash_(chave, M) + iter) % M;
}

uint32_t quadratic_probing(uint32_t chave, uint32_t iter, uint32_t M) {
  return (hash_(chave, M) + (1 * iter) + (2 * iter * iter)) % M;
}

uint32_t double_hash(uint32_t chave, uint32_t iter, uint32_t M) {
  return (hash_(chave, M) + iter * hash__(chave, 7)) % M;
}

Hash* create_hash(uint32_t M) { 
  uint32_t h;
  Hash *H = (Hash *)malloc(M * sizeof(Hash));
  for (h = 0; h < M; h++) {
    H[h].key = EMPTY;
  }
  return H;
}

void free_hash(Hash *H) {
  free(H);
}

int hash_search(Hash *H, uint32_t M, int key) {
  uint32_t j, i = 0;
  do {
    //j = linear_probing (key, i, M);
    //j = quadratic_probing (key, i, M);
    j = double_hash (key, i, M);
    //pruint32_tf("Testando chave: %d\n", j);
    if (H[j].key == key) {
      return j;
    }
    i++;
  } while ((i != M) && (H[j].key != EMPTY));
  //fpruint32_tf(stderr, "item nao encontrado no vetor...\n");
  return EMPTY;
}

uint32_t hash_insert(Hash *H, uint32_t M, int key) {
  uint32_t j, i = 0;
  do {
    //j = linear_probing (key, i, M);
    //j = quadratic_probing (key, i, M);
    j = double_hash (key, i, M);
    if (H[j].key == EMPTY) {
      H[j].key = key;
      H[j].freq = 1;
      return j;
    }
    i++;
  } while (i != M);
   
  //fprintf(stderr, "nao foi possivel inserir o item no vetor!\n");
  return EMPTY;
}

void hash_print(Hash *H, uint32_t M) {
  uint32_t i;
  for (i = 0; i < M; i++) {
    if (H[i].key != EMPTY) {
      printf("%2d - %d: %d\n", i, H[i].key, H[i].freq);
    }
    else {
      printf("%2d -  \n", i);
    } 
  }
}

#include "hashmap.h"
#include <stdlib.h>

/*
 * linear_probing: 2 colisoes
 * quadratic_probing: 3 colisoes
 * double_hash: 4 colisoes
*/

int hash_(int chave, int M) {
    return chave % M;
}

int hash__(int chave, int M) {
    return (1 + (chave % M));
}

int linear_probing (int chave, int iter, int M) {
    return (hash_(chave, M) + iter) % M;
}

int quadratic_probing (int chave, int iter, int M) {
    return (hash_(chave, M) + (1 * iter) + (2 * iter * iter)) % M;
}

int double_hash (int chave, int iter, int M) {
    return (hash_(chave, M) + iter * hash__(chave, 7)) % M;
}

Hash* create_hash (int M) { 
   int h;
   Hash *H = (Hash *)malloc(M * sizeof(Hash));
   for (h = 0; h < M; h++) {
      H[h].key = EMPTY;
   }
   return H;
}

void free_hash (Hash *H) {
   free(H);
}

int hash_search (Hash *H, int M, int key) {
   int j, i = 0;
   do {
      //j = linear_probing (key, i, M);
      //j = quadratic_probing (key, i, M);
      j = double_hash (key, i, M);
      //printf("Testando chave: %d\n", j);
      if (H[j].key == key) {
         return j;
      }
      i++;
   } while ((i != M) && (H[j].key != EMPTY));
   //fprintf(stderr, "item nao encontrado no vetor...\n");
   return EMPTY;
}

/* */
int hash_insert (Hash *H, int M, int key, int *colisoes) {
   int j, i = 0;
   do {
      //j = linear_probing (key, i, M);
      //j = quadratic_probing (key, i, M);
      j = double_hash (key, i, M);
      if (H[j].key == EMPTY) {
         H[j].key = key;
         return j;
      }
      else {
         *colisoes = *colisoes + 1;
      }
      i++;
   } while (i != M);
   
   //fprintf(stderr, "nao foi possivel inserir o item no vetor!\n");
   return EMPTY;
}

/*
void hash_print (Hash *H, int M) {
  int i;
  for (i = 0; i < M; i++) {
    if (H[i].key != EMPTY) {
      printf("%2d - %d\n", i, H[i].key);
    }
    else {
      printf("%2d -   \n", i);
    } 
  }
}
*/

#ifndef HASHMAP_H
#define HASHMAP_H
#include <stdint.h>
#define EMPTY -999999

typedef enum {
  FREQ,
  BITS,
  COLOR
} HashDataType;

typedef struct hash {
  int key;
  HashDataType type;
  union {
    uint32_t freq;
    char* bits;
    uint32_t color;
  } data;
  uint32_t freq;
} Hash;

Hash* create_hash(uint32_t M);
int hash_search(Hash *H, uint32_t M, uint32_t key);
uint32_t hash_insert(Hash *H, uint32_t M, uint32_t key);
void hash_print(Hash *H, uint32_t M);
void free_hash(Hash *H);
uint32_t hash_(uint32_t chave, uint32_t M);
uint32_t hash__(uint32_t chave, uint32_t M);
uint32_t linear_probing(uint32_t chave, uint32_t iter, uint32_t M);
uint32_t quadratic_probing(uint32_t chave, uint32_t iter, uint32_t M);
uint32_t double_hash(uint32_t chave, uint32_t iter, uint32_t M);

#endif /* HASHMAP_H */

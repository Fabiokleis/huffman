#ifndef HASHMAP_H
#define HASHMAP_H
#define EMPTY -99999

typedef struct hash {
   int key;
} Hash;

Hash* create_hash(int M);
int hash_search(Hash *H, int M, int key);
int hash_insert(Hash *H, int M, int key, int *colisoes);
void hash_print(Hash *H, int M);
void free_hash(Hash *H);
int hash_(int chave, int M);
int hash__(int chave, int M);
int linear_probing(int chave, int iter, int M);
int quadratic_probing(int chave, int iter, int M);
int double_hash(int chave, int iter, int M);

#endif /* HASHMAP_H */

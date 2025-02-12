#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdint.h>
#include "heap.h"
#include "hashmap.h"

typedef struct huffman {
  char* bytes;
  Heap* heap;
  Hash* hash;
} Huffman;

Huffman* constroi_huff(char* bytes);

#endif /* HUFFMAN_H */

#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdint.h>
#include "heap.h"
#include "hashmap.h"

typedef struct huffman {
  char* bytes;
  char* code;
  uint32_t bits_count;
  uint32_t bytes_count;
  Heap* heap;
  Hash* lut;
} Huffman;

Huffman* constroi_huff(char* bytes);

#endif /* HUFFMAN_H */

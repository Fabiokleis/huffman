#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdio.h>
#include <stdint.h>
#include "heap.h"
#include "hashmap.h"

typedef struct huffman {
  char* bytes;
  char* code;
  uint32_t bits_count;
  uint32_t bytes_count;
  uint32_t h4k_size;
  uint32_t lut_size;
  Heap* heap;
  Hash* lut;
  Arvore* root;
} Huffman;

Huffman* constroi_huff(char* bytes);
void write_huff_bytes(FILE* f, Huffman* huff);
void read_huff_bytes(uint32_t if_size, FILE* i_file, FILE* o_file, Huffman* huff);

#endif /* HUFFMAN_H */

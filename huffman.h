#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdio.h>
#include <stdint.h>
#include "heap.h"
#include "hashmap.h"

typedef struct huffman {
  uint8_t**** img;
  uint32_t height;
  uint32_t width;
  char* code; // codigo das cores ja escrito
  uint32_t bits_count;
  uint32_t h4k_size;
  Heap* heap;
  Hash* lut;
  uint32_t total_colors;
  uint32_t total_nodes;
  uint32_t tree_size; // tamanho em bytes da arvore codificada
  Arvore* root;
} Huffman;


Huffman* constroi_huff(uint8_t**** img, uint32_t height, uint32_t width);
uint8_t write_huff_bytes(FILE* f, uint8_t offset, Huffman* huff);
uint8_t write_huff_tree(Huffman* huff, uint8_t offset, FILE* file);
void free_huffman(Huffman* huff);

#endif /* HUFFMAN_H */

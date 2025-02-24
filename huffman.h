#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdio.h>
#include <stdint.h>
#include "heap.h"
#include "hashmap.h"


/*
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
*/

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

/* Huffman* constroi_huff(uint8_t**** img, uint32_t height, uint32_t width, uint32_t channels); */
Huffman* constroi_huff(uint8_t**** img, uint32_t height, uint32_t width); // integrar
uint8_t write_huff_bytes(FILE* f, uint8_t offset, Huffman* huff); // integrar
void read_huff_bytes(uint32_t if_size, FILE* i_file, FILE* o_file, Huffman* huff); // nao integrar
uint8_t write_huff_tree(Huffman* huff, uint8_t offset, FILE* file); // integrar

#endif /* HUFFMAN_H */

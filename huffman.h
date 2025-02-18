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
  char* code;
  uint32_t bits_count;
  uint32_t h4k_size;
  Heap* heap;
  Hash* lut;
  Hash* rev_lut;
  Arvore* root;
} Huffman;

typedef struct pixel {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} Pixel;

/* Huffman* constroi_huff(uint8_t**** img, uint32_t height, uint32_t width, uint32_t channels); */
Huffman* constroi_huff(uint8_t**** img, uint32_t height, uint32_t width);
void write_huff_bytes(FILE* f, Huffman* huff);
void read_huff_bytes(uint32_t if_size, FILE* i_file, FILE* o_file, Huffman* huff);
uint32_t pack_color(uint8_t r, uint8_t g, uint8_t b);
Pixel unpack_color(uint32_t color_pack);
#endif /* HUFFMAN_H */

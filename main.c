#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "huffman.h"
#include "kmeans.h"

#define CODE_FILE "code.h4k\0"
#define TREE_FILE "tree.bin\0"
#define HEIGHT 4
#define WIDTH 4
#define COLORS 20

int main(int argc, char** argv) {
  (void) argc;
  (void) argv;
  
  //char* texto = "Então, foram chamados os escrivães do rei, naquele mesmo tempo e no mês terceiro (que é o mês de sivã), aos vinte e três do mesmo, e se escreveu conforme tudo quanto ordenou Mardoqueu aos judeus, como também aos sátrapas, e aos governadores, e aos maiorais das províncias que se estendem da Índia até à Etiópia, cento e vinte e sete províncias, a cada província segundo a sua escritura e a cada povo conforme a sua língua; como também aos judeus segundo a sua escritura e conforme a sua língua";
  //char* texto = "hello";
  //printf("texto: %s\n", texto);

  
  float*** img = (float***)malloc(HEIGHT * sizeof(float**));
  for (uint32_t i = 0; i < HEIGHT; ++i) {
    img[i] = (float**)malloc(WIDTH * sizeof(float*));

    for (uint32_t j = 0; j < WIDTH; ++j) {
      img[i][j] = (float*)malloc(3 * sizeof(float));
    }
  }

  float colors[3][COLORS] = {
    {255, 6, 50, 100, 180, 240, 10, 45, 99, 182, 250, 9, 53, 102, 178, 243, 12, 39, 95, 185},
    {129, 58, 98, 77, 159, 242, 13, 38, 66, 80, 25, 33, 9, 0, 245, 111, 29, 32, 95, 208},
    {1, 86, 40, 109, 123, 149, 22, 18, 5, 90, 15, 130, 244, 140, 217, 223, 10, 38, 79, 2}
  };
  
  printf("img float %dx%dx%d\n", HEIGHT, WIDTH, 3);
  for (uint32_t y = 0; y < HEIGHT; ++y) {
    for (uint32_t x = 0; x < WIDTH; ++x) {
      img[y][x][0] = colors[rand() % 3][rand() % (COLORS-1)] / 255;
      img[y][x][1] = colors[rand() % 3][rand() % (COLORS-1)] / 255;
      img[y][x][2] = colors[rand() % 3][rand() % (COLORS-1)] / 255;
      printf("(%f, %f, %f)\n", img[y][x][0], img[y][x][1], img[y][x][2]);
    }
  }
  
  uint8_t*** out = (uint8_t***)malloc(HEIGHT * sizeof(uint8_t**));
  for (uint32_t i = 0; i < HEIGHT; ++i) {
    out[i] = (uint8_t**)malloc(WIDTH * sizeof(uint8_t*));

    for (uint32_t j = 0; j < WIDTH; ++j) {
      out[i][j] = (uint8_t*)malloc(3 * sizeof(uint8_t));
    }
  }

  k_means(img, out, HEIGHT, WIDTH, 4, 10);

  printf("----------------------------------\n");
  printf("img uint8_t %dx%dx%d\n", HEIGHT, WIDTH, 3);
  for (uint32_t y = 0; y < HEIGHT; ++y) {
    for (uint32_t x = 0; x < WIDTH; ++x) {
      printf("(%3d, %3d, %3d)\n", out[y][x][0], out[y][x][1], out[y][x][2]);
    }
  }
  Huffman* huff = constroi_huff(&out, HEIGHT, WIDTH);

  printf("huffman code %s\n", huff->code);

  FILE* f = fopen(CODE_FILE, "w+b");
  if (NULL == f) {
   fprintf(stderr, "ERROR: failed to open %s: %s\n", CODE_FILE, strerror(errno));
   return 1;
  }

  write_huff_bytes(f, huff);

  FILE* bin_arv = fopen(TREE_FILE, "wb");
  if (NULL == bin_arv) {
    fprintf(stderr, "ERROR: failed to open %s: %s\n", TREE_FILE, strerror(errno));
    return 1;
  }
  write_huff_tree(huff, bin_arv);

  //fseek(f, 0, SEEK_SET);
  //read_huff_bytes(huff->h4k_size, f, NULL, huff);

  return 0;
}

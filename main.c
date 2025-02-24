#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "huffman.h"
#include "kmeans.h"
#include "professor_lib.h"
#include"descompactador.h"

#define CODE_FILE "code.h4k\0"
#define TREE_FILE "tree.bin\0"
#define HEIGHT 4
#define WIDTH 4
#define COLORS 20

void print_grids(float ***image, int height, int width, int channels) {
    int grid_size = 4;
    for (int y = 0; y < height; y += grid_size) {
        for (int x = 0; x < width; x += grid_size) {
            printf("Grid at (%d, %d):\n", y, x);
            
            for (int gy = 0; gy < grid_size; gy++) {
                for (int gx = 0; gx < grid_size; gx++) {
                    int yy = y + gy;
                    int xx = x + gx;
                    if (yy < height && xx < width) {
                        printf("(");
                        for (int c = 0; c < channels; c++) {
                            printf("%.2f%s", image[yy][xx][c], (c == channels - 1) ? "" : ", ");
                        }
                        printf(") ");
                    } else {
                        printf("(N/A) \n ");
                    }
                }
                printf("\n");
            }
            printf("\n");
        }
    }
}

void process_grids(float ***image, int height, int width, int channels, FILE *f, FILE *bin_arv) {
    int grid_size = 4;
    uint8_t offset = 0;
    uint32_t offset_write = 0;

    for (int y = 0; y < height; y += grid_size) {
        for (int x = 0; x < width; x += grid_size) {
            printf("Grid at (%d, %d):\n", y, x);

            // Cria um array para armazenar os valores do grid
            float grid[grid_size][grid_size][channels];
            for (int gy = 0; gy < grid_size; gy++) {
                for (int gx = 0; gx < grid_size; gx++) {
                    int yy = y + gy;
                    int xx = x + gx;
                    if (yy < height && xx < width) {
                        for (int c = 0; c < channels; c++) {
                            grid[gy][gx][c] = image[yy][xx][c];
                        }
                    } else {
                        // Preenche com N/A se estiver fora dos limites da imagem
                        for (int c = 0; c < channels; c++) {
                            grid[gy][gx][c] = -1.0f; // Valor inválido para indicar N/A
                        }
                    }
                }
            }

            // Gera a árvore de Huffman para o grid atual
            Huffman* huff = constroi_huff(grid, grid_size, grid_size);
            printf("Huffman code: %s\n", huff->code);

            // Escreve os bytes codificados no arquivo de saída
            offset = write_huff_bytes(f, offset, huff);
            printf("bits offset: %d\n", offset);

            // Escreve a árvore de Huffman no arquivo binário
            offset_write = write_huff_tree(huff, offset_write, bin_arv);
            printf("offset_write: %d\n", offset_write);

            // Libera a memória alocada para a árvore de Huffman
            free_huffman(huff);
        }
    }
}


int main(int argc, char** argv) {
  (void) argc;
  (void) argv;
  
  Imagem * img_strc = get_imagem(3);
  printf("Abriu a imagem \n");
  int altura = img_strc->altura;
  int largura = img_strc->largura;
  float*** teste = img_strc->dados;
  printf("altura: %d\n", altura);
  printf("largura: %d\n", largura);

  
  float*** img = (float***)malloc(altura * sizeof(float**));
  for (uint32_t i = 0; i < largura; ++i) {
    img[i] = (float**)malloc(largura * sizeof(float*));

    for (uint32_t j = 0; j < largura; ++j) {
      img[i][j] = (float*)malloc(3 * sizeof(float));
    }
  }

  for(int y = 0; y < altura; y++) {
    for(int x = 0; x < largura; x++) {
      img[y][x][0] = teste[y][x][0];
      img[y][x][1] = teste[y][x][1];
      img[y][x][2] = teste[y][x][2];
    }
  }
  print_grids(img, altura, largura, 3);
  
  uint8_t*** out = (uint8_t***)malloc(HEIGHT * sizeof(uint8_t**));
  for (uint32_t i = 0; i < HEIGHT; ++i) {
    out[i] = (uint8_t**)malloc(WIDTH * sizeof(uint8_t*));

    for (uint32_t j = 0; j < WIDTH; ++j) {
      out[i][j] = (uint8_t*)malloc(3 * sizeof(uint8_t));
    }
  }

  k_means(img, out, HEIGHT, WIDTH, 5, 50);

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


  /* # integrar
    pegar o Huffman* de retorno da funcao constroi_huff e
    passar para funcao write_huff_bytes junto com o arquivo de saida.

    passar a cada loop o offset retornado de cada write_huff_bytes
  */
  uint8_t offset = write_huff_bytes(f, 0, huff);
  printf("bits offset %d\n", offset);


  Huffman* huff2 = constroi_huff(&out, HEIGHT, WIDTH);
  printf("huffman2 code %s\n", huff2->code);
  uint8_t offset2 = write_huff_bytes(f, offset, huff);
  printf("bits offset2 %d\n", offset2);
  

  FILE* bin_arv = fopen(TREE_FILE, "w+b");
  if (NULL == bin_arv) {
    fprintf(stderr, "ERROR: failed to open %s: %s\n", TREE_FILE, strerror(errno));
    return 1;
  }

  /* # integrar
    passar o struct Huffman* como argumento para funcao
    write_huff_tree que vai salvar a arvore codificada
    no arquivo de saida.

    passar a cada loop o offset retornado de cada write_huff_tree
  */
  uint32_t offset_write = write_huff_tree(huff, 0, bin_arv);
  printf("offset_write1 %d\n", offset_write);
  uint32_t offset_write2 = write_huff_tree(huff, offset_write, bin_arv);
  printf("offset_write2 %d\n", offset_write2);
  return 0;
}
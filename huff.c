#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "huffman.h"
#include "kmeans.h"
#include "pdi.h"
#include"descompactador.h"

#define CODE_FILE "code.h4k\0"
#define TREE_FILE "tree.bin\0"

void liberar_matriz(float ***teste, int altura, int largura) {
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            free(teste[i][j]); // Libera cada vetor de floats (canais)
        }
        free(teste[i]); // Libera cada linha da matriz
    }
    free(teste); // Libera a matriz principal
}
void liberar_matriz_u_int(uint8_t ***teste, int altura, int largura) {
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            free(teste[i][j]); // Libera cada vetor de floats (canais)
        }
        free(teste[i]); // Libera cada linha da matriz
    }
    free(teste); // Libera a matriz principal
}

void print_grids(float ***image, int height, int width, int channels) {
    int grid_size = 8;
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

Imagem* get_imagem(char* input, int canais) {
    Imagem* img = abreImagem(input, canais);
    if (!img) {
        printf("Erro abrindo a imagem.\n");
        exit(1);
    }
    float*** new_img = malloc(img->altura * sizeof(float**));
    for (int y = 0; y < img->altura; y++) {
        new_img[y] = malloc(img->largura * sizeof(float*));
        for (int x = 0; x < img->largura; x++) {
            new_img[y][x] = malloc(img->n_canais * sizeof(float));
        }
    }
    for (int y = 0; y < img->altura; y++) {
        for (int x = 0; x < img->largura; x++) {
            for (int c = 0; c < img->n_canais; c++) {
                new_img[y][x][c] = img->dados[c][y][x];
            }
        }
    }
    for (int c = 0; c < img->n_canais; c++) {
        for (int y = 0; y < img->altura; y++) {
            free(img->dados[c][y]);
        }
        free(img->dados[c]);
    }
    free(img->dados);
    img->dados = new_img;

    return img;
}

void process_grids(float ***img, int height, int width, int channels, FILE *f, FILE *bin_arv) {
    int grid_size = 8; // Tamanho do grid
    uint8_t offset = 0;
    uint8_t offset_write = 0;

    for (int y = 0; y < height; y += grid_size) {
        for (int x = 0; x < width; x += grid_size) {
	  //printf("Grid at (%d, %d):\n", y, x);

            float*** grid = (float***)malloc(grid_size * sizeof(float**));
            for(int i = 0; i < grid_size; i++) {
                grid[i] = (float**)malloc(grid_size * sizeof(float*));
                for(int j = 0; j < grid_size; j++) {
                    grid[i][j] = (float*)malloc(channels * sizeof(float));
                }
            }
            uint8_t*** out = (uint8_t***)malloc(grid_size * sizeof(uint8_t**));
            for(int i = 0; i < grid_size; i++) {
                out[i] = (uint8_t**)malloc(grid_size * sizeof(uint8_t*));
                for(int j = 0; j < grid_size; j++) {
                    out[i][j] = (uint8_t*)malloc(channels * sizeof(uint8_t));
                }
            }
            // Preenche o grid com os valores da imagem
            for (int gy = 0; gy < grid_size; gy++) {
                for (int gx = 0; gx < grid_size; gx++) {
                    int yy = y + gy;
                    int xx = x + gx;
                    if (yy < height && xx < width) {
                        for (int c = 0; c < channels; c++) {
                            grid[gy][gx][c] = img[yy][xx][c];
                        }
                    } else {
                        // Preenche com N/A se estiver fora dos limites da imagem
                        for (int c = 0; c < channels; c++) {
                            grid[gy][gx][c] = -1.0f; // Valor inválido para indicar N/A
                        }
                    }
                }
            }
            
            // Aplica o k-means no grid atual
            k_means(grid, out, grid_size, grid_size, 5, 50); // 5 clusters, 50 iterações
            //printf("passando pelo out:\n");
           /* for (int gy = 0; gy < grid_size; gy++) {
                for (int gx = 0; gx < grid_size; gx++) {
                    printf("(");
                    for (int c = 0; c < channels; c++) {
                        printf("%d%s", out[gy][gx][c], (c == channels - 1) ? "" : ", ");
                    }
                    printf(") ");
                }
                printf("\n");
            }*/

            // Gera a árvore de Huffman para o grid processado
            Huffman* huff = constroi_huff(&out, grid_size, grid_size);
            //printf("Huffman code: %s\n", huff->code);

            // Escreve os bytes codificados no arquivo de saída
            offset = write_huff_bytes(f, offset, huff);
            //printf("bits offset: %d\n", offset);

            // Escreve a árvore de Huffman no arquivo binário
            offset_write = write_huff_tree(huff, offset_write, bin_arv);
            //printf("offset_write: %d\n", offset_write);
            liberar_matriz_u_int(out, grid_size, grid_size);
	    liberar_matriz(grid, grid_size, grid_size);

            // Libera a memória alocada para a árvore de Huffman
	    
	    if (huff->root != NULL)
	      arv_libera(huff->root);

	    
	    if (huff->heap != NULL && huff->heap->vetor != NULL) {
	      free(huff->heap->vetor);
	      free(huff->heap);
	    }

	    if (huff->code != NULL) {
	      free(huff->code);
	    }

            free(huff);
        }
    }
}


int main(int argc, char** argv) {
  (void) argc;
  (void) argv;
  
  Imagem * img_strc = get_imagem("bmp_24.bmp", 3);
  printf("Abriu a imagem \n");
  int altura = img_strc->altura;
  int largura = img_strc->largura;
  float*** img = img_strc->dados;
  printf("altura: %d\n", altura);
  printf("largura: %d\n", largura);

  FILE* f = fopen(CODE_FILE, "w+b");
  if (NULL == f) {
   fprintf(stderr, "ERROR: failed to open %s: %s\n", CODE_FILE, strerror(errno));
   return 1;
  }
  FILE* bin_arv = fopen(TREE_FILE, "w+b");
  if (NULL == bin_arv) {
    fprintf(stderr, "ERROR: failed to open %s: %s\n", TREE_FILE, strerror(errno));
    return 1;
  }
  process_grids(img, altura, largura, 3, f, bin_arv);
  printf("escrito arquivo %s e %s\n", CODE_FILE, TREE_FILE);

  fclose(f);
  fclose(bin_arv);

  return 0;
}

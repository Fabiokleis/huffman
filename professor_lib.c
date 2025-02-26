/*============================================================================*/
/* Exemplo: segmenta��o de uma imagem em escala de cinza.                     */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu                                                     */
/* Universidade Tecnol�gica Federal do Paran�                                 */
/*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pdi.h"

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

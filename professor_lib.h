/*============================================================================*/
/* Cabeçalho global para segmentação de imagem em escala de cinza.          */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu                                                     */
/* Universidade Tecnológica Federal do Paraná                                 */
/*============================================================================*/

#ifndef __PROFESSOR_LIB_H
#define __PROFESSOR_LIB_H

#include "pdi.h"

/*============================================================================*/

#define INPUT_IMAGE "arroz.bmp"

// TODO: ajuste estes parâmetros!
#define NEGATIVO 0
#define THRESHOLD 0.4f
#define ALTURA_MIN 1
#define LARGURA_MIN 1
#define N_PIXELS_MIN 1

/*============================================================================*/

typedef struct
{
    float label; // Rótulo do componente.
    Retangulo roi; // Coordenadas do retângulo envolvente do componente.
    int n_pixels; // Número de pixels do componente.
} Componente;

/*============================================================================*/

void binariza(Imagem* in, Imagem* out, float threshold);
int rotula(Imagem* img, Componente** componentes, int largura_min, int altura_min, int n_pixels_min);
int teste_img();
Imagem* get_imagem(int canais);

#endif // SEGMENTACAO_IMAGEM_H
/*============================================================================*/

/*============================================================================*/
/* MANIPULA��O DE CORES                                                       */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu - nassu@dainf.ct.utfpr.edu.br                       */
/*============================================================================*/
/** Tipos e fun��es para manipula��o de cores. */
/*============================================================================*/

#ifndef __CORES_H
#define __CORES_H

/*============================================================================*/

#include "imagem.h"

/*============================================================================*/
/* Uma cor. */

typedef struct
{
    float canais [3]; // Valores para os 3 canais (R,G,B). Em imagens em escala de cinza, usa apenas o primeiro valor.
} Cor;

Cor criaCor (float r, float g, float b);

/*============================================================================*/
/* Convers�es de cores. */

void RGBParaCinza (Imagem* in, Imagem* out);
void cinzaParaRGB (Imagem* in, Imagem* out);

/*============================================================================*/
/* Transforma��es de cores. */

void inverte (Imagem* in, Imagem* out);

/*============================================================================*/
#endif /* __CORES_H */
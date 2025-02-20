#include "arvore.h"
#include <stdlib.h>
#include <stdio.h>

Arvore* constroi_arv(uint32_t color, uint32_t freq, Arvore* e, Arvore* d) {
  Arvore* no = (Arvore*)malloc(sizeof(Arvore));
  no->color = color;
  no->freq = freq;
  no->esq = e;
  no->dir = d;
  return no;
}

bool verifica_arv_vazia (Arvore* a) {
  return (a == NULL);
}

uint32_t pack_color(uint8_t r, uint8_t g, uint8_t b) {
  return (r << 16) | (g << 8) | b;
}

Pixel unpack_color(uint32_t color_pack) {

  Pixel pixel = {0};

  pixel.r = (color_pack >> 16) & 0xFF;
  pixel.g = (color_pack >> 8);
  pixel.b = color_pack;

  return pixel;
}

uint32_t conta_nodes(Arvore* arv, uint32_t altura) {
  if (arv != NULL) {
    if (arv->esq == NULL && arv->dir == NULL) printf("arv color %b\n", arv->color);
    arv->altura = altura;
    return conta_nodes(arv->esq, altura + 1) + conta_nodes(arv->dir, altura + 1) + 1;
  } else return 0;
}

void pre_imprime_arv (Arvore *arv) {
  if (arv != NULL) {
    Pixel pixel = unpack_color(arv->color);
    printf("altura %d color (%b, %b, %b)\n", arv->altura, pixel.r, pixel.g, pixel.b);
    pre_imprime_arv(arv->esq);
    pre_imprime_arv(arv->dir);
  }
}

void arv_libera (Arvore* a) {
  if (!verifica_arv_vazia(a)) {
    arv_libera(a->esq);
    arv_libera(a->dir);
      free(a);
  }
}

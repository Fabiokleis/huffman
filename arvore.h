#ifndef ARVORE_H
#define ARVORE_H
#include <stdbool.h>
#include <stdint.h>

typedef struct arvore {
  uint32_t color;
  uint32_t freq;
  uint32_t altura;
  struct arvore *esq;
  struct arvore *dir;
} Arvore;

typedef struct pixel {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} Pixel;

Arvore* constroi_arv(uint32_t color, uint32_t freq, Arvore* e, Arvore* d);
uint32_t conta_nodes(Arvore* arv, uint32_t altura);
void pre_imprime_arv (Arvore *arv);
bool verifica_arv_vazia(Arvore* a);
void arv_libera(Arvore* a);
uint32_t pack_color(uint8_t r, uint8_t g, uint8_t b);
Pixel unpack_color(uint32_t color_pack);

#endif /* ARVORE_H */

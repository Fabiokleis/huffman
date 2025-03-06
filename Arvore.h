#ifndef ARVORE_H
#define ARVORE_H

#include <stdint.h>
#include "leitor_bit.h"

//No de uma Arvore que tem uma cor RGB denttro de suas folhas
typedef struct Arvore{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    struct Arvore* esq;
    struct Arvore* dir;
    int folha;
} Arvore;

//inicialliza
Arvore* aloca_arvore();

void desaloca_arvore(Arvore* raiz);

//le 8 bits e coloca em uma arvore
void le_cor(Arvore* atual, Leitor_de_bytes *byte_lido);

//mostra uma arvore a partir da raiz
void mostra_arvore(Arvore* arv, int n);

#endif

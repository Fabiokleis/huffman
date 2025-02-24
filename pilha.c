#ifndef PILHA_C
#define PILHA_C

#include <stdlib.h>
#include "Arvore.h"
#include "pilha.h"

Pilha* inicializa_pilha(int tam)
{
    Pilha* pil = (Pilha*) malloc(sizeof(Pilha));
    pil -> vetor_Arvores = (Arvore**) malloc(sizeof(Arvore*) * tam);
    pil -> quantidade = 0;
    return pil;
}

void coloca_na_pilha(Pilha* pil, Arvore* arv)
{
        pil -> vetor_Arvores[pil -> quantidade] = arv;
        pil -> quantidade++;
}

Arvore* retira_da_pilha(Pilha* pil)
{
    if (pil -> quantidade == 0)
        return NULL;
    pil -> quantidade--;
    return pil -> vetor_Arvores[pil -> quantidade];
}

#endif
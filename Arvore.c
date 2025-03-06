#ifndef ARVORE_C
#define ARVORE_C

#include <stdio.h>
#include <stdlib.h>
#include "Arvore.h"

Arvore* aloca_arvore()
{
    Arvore* arv = (Arvore*) malloc(sizeof(Arvore));
    arv -> esq = NULL;
    arv -> dir = NULL;
    arv -> folha = 0;
    return arv;
}

void desaloca_arvore(Arvore* raiz)
{
    if (raiz -> esq)
        desaloca_arvore(raiz -> esq);
    if (raiz -> dir)
        desaloca_arvore(raiz -> dir);
    free(raiz);
}

//le 24 bits como um valor RGB e coloca em atual
void le_cor(Arvore* atual, Leitor_de_bytes *byte_lido)
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    // OR bit a bit para compor os 8 bits corretamente
    for(int i = 0 ; i < 8 ; i++)
    {
        int bit = ler_bit(byte_lido);
        r |= (bit << (7 - i));
    }
    for(int i = 0 ; i < 8 ; i++)
    {
        int bit = ler_bit(byte_lido);
        g |= (bit << (7 - i));
    }
    for(int i = 0 ; i < 8 ; i++)
    {
        int bit = ler_bit(byte_lido);
        b |= (bit << (7 - i));
    }

    // colocando os valores em Atual
    atual->r = r;
    atual->g = g;
    atual->b = b;

    // iniciando atual como folha
    atual->folha = 1;
}
void mostra_arvore(Arvore* arv, int n)
{
    printf("\n%d", n);
    if (arv ->folha)
        printf("\n %d = (%d , %d , %d)", n , arv->r, arv->g, arv->b);
    if(arv->esq != NULL)
    {
        printf("\n filha esq de %d", n);
        mostra_arvore(arv->esq, n+1);
    }
    if(arv->dir != NULL)
    {
        printf("\n filha dir de %d", n);
        mostra_arvore(arv->dir, n+1);
    }
}

#endif

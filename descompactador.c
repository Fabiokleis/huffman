#include <stdio.h>
#include "leitor_bit.h"
#include "Arvore.h"
#include "pilha.h"

///realmente o descompactador -----------------
#define ARVORE "arvore.kh4"
#define CODIGO "codigo.kh4"
#define ALTURA 4
#define LARGURA 4
#define CANAIS 3
#define TAM_PILHA 100


void mostra_saida(uint8_t img[ALTURA][LARGURA][CANAIS])
{
    for (int y = 0 ; y < ALTURA ; y++)
    {
        printf("\n");
        for (int x = 0 ; x < LARGURA ; x++)
            printf("(%d , %d , %d)", img[y][x][0], img[y][x][1], img[y][x][2]);
    }
}

void descompacta_grid(int altura, int largura, Leitor_de_bytes *arvore, Leitor_de_bytes *codigo, Arvore* root)
{
    //abrindo o arquivo
    printf("Chamou descompacta_grid\n");
    //abrindo o arquivo
    Leitor_de_bytes *byte_lido = arvore;
    if (!byte_lido)
        return 1;

    int bit1, bit2;
    Arvore* raiz = aloca_arvore();
    Arvore* atual = raiz;
    Pilha* pilha_de_filhos = inicializa_pilha(TAM_PILHA);

    //lendo cada bit
    while ((bit1 = ler_bit(byte_lido)) != 255)

    {
        printf("bit1: %d\n", bit1);
        bit2 =  ler_bit(byte_lido);
        //criando filhos se tiver
        if(bit2 == 1)
        {
            atual->dir = aloca_arvore();
            coloca_na_pilha(pilha_de_filhos, atual -> dir);
        }
        if (bit1 == 1)
        {
            atual->esq = aloca_arvore();
            coloca_na_pilha(pilha_de_filhos, atual -> esq);
        }
        //iniciando a cor no Nó se for uma folha
        if((bit1 == 0) && (bit2 == 0))
            le_cor(atual, byte_lido);

        //pega o proximo ponteiro
        atual = retira_da_pilha(pilha_de_filhos);

        //se acabou a arvore encerra o loop
        if (atual == NULL)
            break;
    }


    fechar_Leitor_de_bytes(byte_lido);

    //debugando com print pq né
    mostra_arvore(raiz, 0);

    //arvore feita ======================================
    printf("Arvore lida e feita\n");

    //abrindo o arquivo
    byte_lido = codigo;
    if (!byte_lido)
        return 1;

    //lendo cada bit
    atual = raiz;
    uint8_t saida[ALTURA][LARGURA][CANAIS];
    for (int y = 0 ; y < ALTURA ; y++)
    {
        printf("y: %d\n", y);
        for (int x = 0 ; x < LARGURA ; x++)
        {
            printf("x: %d\n", x);
            while (!(atual -> folha))
            {
                //codigo de huffman até encontrar a cor
                bit1 = ler_bit(byte_lido);
                if (bit1 == 1)
                    atual = atual->esq;
                else if (bit1 == 0)
                    atual = atual -> dir;
            }

            // TODO: usar essa saida, ou retornar a matriz na funcao para concatenar no arquivo BMP
            //colocando a cor na saida
            saida[y][x][0] = atual -> r;
            saida[y][x][1] = atual -> g;
            saida[y][x][2] = atual -> b;
            //reiniciando o atual na raiz
            atual = raiz;
        }
    }


    //fecha o arquivo do codigo
    fechar_Leitor_de_bytes(byte_lido);

    mostra_saida(saida);

    return 0;
    
    
}

// TODO: transformar a main em uma funcao, desconpacta_grid ou algo assim que recebe como parametros (altura da grid, largura da grid, leitor de bytes da arvore, e leitor de bytes do codigo de huff)
// OBS, tem que adaptar dentro do codigo tirando a criacao do leitor_de_bytes pra fora, se quiser ler como ele funciona é bem simples só tem uma funcao de verdade, se nao eu arrumo isso amanha ou algum outro dia com mais calma
int main_teste() {
    //abrindo o arquivo
    Leitor_de_bytes *byte_lido = criar_Leitor_de_bytes(ARVORE);
    if (!byte_lido)
        return 1;

    int bit1, bit2;
    Arvore* raiz = aloca_arvore();
    Arvore* atual = raiz;
    Pilha* pilha_de_filhos = inicializa_pilha(TAM_PILHA);

    //lendo cada bit
    while ((bit1 = ler_bit(byte_lido)) != 255)

    {
        bit2 =  ler_bit(byte_lido);
        //criando filhos se tiver
        if(bit2 == 1)
        {
            atual->dir = aloca_arvore();
            coloca_na_pilha(pilha_de_filhos, atual -> dir);
        }
        if (bit1 == 1)
        {
            atual->esq = aloca_arvore();
            coloca_na_pilha(pilha_de_filhos, atual -> esq);
        }
        //iniciando a cor no Nó se for uma folha
        if((bit1 == 0) && (bit2 == 0))
            le_cor(atual, byte_lido);

        //pega o proximo ponteiro
        atual = retira_da_pilha(pilha_de_filhos);

        //se acabou a arvore encerra o loop
        if (atual == NULL)
            break;
    }

    fechar_Leitor_de_bytes(byte_lido);

    //debugando com print pq né
    //mostra_arvore(raiz, 0);

    //arvore feita ======================================

    //abrindo o arquivo
    byte_lido = criar_Leitor_de_bytes(CODIGO);
    if (!byte_lido)
        return 1;

    //lendo cada bit
    atual = raiz;
    uint8_t saida[ALTURA][LARGURA][CANAIS];
    for (int y = 0 ; y < ALTURA ; y++)
    {
        for (int x = 0 ; x < LARGURA ; x++)
        {
            while (!(atual -> folha))
            {
                //codigo de huffman até encontrar a cor
                bit1 = ler_bit(byte_lido);
                if (bit1 == 1)
                    atual = atual->esq;
                else if (bit1 == 0)
                    atual = atual -> dir;
            }

            // TODO: usar essa saida, ou retornar a matriz na funcao para concatenar no arquivo BMP
            //colocando a cor na saida
            saida[y][x][0] = atual -> r;
            saida[y][x][1] = atual -> g;
            saida[y][x][2] = atual -> b;
            //reiniciando o atual na raiz
            atual = raiz;
        }
    }


    //fecha o arquivo do codigo
    fechar_Leitor_de_bytes(byte_lido);

    mostra_saida(saida);

    return 0;
}

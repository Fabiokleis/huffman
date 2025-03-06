#include <stdio.h>
#include "leitor_bit.h"
#include "Arvore.h"
#include "pilha.h"
#include "imagem.h"
#include <stdlib.h>

///realmente o descompactador -----------------
#define ARVORE "tree.bin"
#define CODIGO "code.h4k"
#define SAIDA "resultado.bmp"
#define ALTURA_IMAGEM 720
#define LARGURA_IMAGEM 1280
#define TAM_GRID 4
#define CANAIS 3
#define DEBUG 0

void mostra_saida(uint8_t*** img)
{
    for (int y = 0 ; y < ALTURA_IMAGEM ; y++)
    {
        printf("\n");
        for (int x = 0 ; x < LARGURA_IMAGEM ; x++)
            printf("(%d , %d , %d)", img[y][x][0], img[y][x][1], img[y][x][2]);
    }
}

int descompacta_grid(int altura, int largura, char *tree, int *at_tree, int *at_huff, char *huff, float*** saida, int x_ini_saida, int y_ini_saida)
{

    //abrindo o arquivo
    Leitor_de_bytes* arvore = criar_Leitor_de_bytes(tree, *at_tree);
    if (!arvore)
        return 1;

    uint8_t bit1, bit2; //pra poder receber -1
    Arvore* raiz = aloca_arvore();
    Arvore* atual = raiz;
    Pilha* pilha_de_filhos = inicializa_pilha(100); //tamanho 100 empirico pra pilha

    //lendo cada bit
    while (atual)
    {
        bit1 = ler_bit(arvore);
        bit2 =  ler_bit(arvore);

#if DEBUG
	printf("par lido %d%d\n", bit1, bit2);
	//getchar();
#endif
        //criando filhos se tiver
        if(bit2 == 1)
        {
#if DEBUG
	  printf("Cria filho a direita\n");
#endif
            atual->dir = aloca_arvore();
            coloca_na_pilha(pilha_de_filhos, atual -> dir);
        }
        if (bit1 == 1)
        {
#if DEBUG
            printf("Cria filho a esquerda\n");
#endif
            atual->esq = aloca_arvore();
            coloca_na_pilha(pilha_de_filhos, atual -> esq);
        }
        //iniciando a cor no Nó se for uma folha
        if((bit1 == 0) && (bit2 == 0))
        {
            if (!atual) printf("lendo cor em NULL");
#if DEBUG
            printf("Le uma cor\n");
#endif	    
            le_cor(atual, arvore);
        }
        //pega o proximo ponteiro
#if DEBUG
	printf("\npegando proximo no da pilha\n");
#endif
        atual = retira_da_pilha(pilha_de_filhos);
#if DEBUG
        printf("a pilha ainda tem %d\n", pilha_de_filhos->quantidade);
#endif
    }

    //Aqui a arvore ja foi lida logo a gente garante que o proximo leitor vai começar na proxima arvore e nao em padding
    //while(arvore -> posicao_bit < 8)
        //bit1 == ler_bit(arvore);

    //debugando com print pq né
#if DEBUG
    mostra_arvore(raiz, 0);
#endif
    desaloca_pilha(pilha_de_filhos);

    *at_tree = fechar_Leitor_de_bytes(arvore);

#if DEBUG
    printf("\n at_huff: %d\n", *at_tree);
#endif

    Leitor_de_bytes* codigo = criar_Leitor_de_bytes(huff, *at_huff);
    if (!codigo) return 1;

    //lendo cada bit
    for (int y = 0 ; y < altura ; y++)
    {
        for (int x = 0 ; x < largura ; x++)
        {
            //reiniciando o atual na raiz
            atual = raiz;
            while (atual->esq || atual->dir) //no huff nao tem no de 1 só
            {
                //codigo de huffman até encontrar a cor
                bit1 = ler_bit(codigo);
                if (bit1 == 0){
                    if (!atual->esq){
                        printf("\ntentou acessar esq inexistente");
                    }
                    else
                        atual = atual->esq;
                }
                else if (bit1 == 1)
                {
                    if (!atual->dir){
                        printf("\ntentou acessar esq inexistente");
                    }
                    else
                        atual = atual -> dir;
                }
                else
                    printf("LEU ALGUMA COISA MUITO ERRADA");
            }
            //colocando a cor na saida
#if DEBUG
	    printf(" x: %d, y: %d cor (%d,%d,%d)\n", x_ini_saida + x,  y_ini_saida + y, atual->r,atual->g,atual->b);
#endif	    
            saida[0][y_ini_saida + y][x_ini_saida + x] = atual -> r / 255.0;
            saida[1][y_ini_saida + y][x_ini_saida + x] = atual -> g / 255.0;
            saida[2][y_ini_saida + y][x_ini_saida + x] = atual -> b / 255.0;
        }
    }

    desaloca_arvore(raiz);
    *at_huff = fechar_Leitor_de_bytes(codigo);
#if DEBUG
    printf("\n at_huff: %d\n", *at_huff);
#endif

    //fecha o arquivo do codigo
    //fechar_Leitor_de_bytes(byte_lido);

    //mostra_saida(saida);

    return 0;
}


int main()
{
    printf("descompactando arquivo %s e %s...\n", CODIGO, ARVORE);
    Imagem* saida = criaImagem(LARGURA_IMAGEM, ALTURA_IMAGEM, CANAIS);
    if (!saida)
        return 1;

    int grid = 0;

    int atual_huff = 0;
    int atual_arvore = 0;

    for (int y_out = 0 ; y_out < ALTURA_IMAGEM ; y_out += TAM_GRID)
    {
        for(int x_out = 0 ; x_out < LARGURA_IMAGEM ; x_out+= TAM_GRID, grid++)
        {
            descompacta_grid(TAM_GRID, TAM_GRID, ARVORE, &atual_arvore, &atual_huff, CODIGO, saida->dados, x_out, y_out);

#if DEBUG
            printf("Grid Atual: %d\n", grid);
#endif
        }
    }
    salvaImagem(saida, SAIDA);
    printf("arquivo descompactado %s\n", SAIDA);
}

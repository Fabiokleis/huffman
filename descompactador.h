#ifndef DESCOMPACTADOR_H
#define DESCOMPACTADOR_H

#include <stdio.h>
#include <stdint.h>
#include "leitor_bit.h"
#include "Arvore.h"
#include "pilha.h"

// Definições de constantes
#define ARVORE "arvore.kh4"
#define CODIGO "codigo.kh4"
#define ALTURA 200
#define LARGURA 200
#define CANAIS 3
#define TAM_PILHA 100

// Função para exibir a matriz de saída
void mostra_saida(uint8_t img[ALTURA][LARGURA][CANAIS]);

// Função para descompactar a grade a partir da árvore de Huffman
void descompacta_grid(int altura, int largura, char *tree, int* at_tree, int* at_huff, char *huff, float*** saida, int x_ini_saida, int y_ini_saida);

// Função principal da descompactação
int descompactador_main();

#endif // DESCOMPACTADOR_H

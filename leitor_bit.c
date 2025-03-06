#ifndef LEITOR_BIT_C
#define LEITOR_BIT_C

#include <stdio.h>
#include <stdlib.h>
#include "leitor_bit.h"

//criando e inicializando o leitor de bytes
Leitor_de_bytes* criar_Leitor_de_bytes(char *nome_arquivo, int ini)
{
    Leitor_de_bytes *byte_lido = (Leitor_de_bytes*)malloc(sizeof(Leitor_de_bytes));

    //abrindo o arquivo e salvando no leitor
    byte_lido -> arquivo = fopen(nome_arquivo, "rb");
    if (!byte_lido -> arquivo)
    {
        printf("\n Erro ao abrir o arquivo \"%s\"\n", nome_arquivo);
        free(byte_lido);
        return NULL;
    }

    byte_lido -> buffer = 0;
    byte_lido ->atual = 0;
    //inicializa um bit "ja lido"
    byte_lido -> posicao_bit = 8;

    for (int i = 0 ; i < ini ; i++)
        ler_bit(byte_lido);
    return byte_lido;
}

//funcao que le o proximo bit do leitor
uint8_t ler_bit(Leitor_de_bytes *byte_lido) {
    //se o byte ja foi lido, le o proximo
    if (byte_lido -> posicao_bit == 8)
    {
        uint8_t byte_lido_novo = fgetc(byte_lido->arquivo);

        byte_lido->buffer = (uint8_t) byte_lido_novo;
        byte_lido->posicao_bit = 0;
    }

    //isola e le o bit atual
    uint8_t bit = (byte_lido->buffer >> (7 - byte_lido->posicao_bit)) & 1;

    //printf("bit n %d valor %d\n",byte_lido->posicao_bit+1, bit);
    byte_lido->posicao_bit++;
    byte_lido->atual++;
    return bit;
}

//destruindo o leitor de bytes e fechar o arquivo lido
int fechar_Leitor_de_bytes(Leitor_de_bytes *byte_lido) {
    fclose(byte_lido -> arquivo);
    int aux = byte_lido->atual;
    free(byte_lido);
    return aux;
}

#endif

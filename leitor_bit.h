#ifndef LEITOR_BIT_H
#define LEITOR_BIT_H

#include <stdio.h>
#include <stdint.h>

//struct para ler arquivos binarios
typedef struct {
    FILE *arquivo;
    uint8_t buffer;
    int posicao_bit;
    int inicial;
    int atual;
} Leitor_de_bytes;

//criando e inicializando o leitor de bytes
Leitor_de_bytes* criar_Leitor_de_bytes(char *nome_arquivo, int inicial);

//funcao que le o proximo bit do leitor
uint8_t ler_bit(Leitor_de_bytes *byte_lido);

//destruindo o leitor de bytes e fechar o arquivo lido
int fechar_Leitor_de_bytes(Leitor_de_bytes *byte_lido);

#endif

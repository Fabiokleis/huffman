/*============================================================================*/
/* MANIPULA��O DE ARQUIVOS BMP                                                */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu - nassu@dainf.ct.utfpr.edu.br                       */
/*============================================================================*/
/** Este arquivo traz declara��es de um tipo e rotinas para manipula��o de
 * arquivos bmp. Como temos um prop�sito puramente did�tico, apenas um sub-
 * conjunto m�nimo do formato foi implementado. Usamos aqui matrizes alocadas
 * dinamicamente e n�meros de ponto flutuante. Esta n�o � a forma mais eficiente
 * de se trabalhar com imagens, mas procuramos priorizar a clareza e a
 * facilidade de uso. */
/*============================================================================*/

#ifndef __IMAGEM_H
#define __IMAGEM_H

/*============================================================================*/

typedef struct
{
	int largura;
	int altura;
	int n_canais;
	float*** dados; /* Uma matriz de dados por canal. Acessar com 3 �ndices: [canal][y][x]. */
} Imagem;

/*----------------------------------------------------------------------------*/
/* Por simplicidade e compatibilidade, n�s sempre consideramos a leitura e
 * escrita de imagens com 3 canais, 24bpp. Todas as convers�es para escala de
 * cinza e float s�o feitas internamente. */

Imagem* criaImagem (int largura, int altura, int n_canais);
void destroiImagem (Imagem* img);
Imagem* abreImagem (char* arquivo, int n_canais);
int salvaImagem (Imagem* img, char* arquivo);
Imagem* clonaImagem (Imagem* img);
void copiaConteudo (Imagem* in, Imagem* out);

/*============================================================================*/
#endif /* __IMAGEM_H */
/*============================================================================*/
/* Exemplo: segmenta��o de uma imagem em escala de cinza.                     */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu                                                     */
/* Universidade Tecnol�gica Federal do Paran�                                 */
/*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pdi.h"

/*============================================================================*/

#define INPUT_IMAGE "wind_waker.bmp"

// TODO: ajuste estes par�metros!
#define NEGATIVO 0
#define THRESHOLD 0.4f
#define ALTURA_MIN 1
#define LARGURA_MIN 1
#define N_PIXELS_MIN 1

/*============================================================================*/

typedef struct
{
    float label; // R�tulo do componente.
    Retangulo roi; // Coordenadas do ret�ngulo envolvente do componente.
    int n_pixels; // N�mero de pixels do componente.

} Componente;

/*============================================================================*/

void binariza (Imagem* in, Imagem* out, float threshold);
int rotula (Imagem* img, Componente** componentes, int largura_min, int altura_min, int n_pixels_min);

/*============================================================================*/

int teste_img ()
{
    int i;

    // Abre a imagem em escala de cinza, e mant�m uma c�pia colorida dela para desenhar a sa�da.
    Imagem* img = abreImagem (INPUT_IMAGE, 1);
    if (!img)
    {
        printf ("Erro abrindo a imagem.\n");
        exit (1);
    }

    Imagem* img_out = criaImagem (img->largura, img->altura, 3);
    printf("largura: %d\n", img->largura);
    printf("altura: %d\n", img->altura);


//    salvaImagem (img, "01 - binarizada.bmp");

    destroiImagem (img_out);
    destroiImagem (img);
    return (0);
}
Imagem* get_imagem(int canais) {
    Imagem* img = abreImagem(INPUT_IMAGE, canais);
    if (!img) {
        printf("Erro abrindo a imagem.\n");
        exit(1);
    }
    float*** new_img = malloc(img->altura * sizeof(float**));
    for (int y = 0; y < img->altura; y++) {
        new_img[y] = malloc(img->largura * sizeof(float*));
        for (int x = 0; x < img->largura; x++) {
            new_img[y][x] = malloc(img->n_canais * sizeof(float));
        }
    }
    for (int y = 0; y < img->altura; y++) {
        for (int x = 0; x < img->largura; x++) {
            for (int c = 0; c < img->n_canais; c++) {
                new_img[y][x][c] = img->dados[c][y][x];
            }
        }
    }
    for (int c = 0; c < img->n_canais; c++) {
        for (int y = 0; y < img->altura; y++) {
            free(img->dados[c][y]);
        }
        free(img->dados[c]);
    }
    free(img->dados);
    img->dados = new_img;

    return img;
}

/*----------------------------------------------------------------------------*/
/** Binariza��o simples por limiariza��o.
 *
 * Par�metros: Imagem* in: imagem de entrada. Se tiver mais que 1 canal,
 *               binariza cada canal independentemente.
 *             Imagem* out: imagem de sa�da. Deve ter o mesmo tamanho da
 *               imagem de entrada.
 *             float threshold: limiar.
 *
 * Valor de retorno: nenhum (usa a imagem de sa�da). */

void binariza (Imagem* in, Imagem* out, float threshold)
{
    if (in->largura != out->largura || in->altura != out->altura || in->n_canais != out->n_canais)
    {
        printf ("ERRO: binariza: as imagens precisam ter o mesmo tamanho e numero de canais.\n");
        exit (1);
    }

    // TODO: escreva o c�digo desta fun��o.
}

/*============================================================================*/
/* ROTULAGEM                                                                  */
/*============================================================================*/
/** Rotulagem usando flood fill. Marca os objetos da imagem com os valores
 * [0.1,0.2,etc].
 *
 * Par�metros: Imagem* img: imagem de entrada E sa�da.
 *             Componente** componentes: um ponteiro para um vetor de sa�da.
 *               Supomos que o ponteiro inicialmente � inv�lido. Ele ir�
 *               apontar para um vetor que ser� alocado dentro desta fun��o.
 *               Lembre-se de desalocar o vetor criado!
 *             int largura_min: descarta componentes com largura menor que esta.
 *             int altura_min: descarta componentes com altura menor que esta.
 *             int n_pixels_min: descarta componentes com menos pixels que isso.
 *
 * Valor de retorno: o n�mero de componentes conexos encontrados. */

int rotula (Imagem* img, Componente** componentes, int largura_min, int altura_min, int n_pixels_min)
{
    // TODO: escreva esta fun��o.
	// Use a abordagem com flood fill recursivo.
	// Observe que o par�metro 'componentes' � um ponteiro para um vetor, ent�o a aloca��o dele deve ser algo como:
	// *componentes = malloc (sizeof (Componente) * n);
	// Dependendo de como voc� fizer a sua implementa��o, pode ser tamb�m interessante alocar primeiro um vetor maior do que o necess�rio, ajustando depois o tamanho usando a fun��o realloc.
    return (0);
}

/*============================================================================*/
#include "kmeans.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

float dist_euclidiana(float x1, float y1, float z1, float x2, float y2, float z2)
{
    return sqrt(pow((x2 - x1) , 2) + pow((y2 - y1) , 2) + pow((z2 - z1) , 2));
}

void k_means(float*** img, uint8_t*** out, int altura, int largura, int n, int max_interacoes)
{
    //definindo os pontos aleatorios do k-means
    float cores[n][3];

    for (int i = 0 ; i < n ; i++)
    {
        int rand_x = (rand() % largura);
        int rand_y = (rand() % altura);
        for (int z = 0 ; z < 3 ; z++)
            cores[i][z] = img[rand_y][rand_x][z];
    }

    //mapa pra saber a separacao dos grupos inicializado em um grupo que nao existe
    int mapa[altura][largura];
    for (int y = 0 ; y < altura ; y++)
        for (int x = 0 ; x < largura ; x++)
            mapa[y][x] = 0;

    int moveu = TRUE;
    int interacoes = 0;
    float somas_grupo[n][3];
    int elementos_grupo[n][3];
    for (int i = 0 ; i < n ; i++)
        for (int z = 0 ; z < 3 ; z++)
            somas_grupo[i][z] = 0;

    for (int y = 0 ; y < altura ; y++)
        for (int x = 0 ; x < largura ; x++)
            for (int z = 0 ; z < 3 ; z++)
                somas_grupo[0][z] += img[y][x][z];

    for (int z = 0 ; z < 3 ; z++)
        elementos_grupo[0][z] = (altura * largura);


    while (moveu && interacoes < max_interacoes)
    {
        moveu = FALSE;
        interacoes++;
        //escolhendo a cor mais perto pra cada ponto
        for (int y = 0 ; y < altura ; y++)
        {
            for (int x = 0 ; x < largura ; x++)
            {
                //inicializa no valor do antigo grupo como menor distancia
                float min = dist_euclidiana(img[y][x][0], img[y][x][1], img[y][x][2], cores[mapa[y][x]][0], cores[mapa[y][x]][1], cores[mapa[y][x]][2]);
                int grupo = mapa[y][x];

                for (int i = 0 ; i < n ; i++)
                {
                    float dist = dist_euclidiana(img[y][x][0], img[y][x][1], img[y][x][2], cores[i][0], cores[i][1], cores[i][2]);
                    if (dist < min)
                    {
                        min = dist;
                        moveu = TRUE;

                        //desconta do grupo anterior
                        for (int z = 0 ; z < 3 ; z++)
                        {
                            somas_grupo[grupo][z] -= img[y][x][z];
                            elementos_grupo[grupo][z]--;
                        }
                        grupo = i;
                        mapa[y][x] = grupo;

                        //adiciona no novo grupo
                        for (int z = 0 ; z < 3 ; z++)
                        {
                            somas_grupo[grupo][z] += img[y][x][z];
                            elementos_grupo[grupo][z]++;
                        }
                    }
                }
            }
        }
        //Recalculando as cores, as centroides
        for (int i = 0 ; i < n ; i++)
        {
            for (int z = 0 ; z < 3 ; z++)
            {
                if (elementos_grupo[i][z] > 0)
                    cores[i][z] = (somas_grupo[i][z] / elementos_grupo[i][z]);
                else
                {
                    int rand_x = (rand() % largura);
                    int rand_y = (rand() % altura);
                    for (int z = 0 ; z < 3 ; z++)
                        cores[i][z] = img[rand_y][rand_x][z];
                }
            }
        }
    }
    //quando terminado trocando as cores aintigas pelas novas na imagem
    /* for (int y = 0 ; y < altura ; y++) */
    /*     for (int x = 0 ; x < largura ; x++) */
    /*         for (int z = 0 ; z < 3 ; z++) */
    /*             img[y][x][z] = cores[mapa[y][x]][z]; */
    
    for (int y = 0 ; y < altura ; y++)
        for (int x = 0 ; x < largura ; x++)
            for (int z = 0 ; z < 3 ; z++)
	      out[y][x][z] = (uint8_t)(cores[mapa[y][x]][z] * 255);
    
}


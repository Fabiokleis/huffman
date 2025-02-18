#ifndef KMEANS_H
#define KMEANS_H
#include <stdint.h>

float dist_euclidiana(float x1, float y1, float z1, float x2, float y2, float z2);
void k_means(float*** img, uint8_t*** out, int altura, int largura, int n, int max_interacoes);

#endif /* KMEANS_H */

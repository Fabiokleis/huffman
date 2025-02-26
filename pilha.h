#ifndef PILHA_H
#define PILHA_H

typedef struct {
    Arvore** vetor_Arvores;
    int quantidade;
} Pilha;

Pilha* inicializa_pilha(int tam);
void coloca_na_pilha(Pilha* pil, Arvore* arv);
Arvore* retira_da_pilha(Pilha* pil);

#endif
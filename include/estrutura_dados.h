#ifndef ESTRUTURA_DADOS_H
#define ESTRUTURA_DADOS_H

#include "processo.h"

// Nó da fila encadeada
typedef struct No {
    Processo* processo;
    struct No* prox;
} No;

// Estrutura da Fila
typedef struct Fila {
    No* inicio;
    No* fim;
    int tamanho;
} Fila;

// Assinaturas das funções de manipulação da fila
Fila* criar_fila();
void enfileirar(Fila* f, Processo* p);
Processo* desenfileirar(Fila* f);
int fila_vazia(Fila* f);
void liberar_fila(Fila* f);

#endif 
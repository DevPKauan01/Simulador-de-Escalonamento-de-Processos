#include <stdio.h>
#include <stdlib.h>
#include "../include/estrutura_dados.h"
#include "../include/processo.h"


Fila* criar_fila() {
    Fila* f = (Fila*)malloc(sizeof(Fila));
    if (f != NULL) {
        f->inicio = NULL;
        f->fim = NULL;
        f->tamanho = 0;
    }
    return f;
}

// Insere um processo no final da fila
void enfileirar(Fila* f, Processo* p) {
    if (f == NULL || p == NULL) return;

    No* novo = (No*)malloc(sizeof(No));
    novo->processo = p;
    novo->prox = NULL;

    if (f->fim == NULL) {
        f->inicio = novo;
    } else {
        f->fim->prox = novo;
    }
    f->fim = novo;
    f->tamanho++;
}

// Remove e retorna o processo do início da fila
Processo* desenfileirar(Fila* f) {
    if (f == NULL || f->inicio == NULL) return NULL;

    No* temp = f->inicio;
    Processo* p = temp->processo;

    f->inicio = temp->prox;
    if (f->inicio == NULL) {
        f->fim = NULL;
    }
    
    free(temp);
    f->tamanho--;
    return p;
}

// Verifica se a fila está vazia
int fila_vazia(Fila* f) {
    return (f == NULL || f->inicio == NULL);
}

// Libera a memória alocada para a fila
void liberar_fila(Fila* f) {
    while (!fila_vazia(f)) {
        desenfileirar(f);
    }
    free(f);
}
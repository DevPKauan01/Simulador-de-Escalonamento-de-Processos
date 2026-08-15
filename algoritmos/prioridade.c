#include <stdlib.h>
#include "../include/escalonador.h"
#include "../include/processo.h"
#include "../include/estrutura_dados.h"

Processo* prioridade_escolher(Fila* fila_prontos) {
    if (fila_vazia(fila_prontos)) return NULL;

    No* atual = fila_prontos->inicio;
    No* no_escolhido = atual;
    Processo* melhor = atual->processo;

    atual = atual->prox;
    while (atual != NULL) {
        if (atual->processo->prioridade < melhor->prioridade) {
            melhor = atual->processo;
            no_escolhido = atual;
        }
        atual = atual->prox;
    }

    No* anterior = NULL;
    No* percorre = fila_prontos->inicio;
    while (percorre != no_escolhido) {
        anterior = percorre;
        percorre = percorre->prox;
    }

    if (anterior == NULL) {
        fila_prontos->inicio = no_escolhido->prox;
    } else {
        anterior->prox = no_escolhido->prox;
    }
    if (no_escolhido == fila_prontos->fim) {
        fila_prontos->fim = anterior;
    }
    fila_prontos->tamanho--;

    free(no_escolhido);
    return melhor;
}

int prioridade_preemptar(Processo* processo_atual) {
    return 0;
}

Algoritmo criar_prioridade(void) {
    Algoritmo a;
    a.escolher_proximo = prioridade_escolher;
    a.deve_preemptar = prioridade_preemptar;
    return a;
}

#include <stdlib.h>
#include "../include/escalonador.h"
#include "../include/processo.h"
#include "../include/estrutura_dados.h"

#define FATOR_ENVELHECIMENTO 0.6
#define QUANTUM_MAXIMO 8
#define QUANTUM_MINIMO 2

static int ticks_executando = 0;
static int pid_em_execucao = -1;

static int calcular_quantum(int prioridade) {
    int quantum = QUANTUM_MAXIMO - (prioridade - 1);
    if (quantum < QUANTUM_MINIMO) quantum = QUANTUM_MINIMO;
    if (quantum > QUANTUM_MAXIMO) quantum = QUANTUM_MAXIMO;
    return quantum;
}

Processo* proprio_escolher(Fila* fila_prontos) {
    if (fila_vazia(fila_prontos)) return NULL;

    No* atual = fila_prontos->inicio;
    No* no_escolhido = atual;
    Processo* melhor = atual->processo;
    double melhor_score = melhor->prioridade - FATOR_ENVELHECIMENTO * melhor->tempo_espera_pronto;

    atual = atual->prox;
    while (atual != NULL) {
        double score = atual->processo->prioridade - FATOR_ENVELHECIMENTO * atual->processo->tempo_espera_pronto;
        if (score < melhor_score) {
            melhor_score = score;
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

    if (melhor->id != pid_em_execucao) {
        ticks_executando = 0;
        pid_em_execucao = melhor->id;
    }

    return melhor;
}

int proprio_preemptar(Processo* processo_atual) {
    ticks_executando++;
    int quantum = calcular_quantum(processo_atual->prioridade);
    if (ticks_executando >= quantum) {
        ticks_executando = 0;
        pid_em_execucao = -1;
        return 1;
    }
    return 0;
}

Algoritmo criar_proprio(void) {
    Algoritmo a;
    a.escolher_proximo = proprio_escolher;
    a.deve_preemptar = proprio_preemptar;
    return a;
}

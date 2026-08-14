#include <stdlib.h>
#include "../include/escalonador.h"
#include "../include/processo.h"
#include "../include/estrutura_dados.h"

Processo* fcfs_escolher(Fila* fila_prontos) {
    if (fila_vazia(fila_prontos)) return NULL;
    return desenfileirar(fila_prontos);
}

int fcfs_preemptar(Processo* processo_atual) {
    return 0; // FCFS nunca preempta
}

Algoritmo criar_fcfs(void) {
    Algoritmo a;
    a.escolher_proximo = fcfs_escolher;
    a.deve_preemptar = fcfs_preemptar;
    return a;
}

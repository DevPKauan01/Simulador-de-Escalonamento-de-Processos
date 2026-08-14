#include <stdlib.h>
#include "../include/escalonador.h"
#include "../include/processo.h"
#include "../include/estrutura_dados.h"

static int quantum = 4;
static int ticks_consumidos = 0;

Processo* rr_escolher(Fila* fila_prontos) {
    ticks_consumidos = 0; // reset ao escolher novo processo
    if (fila_vazia(fila_prontos)) return NULL;
    return desenfileirar(fila_prontos);
}

int rr_preemptar(Processo* processo_atual) {
    ticks_consumidos++;
    if (ticks_consumidos >= quantum) {
        ticks_consumidos = 0;
        return 1; // força preempção
    }
    return 0;
}

Algoritmo criar_rr(int q) {
    quantum = q;
    Algoritmo a;
    a.escolher_proximo = rr_escolher;
    a.deve_preemptar = rr_preemptar;
    return a;
}
#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "processo.h"
#include "estrutura_dados.h"

// Estrutura genérica para plugar os diferentes algoritmos no simulador
typedef struct {
    // Ponteiro de função para escolher o próximo processo da fila de prontos
    Processo* (*escolher_proximo)(Fila* fila_prontos);
    
    // Ponteiro de função opcional para preempção (ex: verificar quantum no Round Robin)
    // Retorna 1 se deve preemptar, 0 caso contrário.
    int (*deve_preemptar)(Processo* processo_atual);
} Algoritmo;

#endif // ESCALONADOR_H
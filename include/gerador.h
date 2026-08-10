#ifndef GERADOR_H
#define GERADOR_H

#include "processo.h"

// Cenários obrigatórios definidos na especificação
typedef enum {
    CENARIO_ALEATORIO_EQUILIBRADO = 1,
    CENARIO_IO_BOUND = 2,
    CENARIO_CPU_BOUND = 3,
    CENARIO_PRIORIDADES_DESBALANCEADAS = 4
} CenarioSimulacao;

// Gera um vetor de ponteiros para processos com base em uma seed
Processo** gerar_carga_trabalho(unsigned int seed, CenarioSimulacao cenario, int num_processos);

// Libera a memória alocada para um lote de processos
void liberar_carga_trabalho(Processo **processos, int num_processos);

#endif // GERADOR_H
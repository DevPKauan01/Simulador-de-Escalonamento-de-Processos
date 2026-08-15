#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processo.h"
#include "gerador.h"
#include "simulador.h"
#include "escalonador.h"

Algoritmo criar_fcfs(void);
Algoritmo criar_rr(int q);
Algoritmo criar_prioridade(void);
Algoritmo criar_proprio(void);

int main(int argc, char *argv[]) {
    unsigned int seed = 42;
    int num_processos = 1000;
    CenarioSimulacao cenario = CENARIO_ALEATORIO_EQUILIBRADO;
    char algoritmo[20] = "FCFS";
    int custo_troca_contexto = 2;
    int quantum = 10;

    if (argc >= 5) {
        seed = (unsigned int) atoi(argv[1]);
        cenario = (CenarioSimulacao) atoi(argv[2]);
        strncpy(algoritmo, argv[3], sizeof(algoritmo) - 1);
        num_processos = atoi(argv[4]);
    }

    Processo **processos = gerar_carga_trabalho(seed, cenario, num_processos);

    Algoritmo alg;
    if (strcmp(algoritmo, "FCFS") == 0) {
        alg = criar_fcfs();
    } else if (strcmp(algoritmo, "RR") == 0) {
        alg = criar_rr(quantum);
    } else if (strcmp(algoritmo, "PRIORIDADE") == 0) {
        alg = criar_prioridade();
    } else if (strcmp(algoritmo, "PROPRIO") == 0) {
        alg = criar_proprio();
    } else {
        fprintf(stderr, "Algoritmo %s nao implementado ainda.\n", algoritmo);
        liberar_carga_trabalho(processos, num_processos);
        return 1;
    }

    executar_simulacao(processos, num_processos, custo_troca_contexto, alg);

    liberar_carga_trabalho(processos, num_processos);
    return 0;
}

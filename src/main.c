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

    if (argc >= 7) { 
        seed = (unsigned int) atoi(argv[1]);
        cenario = (CenarioSimulacao) atoi(argv[2]);
        strncpy(algoritmo, argv[3], sizeof(algoritmo) - 1);
        num_processos = atoi(argv[4]);
        custo_troca_contexto = atoi(argv[5]);
        quantum = atoi(argv[6]);
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

    int trocas_contexto = executar_simulacao(processos, num_processos, custo_troca_contexto, alg);

    double avg_turnaround = 0.0;
    double sum_slowdown = 0.0;
    double sum_slowdown_sq = 0.0;
    
    for (int i = 0; i < num_processos; i++) {
        avg_turnaround += processos[i]->turnaround_time;
        
        double tempo_minimo_ideal = processos[i]->tempo_total_cpu + processos[i]->tempo_total_io;
        double slowdown = (double)processos[i]->turnaround_time / tempo_minimo_ideal;
        
        sum_slowdown += slowdown;
        sum_slowdown_sq += (slowdown * slowdown);
    }
    
    avg_turnaround /= num_processos;
    
    double jain_index = (sum_slowdown * sum_slowdown) / (num_processos * sum_slowdown_sq) * 100.0;

    printf("%s,%d,%u,%.4f,%d,%.4f\n", algoritmo, cenario, seed, avg_turnaround, trocas_contexto, jain_index);

    liberar_carga_trabalho(processos, num_processos);
    return 0;
}

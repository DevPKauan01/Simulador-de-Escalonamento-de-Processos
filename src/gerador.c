#include <stdio.h>
#include <stdlib.h>
#include "gerador.h"

Processo* criar_processo(int id, int tempo_chegada, int prioridade, int *rajadas, int num_rajadas) {
    Processo *p = (Processo*) malloc(sizeof(Processo));
    p->id = id;
    p->tempo_chegada = tempo_chegada;
    p->prioridade = prioridade;
    p->num_rajadas = num_rajadas;
    p->idx_rajada_atual = 0;
    
    p->rajadas = (int*) malloc(sizeof(int) * num_rajadas);
    p->tempo_total_cpu = 0;
    p->tempo_total_io = 0;

    for (int i = 0; i < num_rajadas; i++) {
        p->rajadas[i] = rajadas[i];
        if (i % 2 == 0) {
            p->tempo_total_cpu += rajadas[i];
        } else {
            p->tempo_total_io += rajadas[i];
        }
    }

    p->tempo_restante_rajada = p->rajadas[0];
    p->tempo_espera_pronto = 0;
    p->tempo_fim = 0;
    p->turnaround_time = 0;
    p->estado = ESTADO_NOVO;

    return p;
}

void destruir_processo(Processo *p) {
    if (p) {
        if (p->rajadas) free(p->rajadas);
        free(p);
    }
}

Processo** gerar_carga_trabalho(unsigned int seed, CenarioSimulacao cenario, int num_processos) {
    srand(seed); // Define a semente pseudoaleatória

    Processo **processos = (Processo**) malloc(sizeof(Processo*) * num_processos);
    int tempo_chegada_acumulado = 0;

    for (int i = 0; i < num_processos; i++) {
        // Modelo de chegada: intervalos aleatórios entre 0 e 5 ticks
        tempo_chegada_acumulado += (rand() % 6);
        int tempo_chegada = tempo_chegada_acumulado;

        int num_rajadas = 3 + (rand() % 5); // 3 a 7 rajadas (sempreímpar para terminar em CPU)
        if (num_rajadas % 2 == 0) num_rajadas++;

        int *rajadas = (int*) malloc(sizeof(int) * num_rajadas);
        int prioridade = 0;

        switch (cenario) {
            case CENARIO_ALEATORIO_EQUILIBRADO:
                prioridade = 1 + (rand() % 10);
                for (int j = 0; j < num_rajadas; j++) {
                    if (j % 2 == 0) rajadas[j] = 5 + (rand() % 20);  // CPU: 5 a 24
                    else           rajadas[j] = 5 + (rand() % 20);  // I/O: 5 a 24
                }
                break;

            case CENARIO_IO_BOUND:
                prioridade = 1 + (rand() % 10);
                for (int j = 0; j < num_rajadas; j++) {
                    if (j % 2 == 0) rajadas[j] = 1 + (rand() % 5);   // CPU curta: 1 a 5
                    else           rajadas[j] = 15 + (rand() % 30); // I/O longa: 15 a 44
                }
                break;

            case CENARIO_CPU_BOUND:
                prioridade = 1 + (rand() % 10);
                for (int j = 0; j < num_rajadas; j++) {
                    if (j % 2 == 0) rajadas[j] = 30 + (rand() % 50); // CPU longa: 30 a 79
                    else           rajadas[j] = 1 + (rand() % 5);   // I/O curta: 1 a 5
                }
                break;

            case CENARIO_PRIORIDADES_DESBALANCEADAS:
                // 80% Alta prioridade (1 a 3) e 20% Baixa prioridade (8 a 10)
                if ((rand() % 100) < 80) {
                    prioridade = 1 + (rand() % 3);
                } else {
                    prioridade = 8 + (rand() % 3);
                }
                for (int j = 0; j < num_rajadas; j++) {
                    if (j % 2 == 0) rajadas[j] = 5 + (rand() % 15);
                    else           rajadas[j] = 5 + (rand() % 15);
                }
                break;
        }

        processos[i] = criar_processo(i + 1, tempo_chegada, prioridade, rajadas, num_rajadas);
        free(rajadas); // O construtor aloca seu próprio vetor interno
    }

    return processos;
}

void liberar_carga_trabalho(Processo **processos, int num_processos) {
    if (!processos) return;
    for (int i = 0; i < num_processos; i++) {
        destruir_processo(processos[i]);
    }
    free(processos);
}
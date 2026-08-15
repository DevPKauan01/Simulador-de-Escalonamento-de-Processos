#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "processo.h"
#include "escalonador.h"

// Executa a simulação em tempo discreto iterando sobre os processos e a CPU
int executar_simulacao(Processo** lista_processos, int total_processos, int custo_troca_contexto, Algoritmo algoritmo);

#endif 
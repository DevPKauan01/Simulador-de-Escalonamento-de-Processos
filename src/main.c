#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processo.h"
#include "gerador.h"

// Assumindo que a interface da simulação existe em simulador.h
// void executar_simulacao(Processo **processos, int num_processos, const char* algoritmo, int custo_troca_contexto, int quantum);

int main(int argc, char *argv[]) {
    // Parâmetros padrão
    unsigned int seed = 42;
    int num_processos = 1000;      // Mínimo de 1.000 conforme especificação
    CenarioSimulacao cenario = CENARIO_ALEATORIO_EQUILIBRADO;
    char algoritmo[20] = "FCFS";
    int custo_troca_contexto = 2;   // Custo configurável > 0
    int quantum = 10;               // Para o Round Robin

    // Tratamento de argumentos por linha de comando
    // Uso: ./simulador <seed> <cenario [1-4]> <algoritmo [FCFS|RR|PRIORIDADE|PROPRIO]> <num_processos>
    if (argc >= 5) {
        seed = (unsigned int) atoi(argv[1]);
        cenario = (CenarioSimulacao) atoi(argv[2]);
        strncpy(algoritmo, argv[3], sizeof(algoritmo) - 1);
        num_processos = atoi(argv[4]);
    }

    // 1. Gerar carga de trabalho baseada na Seed
    Processo **processos = gerar_carga_trabalho(seed, cenario, num_processos);

    // 2. Imprimir cabeçalho se for execução individual (útil para testes de depuração)
    // fprintf(stderr, "Executando Seed=%u | Cenario=%d | Algoritmo=%s | N=%d\n", seed, cenario, algoritmo, num_processos);

    // 3. Chamar a Engine da Simulação (implementada pelo Integrante 1/3)
    // executar_simulacao(processos, num_processos, algoritmo, custo_troca_contexto, quantum);

    // 4. Liberação de memória
    liberar_carga_trabalho(processos, num_processos);

    return 0;
}
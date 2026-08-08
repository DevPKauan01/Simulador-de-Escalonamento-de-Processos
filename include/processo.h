#ifndef PROCESSO_H
#define PROCESSO_H

typedef enum {
    ESTADO_NOVO,
    ESTADO_PRONTO,
    ESTADO_EXECUTANDO,
    ESTADO_BLOQUEADO,
    ESTADO_FINALIZADO
} EstadoProcesso;

typedef struct {
    int id;
    int tempo_chegada;
    int prioridade;             // Ex: menor valor = maior prioridade
    
    // Lista de rajadas alternadas: [CPU, I/O, CPU, I/O, CPU...]
    int *rajadas;
    int num_rajadas;
    int idx_rajada_atual;       // Qual rajada está sendo executada no momento
    
    // Contadores de tempo da rajada atual
    int tempo_restante_rajada;
    
    // Métricas individuais de desempenho
    int tempo_espera_pronto;    // Tempo total gasto na fila de prontos
    int tempo_total_cpu;        // Soma de todas as rajadas de CPU
    int tempo_total_io;         // Soma de todas as rajadas de I/O
    int tempo_fim;              // Instante (tick) em que o processo finalizou
    int turnaround_time;        // tempo_fim - tempo_chegada
    
    EstadoProcesso estado;
} Processo;

// Protótipos de funções utilitárias do processo
Processo* criar_processo(int id, int tempo_chegada, int prioridade, int *rajadas, int num_rajadas);
void destruir_processo(Processo *p);

#endif // PROCESSO_H
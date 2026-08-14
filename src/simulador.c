#include <stdio.h>
#include <stdlib.h>
#include "../include/simulador.h"
#include "../include/estrutura_dados.h"
#include "../include/processo.h"
#include "../include/escalonador.h"

static void atualizar_tempo_espera_pronto(Fila* fila_prontos) {
    if (fila_prontos == NULL || fila_prontos->inicio == NULL) {
        return;
    }

    No* atual = fila_prontos->inicio;
    while (atual != NULL) {
        if (atual->processo != NULL && atual->processo->estado == ESTADO_PRONTO) {
            atual->processo->tempo_espera_pronto++;
        }
        atual = atual->prox;
    }
}

// Função principal que orquestra a simulação em tempo discreto
void executar_simulacao(Processo** lista_processos, int total_processos, int custo_troca_contexto, Algoritmo algoritmo) {
    int tempo_atual = 0;
    int processos_concluidos = 0;

    Fila* fila_prontos = criar_fila();
    Fila* fila_bloqueados = criar_fila();

    Processo* processo_atual = NULL;
    Processo* processo_anterior = NULL;

    while (processos_concluidos < total_processos) {
        // Chegada de novos processos
        for (int i = 0; i < total_processos; i++) {
            if (lista_processos[i] != NULL && lista_processos[i]->tempo_chegada == tempo_atual && lista_processos[i]->estado == ESTADO_NOVO) {
                lista_processos[i]->estado = ESTADO_PRONTO;
                enfileirar(fila_prontos, lista_processos[i]);
            }
        }

        // Processos bloqueados: decrementam a rajada de I/O em execução
        int num_bloqueados = fila_bloqueados->tamanho;
        for (int i = 0; i < num_bloqueados; i++) {
            Processo* p_bloqueado = desenfileirar(fila_bloqueados);

            if (p_bloqueado == NULL) {
                continue;
            }

            p_bloqueado->tempo_restante_rajada--;

            if (p_bloqueado->tempo_restante_rajada <= 0) {
                p_bloqueado->idx_rajada_atual++;

                if (p_bloqueado->idx_rajada_atual < p_bloqueado->num_rajadas) {
                    p_bloqueado->estado = ESTADO_PRONTO;
                    p_bloqueado->tempo_restante_rajada = p_bloqueado->rajadas[p_bloqueado->idx_rajada_atual];
                    enfileirar(fila_prontos, p_bloqueado);
                }
            } else {
                p_bloqueado->estado = ESTADO_BLOQUEADO;
                enfileirar(fila_bloqueados, p_bloqueado);
            }
        }

        // Atualiza o tempo de espera dos processos na fila de prontos
        atualizar_tempo_espera_pronto(fila_prontos);

        // Escolha do próximo processo para a CPU
        if (processo_atual == NULL && fila_prontos != NULL && fila_prontos->tamanho > 0) {
            if (algoritmo.escolher_proximo != NULL) {
                Processo* proximo = algoritmo.escolher_proximo(fila_prontos);
                if (proximo != NULL) {
                    if (proximo != processo_anterior) {
                        tempo_atual += custo_troca_contexto;
                    }
                    processo_anterior = processo_atual;
                    processo_atual = proximo;
                    processo_atual->estado = ESTADO_EXECUTANDO;
                }
            }
        }

        // Execução do processo na CPU por um tick
        if (processo_atual != NULL) {
            if (algoritmo.deve_preemptar != NULL && algoritmo.deve_preemptar(processo_atual)) {
                processo_atual->estado = ESTADO_PRONTO;
                enfileirar(fila_prontos, processo_atual);
                processo_anterior = processo_atual;
                processo_atual = NULL;
            } else {
                processo_atual->tempo_restante_rajada--;

                if (processo_atual->tempo_restante_rajada <= 0) {
                    if (processo_atual->idx_rajada_atual % 2 == 0) {
                        // Rajada de CPU terminou: avança para a próxima rajada de I/O, se houver
                        processo_atual->idx_rajada_atual++;

                        if (processo_atual->idx_rajada_atual < processo_atual->num_rajadas) {
                            processo_atual->estado = ESTADO_BLOQUEADO;
                            processo_atual->tempo_restante_rajada = processo_atual->rajadas[processo_atual->idx_rajada_atual];
                            enfileirar(fila_bloqueados, processo_atual);
                        } else {
                            processo_atual->estado = ESTADO_FINALIZADO;
                            processo_atual->tempo_fim = tempo_atual + 1;
                            processo_atual->turnaround_time = processo_atual->tempo_fim - processo_atual->tempo_chegada;
                            processos_concluidos++;
                        }
                    } else {
                        // Rajada de I/O terminou: volta para fila de prontos e avança para a próxima CPU
                        processo_atual->idx_rajada_atual++;

                        if (processo_atual->idx_rajada_atual < processo_atual->num_rajadas) {
                            processo_atual->tempo_restante_rajada = processo_atual->rajadas[processo_atual->idx_rajada_atual];
                            processo_atual->estado = ESTADO_PRONTO;
                            enfileirar(fila_prontos, processo_atual);
                        } else {
                            processo_atual->estado = ESTADO_FINALIZADO;
                            processo_atual->tempo_fim = tempo_atual + 1;
                            processo_atual->turnaround_time = processo_atual->tempo_fim - processo_atual->tempo_chegada;
                            processos_concluidos++;
                        }
                    }

                    processo_anterior = processo_atual;
                    processo_atual = NULL;
                }
            }
        }

        tempo_atual++;
    }

    liberar_fila(fila_prontos);
    liberar_fila(fila_bloqueados);
}

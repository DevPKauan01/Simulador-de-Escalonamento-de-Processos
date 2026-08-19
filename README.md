# 🖥 Simulador de Escalonamento de Processos

Simulador por tempo discreto desenvolvido em linguagem C para estudo, execução e análise comparativa de algoritmos de escalonamento de processos de Sistemas Operacionais. O projeto possibilita a avaliação estatística de desempenho sob condições estritamente reproduzíveis através do controle de seeds pseudoaleatórias.

## 📌 Sobre o Projeto

Este projeto foi desenvolvido como parte dos estudos de Sistemas Operacionais na **Universidade Federal do Cariri (UFCA)**. Ele visa analisar a relação de trade-off entre eficiência de processamento, retenção de overhead (trocas de contexto) e justiça no tratamento de tarefas por parte do sistema operacional. O simulador executa uma carga determinística de **1.000 processos por simulação** sob **4 cenários operacionais distintos**, repetindo cada configuração ao longo de 100 sementes (seeds), totalizando **1.600 execuções independentes** com **Intervalo de Confiança de 95% (\text{IC}_{95\%})**

## ⚙ Algoritmos Implementados

Os algoritmos compartilham uma interface modular em C (escolher_proximo e deve_preemptar):
- **FCFS (First-Come, First-Served):** Atendimento por ordem de chegada na fila de prontos; política não preemptiva.
- **Round Robin (RR):** Atendimento por ordem de chegada com preempção por tempo de quantum fixo (Q = 10 ticks).
- **Prioridade Não Preemptivo:** Seleciona o processo de maior prioridade (menor valor numérico). O processo executa até concluir sua rajada de CPU sem preempção.
- **PROPRIO (Prioridade Dinâmica, Aging e Quantum Adaptativo):** Algoritmo autoral projetado para mitigar starvation e otimizar overhead de alternância. Utiliza:
- **Aging (Envelhecimento):** \text{score}_i = P_i - (\alpha \times W_i), onde \alpha = 0,6 e W_i é o tempo de espera.
- **Quantum Dinâmico:** Q_i = \max(Q_{\min}, \min(Q_{\max}, Q_{\max} - (P_i - 1))),
variando entre 2 e 8 ticks proporcionalmente à prioridade do processo.

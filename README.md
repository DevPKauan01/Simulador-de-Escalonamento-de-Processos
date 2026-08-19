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
  - **Quantum Dinâmico:** Q_i = \max(Q_{\min}, \min(Q_{\max}, Q_{\max} - (P_i - 1))), variando entre 2 e 8 ticks proporcionalmente à prioridade do processo.

## 📊 Cenários de Carga de Trabalho

1. **Cenário 1 — Aleatório Equilibrado:** Rajadas moderadas e homogêneas de CPU e E/S (5 a 24 ticks).
2. **Cenário 2 — I/O-Bound:** Rajadas curtas de CPU (1 a 5 ticks) e operações longas de E/S (15 a 44 ticks).
3. **Cenário 3 — CPU-Bound:** Rajadas intensas de CPU (30 a 79 ticks) e raras operações
de E/S (1 a 5 ticks).
4. **Cenário 4 — Prioridades Desbalanceadas:** 80% dos processos possuem alta
prioridade (níveis 1 a 3) e 20% baixa prioridade (8 a 10).

## 📈 Métricas Avaliadas

- **Turnaround Médio (\overline{T}):** Tempo total transcorrido desde a chegada até a conclusão do processo.
- **Trocas de Contexto:** Quantidade de substituições de processo na CPU (C = 2 ticks de custo por troca).
- **Índice de Justiça de Jain (J_{slowdown}):** Medido sobre o slowdown individual dos processos para verificar a equidade na distribuição de recursos.

## 📁 Estrutura do Repositório

```
├── include/ # Arquivos de cabeçalho (.h)
├── src/ # Código-fonte principal do simulador em C
│ └── algoritmos/ # Implementação de cada política de escalonamento
├── scripts/ # Scripts em Python para processamento de estatísticas e gráficos
├── executar_experimentos.sh # Automagicamente roda a bateria de 1.600 simulações
├── resultados.csv 
├── resultados_simulacao.csv
├── grafico_1_turnaround.png
├── grafico_2_trocas_contexto.png
├── grafico_3_justica_jain.png
├── simulador.exe
├── README.md # Documentação do repositório 
└── Projeto_escalonamento_de_processos.pdf # Artigo do estudo
```

## 🚀 Como Compilar e Executar

### Pré-requisitos

- GCC (Compilador C)
- Python 3.8+ com as bibliotecas: pandas, matplotlib, seaborn, scipy

### 1. Clonar o Repositório

```
git clone
https://github.com/DevPKauan01/Simulador-de-Escalonamento-de-Processos.git
cd Simulador-de-Escalonamento-de-Processos
```

### 2. Instalar Dependências do Python

``` 
pip install pandas matplotlib seaborn scipy
```

### 3. Compilar o Simulador em C

``` 
gcc -Wall -Wextra -Iinclude src/*.c src/algoritmos/*.c -o simulador
```

### 4. Executar Bateria Completa de Experimentos

- Rode o script de automação para executar as 1.600 simulações e gerar a base resultados.csv:
```
chmod +x executar_experimentos.sh
./executar_experimentos.sh
```

### 5. Gerar Gráficos e Análise Estatística

``` 
python scripts/gerar_graficos.py
```

## 👥 Equipe e Autores

Trabalho desenvolvido por alunos da Universidade Federal do Cariri (UFCA):
- Heberthy Samir Oliveira Furtado de Souza — [@heberthysamir](github.com/heberthysamir)
- Pedro Kauan Cardoso da Silva — [@DevPKauan01](github.com/DevPKauan01)
- Joaquim Arthur Muniz Leite — [@joarthurr](github.com/joarthurr)
- José Welton de Sousa Melo — [@jweltu](github.com/jweltu)
- Ramona Vitória Clemente Cardoso — [@ramona-dev](github.com/ramona-dev)

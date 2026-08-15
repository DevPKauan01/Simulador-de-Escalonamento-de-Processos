#!/bin/bash
set -e

echo "Compiling the simulator..."
rm -f simulador.exe
gcc -Iinclude -Wall -O2 -o simulador.exe src/*.c algoritmos/*.c

OUTPUT_FILE="resultados_simulacao.csv"
echo "Algoritmo,Cenario,Seed,Turnaround,TrocasContexto,JainIndex" > $OUTPUT_FILE

ALGORITMOS=("FCFS" "RR" "PRIORIDADE" "PROPRIO")
CENARIOS=(1 2 3 4)
NUM_PROCESSOS=1000
TOTAL_RODADAS=100

CUSTO_TROCA=2
QUANTUM=10

echo "Starting experiments. This might take a moment..."

for cenario in "${CENARIOS[@]}"; do
    for alg in "${ALGORITMOS[@]}"; do
        echo "Running Scenario $cenario with Algorithm $alg..."
        for ((seed=1; seed<=TOTAL_RODADAS; seed++)); do
            ./simulador.exe $seed $cenario $alg $NUM_PROCESSOS $CUSTO_TROCA $QUANTUM >> $OUTPUT_FILE
        done
    done
done

EXPECTED_LINES=$(( (4 * 4 * 100) + 1 ))
ACTUAL_LINES=$(wc -l < $OUTPUT_FILE | tr -d ' ')

if [ "$ACTUAL_LINES" -eq "$EXPECTED_LINES" ]; then
    echo "Experiments completed successfully! Validated exactly $ACTUAL_LINES rows in $OUTPUT_FILE."
else
    echo "CRITICAL ERROR: Dataset incompleto/inválido. Esperado $EXPECTED_LINES linhas, encontrado $ACTUAL_LINES."
    exit 1
fi
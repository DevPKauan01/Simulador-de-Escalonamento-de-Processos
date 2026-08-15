import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import scipy.stats as st
import seaborn as sns

sns.set_theme(style="whitegrid")

def calculate_ci95(data):
    """Calcula a margem de erro do IC 95%."""
    n = len(data)
    if n <= 1: return 0
    sem = st.sem(data)
    return sem * st.t.ppf((1 + 0.95) / 2., n-1)

df = pd.read_csv('resultados_simulacao.csv')

cenario_map = {
    1: 'Equilibrado',
    2: 'I/O Bound',
    3: 'CPU Bound',
    4: 'Prioridades Desbalanceadas'
}
df['Cenario_Nome'] = df['Cenario'].map(cenario_map)

grupos = df.groupby(['Cenario_Nome', 'Algoritmo'])

tamanhos = grupos.size()
if not (tamanhos == 100).all():
    print("ERRO CRÍTICO METODOLÓGICO: Alguns grupos não têm 100 amostras independentes")
    import sys
    sys.exit(1)

grouped = grupos.agg(
    Turnaround_Mean=('Turnaround', 'mean'),
    Turnaround_CI=('Turnaround', calculate_ci95),
    TrocasContexto_Mean=('TrocasContexto', 'mean'),
    TrocasContexto_CI=('TrocasContexto', calculate_ci95),
    JainIndex_Mean=('JainIndex', 'mean'),
    JainIndex_CI=('JainIndex', calculate_ci95)
).reset_index()

def plot_metric(metric_name, ylabel, title, filename, is_percentage=False):
    cenarios = df['Cenario_Nome'].unique()
    algoritmos = df['Algoritmo'].unique()
    
    x = np.arange(len(cenarios))
    width = 0.2
    
    fig, ax = plt.subplots(figsize=(12, 6))
    
    for i, alg in enumerate(algoritmos):
        alg_data = grouped[grouped['Algoritmo'] == alg]
        alg_data = alg_data.set_index('Cenario_Nome').reindex(cenarios).reset_index()
        
        means = alg_data[f'{metric_name}_Mean']
        cis = alg_data[f'{metric_name}_CI']
        
        offset = (i - len(algoritmos)/2 + 0.5) * width
        ax.bar(x + offset, means, width, yerr=cis, label=alg, capsize=5, alpha=0.9)

    if is_percentage:
        ax.set_ylim(0, 110)
        
    ax.set_ylabel(ylabel)
    ax.set_title(title, fontsize=14, pad=15)
    ax.set_xticks(x)
    ax.set_xticklabels(cenarios)
    ax.legend(title='Algoritmo')
    
    plt.tight_layout()
    plt.savefig(filename, dpi=300)
    print(f"Gráfico salvo como: {filename}")
    plt.close()

plot_metric(
    metric_name='Turnaround',
    ylabel='Turnaround Médio (Ticks)',
    title='Comparação de Turnaround por Cenário (Média ± IC95%)',
    filename='grafico_1_turnaround.png'
)

plot_metric(
    metric_name='TrocasContexto',
    ylabel='Quantidade Média de Trocas',
    title='Custo Oculto: Trocas de Contexto por Cenário (Média ± IC95%)',
    filename='grafico_2_trocas_contexto.png'
)

plot_metric(
    metric_name='JainIndex',
    ylabel='Índice de Justiça de Jain (%)',
    title='Justiça dos Escalonadores (Slowdown) por Cenário (Média ± IC95%)',
    filename='grafico_3_justica_jain.png',
    is_percentage=True
)
CC=gcc
CFLAGS=-Iinclude -Wall -O2

SRC=src/main.c src/simulador.c src/estrutura_dados.c src/gerador.c
ALG=algoritmos/fcfs.c algoritmos/rr.c algoritmos/prioridade.c algoritmos/proprio.c
OBJ=$(SRC:.c=.o) $(ALG:.c=.o)

simulador: $(SRC) $(ALG)
	$(CC) $(CFLAGS) -o simulador $(SRC) $(ALG)

clean:
	rm -f simulador $(OBJ)

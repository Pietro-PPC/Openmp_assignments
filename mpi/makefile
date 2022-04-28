LDLIBS= -lm

all: knapsack

knapsack: knapsack.c
	mpicc knapsack.c -o knapsack $(LDLIBS)

hello: hello.c
	mpicc hello.c -lm -o hello

cabele: cod_cabele.c
	mpicc cod_cabele.c -o cabele

run: knapsack
	mpirun -np 5 ./knapsack

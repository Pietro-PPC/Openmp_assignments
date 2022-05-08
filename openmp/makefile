CFLAGS=-fopenmp -O3
PARFILE3=t1_knapsack3.c
PARFILE2=t1_knapsack2.c
SEQFILE=naive_knapsack.c

all: par2 par3 seq

par2: $(PARFILE2)
	gcc $(PARFILE2) -o par2 $(CFLAGS)

par3: $(PARFILE3)
	gcc $(PARFILE3) -o par3 $(CFLAGS)

seq: $(SEQFILE)
	gcc $(SEQFILE) -o seq $(CFLAGS)

debug:
	gcc $(PARFILE3) -o par3 $(CFLAGS) -g

purge: 
	rm -f par2 par3 seq


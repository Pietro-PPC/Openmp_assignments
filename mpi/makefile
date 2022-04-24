all: hello

hello: hello.c
	mpicc hello.c -o hello

run: hello
	mpirun --hostfile hosts -np 4 ./hello

run_local: hello
	mpirun -np 4 ./hello

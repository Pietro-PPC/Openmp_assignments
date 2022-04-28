#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// MPI_COMM_WORLD

// MPI_Comm_rank(MPI_Comm comm, int *rank) -> rank do processo atual
// MPI_Comm_size(MPI_Comm comm, int *size) -> total de processos no comunicador

// MPI_Send(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
// MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);
    // source: MPI_ANY_SRC ou rank
    // tag: MPI_ANY_TAG ou tag
    // status: MPI_STATUS_IGNORE ou status
// MPI_Get_count(MPI_Status *status, MPI_Datatype datatype, int *count)
// MPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status *status)

#define MAX 1000

void init_arr(int *a, int n){
    while(n)a[--n]=n;
}

void pack_unpack_test(){
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int W;
    int pack_buf[1];
    int buf_size = 1;
    if (rank == 0){
        int pos = 0;
        W = 3;
        MPI_Pack(&W, 1, MPI_INT, pack_buf, buf_size, &pos, MPI_COMM_WORLD);
    }
    MPI_Bcast(pack_buf, pos, MPI_PACKED, 0, MPI_COMM_WORLD);

    if (rank != 0){
        MPI_Unpack(pack_buf, buf_size, &pos, W, 1, MPI_INT, MPI_COMM_WORLD);
    }
}

void test(){
    int world_size, world_rank;
    int a[MAX], n = 10;

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank == 0) init_arr(a, n);

    int elems_per_proc = ceil ((double)n / (double)world_size);

    int *sub_arr = (int *) malloc(elems_per_proc*sizeof(int));
    MPI_Scatter(a, elems_per_proc, MPI_INT, sub_arr, elems_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    printf("world_rank: %d\n", world_rank);
    for (int i = 0; i < elems_per_proc && elems_per_proc*world_rank + i < n; ++i){
        printf("%d ", sub_arr[i]);
    }
    printf("\n");
}

int main(int argc, char **argv){
    MPI_Init(&argc, &argv);


    pack_unpack_test

    MPI_Finalize();
}
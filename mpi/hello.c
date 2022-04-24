#include <mpi.h>
#include <stdio.h>

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

int main(int argc, char **argv){
    MPI_Init(&argc, &argv);

    printf("Hello, world!\n");

    MPI_Finalize();
}
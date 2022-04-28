#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define INP_FILE "inputs/inp.in"

// A utility function that returns
// maximum of two integers
int max(int a, int b) {return (a > b) ? a : b;}


int** get_matrix(int rows, int columns) {   
    int **mat;
    int i;
    
    // for each line
    mat = (int**) calloc(rows, sizeof (int*));
    
    mat[0] = (int*) calloc(rows * columns, sizeof (int));

    // set up pointers to rows
    for (i = 1; i < rows; i++)
        mat[i] = mat[0] + i * columns;

    return mat;
}

void free_matrix(int** mat) {
    free(mat[0]);
    free(mat);
}


int knapsack(int MAXIMUM_CAPACITY, int wt[], int val[], int n)
{
    // Matrix-based solution
    int** V = get_matrix(n + 1, MAXIMUM_CAPACITY + 1);

    // V Stores, for each (1 + i, j), the best profit for a knapscak
    // of capacity `j` considering every item k such that (0 <= k < i)
    int i, j;

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int send_count = ceil((double)MAXIMUM_CAPACITY/(double)size); // quantos objetos mando pra cada vetor
    int *array_partition = malloc(send_count*sizeof(int));
    // evaluate item `i`
    printf("Processo %d\n", rank);
    for(i = 0; i < n; i++) {
        // Paralelizo esse cara
        // scatter o vetor atual pra todos os processos
        MPI_Scatter(V[1+i]+1, send_count, MPI_INT, array_partition, send_count, MPI_INT, 0, MPI_COMM_WORLD);
        int V_j;
        // for(j = 1; j <= MAXIMUM_CAPACITY; j++) {
        for (j = 0, V_j = rank*send_count + 1; j < send_count, V_j <= MAXIMUM_CAPACITY; ++j, ++V_j){
            if (i == 0){
                printf("%d %d\n", j, V_j);
            }
            if(wt[i] <= j) { // could put item in knapsack
                int previous_value = V[1 + i - 1][V_j];
                int replace_items = val[i] + V[1 + i - 1][V_j - wt[i]];

                // is it better to keep what we already got,
                // or is it better to swap whatever we have in the bag that weights up to `j`
                // and put item `i`?
                V[1 + i][V_j]= max(previous_value, replace_items);
            }
            else {
                // can't put item `i`
                V[1 + i][V_j] = V[1 + i - 1][V_j];
			}
        }
        // gather todo mundo junto depois
    }

    int retval = V[1 + n - 1][MAXIMUM_CAPACITY]; 
    
    free_matrix(V);
    
    return retval;
}

// Le a entrada no processo raiz e faz seu broadcast para todos os outros processos
void get_entry(int *n, int *W, int **val, int **wt){
    
    int i, pos;
    int size, rank;
    FILE *entry = NULL;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // printf("%d\n", rank);

    // Lê e faz broadcast da variável n
    if (rank == 0){
        entry = fopen (INP_FILE, "r");
        fscanf(entry, "%d", n);
    }
    MPI_Bcast(n, 2, MPI_INT, 0, MPI_COMM_WORLD);

    // printf();
    // Aloca buffer necessário para receber restante da entrada e vetores a receberem entrada
    int buf_size = sizeof(int) + 2 * (*n) *sizeof(int);
    char *pack_buf = malloc(buf_size);
    *val = (int*) calloc(*n, sizeof(int));
    *wt = (int*) calloc(*n, sizeof(int));

    // Processo root lê restante da entrada, empacota e faz broadcast
    if (rank == 0){
        fscanf(entry, "%d", W);
        for (i = 0; i < *n; ++i) {
            fscanf(entry, "%d %d", &((*val)[i]), &((*wt)[i])); 
        }
        pos = 0;
        MPI_Pack(W, 1, MPI_INT, pack_buf, buf_size, &pos, MPI_COMM_WORLD);
        MPI_Pack(*val, *n, MPI_INT, pack_buf, buf_size, &pos, MPI_COMM_WORLD);
        MPI_Pack(*wt, *n, MPI_INT, pack_buf, buf_size, &pos, MPI_COMM_WORLD);
    }
    MPI_Bcast(pack_buf, buf_size, MPI_PACKED, 0, MPI_COMM_WORLD);
    
    // Processos não root desempacotam entrada e assinalam variáveis
    if (rank != 0){
        pos = 0;
        MPI_Unpack(pack_buf, buf_size, &pos, W, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(pack_buf, buf_size, &pos, *val, *n, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(pack_buf, buf_size, &pos, *wt, *n, MPI_INT, MPI_COMM_WORLD);
    }

    free(pack_buf);
    if (entry) fclose(entry);
}

// Driver program to test above function
int main()
{
	int n, W;
    MPI_Init(NULL, NULL);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *val, *wt;
    int i, pos;

    get_entry(&n, &W, &val, &wt);

    printf("proc %d\n%d %d\n",rank, n, W);
    for (int i = 0; i < n; ++i) printf("%d %d\n", val[i], wt[i]);

    // printf("%d\n", knapsack(W, wt, val, n));
    free(val);
    free(wt);
    MPI_Finalize();
    return 0;
}

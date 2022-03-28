/* A Naive recursive implementation
of 0-1 Knapsack problem */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#ifndef NUM_THREADS
#define NUM_THREADS 8
#endif

const int level_print = 3;
const int level_paral = 3;
int *aux_arr_1, *aux_arr_2;

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }

int knapSack2(int W, int wt[], int val[], int n)
{
    if (n == 0 || W == 0)
        return 0;
 
    if (wt[n - 1] > W)
        return knapSack2(W, wt, val, n - 1);
 
    return max(
            knapSack2(W - wt[n - 1], wt, val, n - 1) + val[n - 1],
            knapSack2(W, wt, val, n - 1) 
        );
}

// Returns the maximum value that can be
// put in a knapsack of capacity W
int knapSack(int W, int wt[], int val[], int n, int free_thrds)
{
    struct timeval ini, fim;

    // Base Case
    if (n == 0 || W == 0)
        return 0;
 
    // If weight of the nth item is more than
    // Knapsack capacity W, then this item cannot
    // be included in the optimal solution
    if (wt[n - 1] > W)
        return knapSack(W, wt, val, n - 1, free_thrds);
 
    // Return the maximum of two cases:
    // (1) nth item included
    // (2) not included
    int value_1;
    int value_2;
    if (free_thrds){
        #pragma omp task shared(value_1)
        value_1 = knapSack(W, wt, val, n - 1, free_thrds-1);
        
        value_2 = knapSack2(W - wt[n - 1], wt, val, n - 1) + val[n - 1];
        #pragma omp taskwait
    }
    else {
        value_1 = knapSack2(W - wt[n - 1], wt, val, n - 1) + val[n - 1];
        value_2 = knapSack2(W, wt, val, n - 1);
    }
    return max(value_1, value_2);
}

void merge(int *a1, int *a2, int ini, int fim){
    int mid = ini + (fim - ini)/2;

    int il = ini, ir = mid, itaux;
    for (itaux = ini; il < mid && ir < fim; ++itaux){
        if (a1[il] > a1[ir]){
            aux_arr_1[itaux] = a1[il];
            aux_arr_2[itaux] = a2[il++];
        } else {
            aux_arr_1[itaux] = a1[ir];
            aux_arr_2[itaux] = a2[ir++];
        }
    }
    for (; il < mid; ++itaux, il++){
        aux_arr_1[itaux] = a1[il];
        aux_arr_2[itaux] = a2[il];
    }
    for (; ir < fim; ++itaux, ir++){
        aux_arr_1[itaux] = a1[ir];
        aux_arr_2[itaux] = a2[ir];
    }

    for (int i = ini; i < fim; ++i){
        a1[i] = aux_arr_1[i];
        a2[i] = aux_arr_2[i];
    }
}
void mergeSort(int *a1, int *a2, int ini, int fim){
    if (fim - ini == 1)
        return;
    
    int mid = ini + (fim - ini)/2;
    mergeSort(a1, a2, ini, mid);
    mergeSort(a1, a2, mid, fim);
    merge(a1, a2, ini, fim);
}
 
// Driver program to test above function
int main()
{
	int n, W;

	scanf("%d %d", &n, &W);
    aux_arr_2 = (int*) calloc(n, sizeof(int));
    aux_arr_1 = (int*) calloc(n, sizeof(int));
	int *val = (int*) calloc(n, sizeof(int));
	int *wt = (int*) calloc(n, sizeof(int));

	int i;
	for (i = 0; i < n; ++i) {
		scanf("%d %d", &(val[i]), &(wt[i])); 
	}

    // for (i = 0; i < n; ++i){
    //     printf("%d %d\n", val[i], wt[i]);
    // }

    mergeSort(wt, val, 0, n);

    // printf("oi sai do merge\n");
    // for (i = 0; i < n; ++i){
    //     printf("%d %d\n", val[i], wt[i]);
    // }

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        #pragma omp single
        {
            int free_thrds = omp_get_num_threads() - 1;
            printf("%d\n", knapSack(W, wt, val, n, free_thrds));
        }
    }
    return 0;
}

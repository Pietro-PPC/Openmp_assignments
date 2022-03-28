/* A Naive recursive implementation
of 0-1 Knapsack problem */
#include <stdio.h>
#include <stdlib.h>

#ifndef MAX_TASKS
#define MAX_TASKS 8
#endif

int num_tasks = 0;
 
// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// Returns the maximum value that can be
// put in a knapsack of capacity W
// wt: peso dos itens
// val: valor dos itens
// n: quantos itens
int knapSack(int W, int wt[], int val[], int n, int created)
{
    // Base Case
    // ***** Liberta Thread *****
    if (n == 0 || W == 0){
        if (created){
            #pragma omp critical
            num_tasks--;
        }
            
        return 0;
    }
 
    // If weight of the nth item is more than
    // Knapsack capacity W, then this item cannot
    // be included in the optimal solution

    // ***** Mantém Thread *****
    if (wt[n - 1] > W){
        int a = knapSack(W, wt, val, n - 1, 0);
        if (created){
            #pragma omp critical
            num_tasks--;
        }
        return a;
    }
 
    // Return the maximum of two cases:
    // (1) nth item included
    // (2) not included
    // ***** Cria nova Thread *****
    else{
        int b;
        if (num_tasks < MAX_TASKS){
            #pragma omp critical
            num_tasks++;
            #pragma omp task shared(b)
            b = knapSack(W, wt, val, n - 1, 1);
        } else {
            b = knapSack(W, wt, val, n - 1, 0);
        }
        int a = knapSack(W - wt[n - 1], wt, val, n - 1, 0) + val[n - 1];
        if (created){
            #pragma omp critical
            num_tasks--;
        }
        return max(a, b);
    }
}
 
// Driver program to test above function
int main()
{
	int n, W;

	scanf("%d %d", &n, &W);
	int *val = (int*) calloc(n, sizeof(int));
	int *wt = (int*) calloc(n, sizeof(int));

	int i;
	for (i = 0; i < n; ++i) {
		scanf("%d %d", &(val[i]), &(wt[i])); 
	}

    int a;
    #pragma omp parallel num_threads(8)
    {
        #pragma omp single
            a = knapSack(W, wt, val, n, 0);
    }
    printf("tasks %d\n", num_tasks);
    printf("%d\n", a);
    return 0;
}

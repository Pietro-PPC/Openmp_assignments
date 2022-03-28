/* A Naive recursive implementation
of 0-1 Knapsack problem */
#include <stdio.h>
#include <stdlib.h>
 
#ifndef NUM_THREADS
#define NUM_THREADS 8
#endif

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
 
// Returns the maximum value that can be
// put in a knapsack of capacity W
int knapSack(int W, int wt[], int val[], int n, int lvl)
{
    // Base Case
    if (n == 0 || W == 0)
        return 0;
 
    // If weight of the nth item is more than
    // Knapsack capacity W, then this item cannot
    // be included in the optimal solution
    if (wt[n - 1] > W)
        return knapSack(W, wt, val, n - 1, lvl);
 
    // Return the maximum of two cases:
    // (1) nth item included
    // (2) not included
    int value_1;
    int value_2;
    if (lvl < 1){
        #pragma omp parallel num_threads(2)
        {
            #pragma omp sections
            {
                #pragma omp section
                    value_1 = knapSack(W - wt[n - 1], wt, val, n - 1, lvl+1) + val[n - 1];
                #pragma omp section
                    value_2 = knapSack(W, wt, val, n - 1, lvl+1);
            }
        }
    }
    else {
        value_1 = knapSack(W - wt[n - 1], wt, val, n - 1, lvl+1) + val[n - 1];
        value_2 = knapSack(W, wt, val, n - 1, lvl+1);
    }

    return max(value_1, value_2);
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

    printf("%d", knapSack(W, wt, val, n, 0));
    return 0;
}

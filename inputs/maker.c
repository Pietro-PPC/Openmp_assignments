#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char * argv[]) {
  int n = atoi(argv[1]), W = atoi(argv[2]);
  // scanf("%d %d", &n, &W);
  printf("%d %d\n", n, W);

  srand(time(NULL));
  long int max_val = n/W;
  if (max_val <= 1) max_val = W;
  long int max_wt = (W/n)/2;
  if (max_wt <= 1) max_wt = W;
  for (int i = 0; i != n; i++) {
    printf("%d %d\n", rand() % max_val, rand() % max_wt);
  }
}

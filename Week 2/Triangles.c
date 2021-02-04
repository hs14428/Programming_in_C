#include <stdio.h>

int triEquation(int n);

int main(void)
{
   int i, n, t;

   printf("How many triangle numbers you wanna see?\n");
   if (scanf("%d", &n) != 1) {
      printf("Please only enter numbers.\n");
   }
   for (i = 1; i <= n; i++) {
      t = triEquation(i);
      if (i % 5 == 0) {
         printf("\n");
      }
      printf("%5d ", t);
   }
   return 0;
}

int triEquation(int n)
{
   int t;
   t = (n*(n + 1))/2;
   return t;
}

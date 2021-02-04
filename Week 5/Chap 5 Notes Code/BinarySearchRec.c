#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define NMBRS 100000

int binaryRec(int k, int *a, int l, int r);

int main(void)
{
   int i, n;
   int a[NMBRS];
   srand(time(NULL));

   for (i = 0; i < NMBRS; i++) {
      a[i] = 2 * i;
   }

   for (i = 0; i < 10 * NMBRS; i++) {
      n = binaryRec(a[rand()%NMBRS], a, 0, NMBRS - 1);
      assert(n >= 0);
   }
   return 0;
}

int binaryRec(int k, int *a, int l, int r)
{
   int m;

   if (l > r) {
      return -1;
   }

   m = (l + r)/2;

   if (k == a[m]) {
      return m;
   }
   else {
      if (k > a[m]) {
         return binaryRec(k, a, m + 1, r);
      }
      else {
         return binaryRec(k, a, m - 1, r);
      }
   }
}

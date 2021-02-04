#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

int binaryIter(int k, int *a, int l, int r);

int main(int argc, char* argv[])
{
   int i, n;
   int* a;
   srand(time(NULL));
   sscanf(argv[1], "%d", &n);
   a = calloc(sizeof(a[0]), n);
   for (i = 0; i < n; i++) {
      a[i] = 2 * i;
   }

   for (i = 0; i < 100000; i++) {
      assert(binaryIter(a[rand()%n], a, 0, n - 1) >= 0);
   }
   free(a);
   return 0;
}

int binaryIter(int k, int *a, int l, int r)
{
   int m;

   while (l <= r) {
      m = (l + r)/2;
      if (k == a[m]) {
         return m;
      }
      else {
         if (k > a[m]) {
            l = m + 1;
         }
         else {
            r = m - 1;
         }
      }
   }
   return -1;
}

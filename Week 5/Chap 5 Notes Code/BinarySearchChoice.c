#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

int binaryIter(int k, int *a, int l, int r);
int interp(int k, int *a, int l, int r);

int main(int argc, char* argv[])
{
   int i, n;
   int srch;
   int* a;
   srand(time(NULL));
   sscanf(argv[1], "%d", &n);
   sscanf(argv[2], "%d", &srch);
   a = calloc(sizeof(a[0]), n);
   for (i = 0; i < n; i++) {
      a[i] = 2 * i;
   }

   for (i = 0; i < 100000; i++) {
      if (srch == 0) {
         assert(binaryIter(a[rand()%n], a, 0, n - 1) >= 0);
      }
      if (srch == 1) {
         assert(interp(a[rand()%n], a, 0, n - 1) >= 0);
      }
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

int interp(int k, int *a, int l, int r)
{
   int m;
   double md;

   while (l <= r) {
      md = ((double)(k - a[l])/(double)(a[r] - a[l])
           * (double)(r-l)) + (double)(l);
      m = 0.5 + md;
      if (k == a[m]) {
         return m;
      }
      else {
         if (k > a[m]) {
            l = m + l;
         }
         else {
            r = m - 1;
         }
      }
   }
   return -1;
}

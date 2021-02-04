#include "common.h"

int closestmean(const int l[], const int n)
{
   int i, j;
   int s;
   int diff, closest, index;

   s = 0;
   for (i = 0; i < n; i++) {
      s += l[i];
   }
   s /= n;
/*   printf("%d\n", s);*/

   index = 0;
   closest = abs(s);
   for (j = n-1; j >= 0; j--) {
      diff = abs(s - l[j]);
/*      printf("l[%d] diff: %d\n", j, diff);*/
      if (diff <= closest) {
         closest = diff;
         index = j;
      }
   }
/*   printf("%d\n", index);
   printf("closest mean: %d\n", l[index]);*/
   return l[index];
}

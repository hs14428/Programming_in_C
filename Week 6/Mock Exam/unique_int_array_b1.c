#include <stdio.h>
#include <assert.h>
#include "common.h"

int numdiff(const int l[], const int n)
{
   int i, j;
   int dupe, uni;

   int *buffer = (int*)malloc(sizeof(int)*n);

   for (i = 0; i < n; i++) {
      buffer[i] = l[i];
   }

   for (i = 0; i < n; i++) {
      for (j = i+1; j < n; j++) {
         if (buffer[i] == buffer[j]) {
            buffer[j] = buffer[j] + 1;
         }
      }
   }
   uni = 0;
   for (i = 0; i < n; i++) {
      if (buffer[i] == l[i]) {
         uni++;
      }
   }

   free(buffer);
   printf("uni: %d\n", uni);
   return uni;
}

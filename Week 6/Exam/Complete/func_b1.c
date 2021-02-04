#include "common.h"

bool negativeedge(int a[][10], int h)
{
   int r, c;

   for (r = 1; r < h-1; r++) {
      for (c = 1; c < 9; c++) {
         if (a[r][c] < 0) {
            return false;
         }
      }
   }

   return true;
}

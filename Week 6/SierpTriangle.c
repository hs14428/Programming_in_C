#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neillsimplescreen.h"

#define ROWS 80
#define COLS 80

void triangle(char tri[ROWS][COLS], float r, float c, float rows, float cols);

int main(void)
{
   int r, c;
   char tri[ROWS][COLS];
   memset(tri, ' ', ROWS*COLS);

   triangle(tri, 0.0, 0.0, (float)ROWS, (float)COLS);

   for (r = 0; r < ROWS; r++) {
      for (c = 0; c < COLS; c++) {
         switch(tri[r][c]) {
            case ' ':
               neillfgcol(black);
               neillbgcol(black);
               printf(" ");
               break;
            default:
               neillfgcol(blue);
               neillbgcol(blue);
               printf("#");
               break;
         }
      }
      neillreset();
      printf("\n");
   }
   printf("\n");
   neillreset();
   return 0;
}

void triangle(char tri[ROWS][COLS], float r, float c, float rows, float cols)
{
   int ir, ic;

   if ((cols <= 1.) || (rows <= 1.)) {
      ic = (c + 0.5);
      ir = (r + 0.5);
      if ((ic >= 0.0) && (ic < COLS) && (ir >= 0.0) && (ir < ROWS)) {
         tri[ir][ic] = '#';
      }
      return;
   }

   triangle(tri, r+rows/4., c+cols/4., rows/2., cols/2.);

   triangle(tri, r+rows/2., c+rows/2., rows/2., cols/2.);

   triangle(tri, r+rows/2., c,         rows/2., cols/2.);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neillsimplescreen.h"

#define ROWS 81
#define COLS 81

void carpet(char carp[ROWS][COLS], float r, float c, float rows, float cols);

int main(void)
{
   int r, c;
   char carp[ROWS][COLS];
   memset(carp, ' ', ROWS*COLS);

   carpet(carp, 0.0, 0.0, (float)ROWS, (float)COLS);

   for (r = 0; r < ROWS; r++) {
      for (c = 0; c < COLS; c++) {
         switch(carp[r][c]) {
            case ' ':
               neillfgcol(black);
               neillbgcol(black);
               printf(" ");
               break;
            default:
               neillfgcol(blue);
               neillbgcol(blue);
               printf("*");
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

void carpet(char carp[ROWS][COLS], float r, float c, float rows, float cols)
{
   int ir, ic;

   if ((cols <= 1.) || (rows <= 1.)) {
      ic = (c + 0.5);
      ir = (r + 0.5);
      if ((ic >= 0.0) && (ic < COLS) && (ir >= 0.0) && (ir < ROWS)) {
         carp[ir][ic] = '*';
      }
      return;
   }

   carpet(carp, r,            c,            rows/3., cols/3.);
   carpet(carp, r,            c+cols/3.,    rows/3., cols/3.);
   carpet(carp, r,            c+2.*cols/3., rows/3., cols/3.);

   carpet(carp, r+rows/3.,    c,            rows/3., cols/3.);
   carpet(carp, r+rows/3.,    c+2.*cols/3., rows/3., cols/3.);

   carpet(carp, r+2.*rows/3., c,            rows/3., cols/3.);
   carpet(carp, r+2.*rows/3., c+cols/3.,    rows/3., cols/3.);
   carpet(carp, r+2.*rows/3., c+2.*cols/3., rows/3., cols/3.);
}

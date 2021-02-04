/*
   For a Boolean 2D array, a, of width 9 and height h, check that the
   array is vertically symmetric. This means that the first column is
   same as the last column, the second column is same as the second last
   column and so on.
*/

#include "common.h"

#define T true
#define F false
bool leftright(bool a[][9], int h);

int main(void)
{

   int x, y;
   bool a[1][9] = {{F,T,F,T,T,T,F,T,F}};
   bool c[9][9] = {{F}};
   bool d[2][9] = {{F,T,F,T,T,T,F,T,F},
                   {T,T,F,T,T,T,F,T,T}};

   assert(leftright(d, 2));
   assert(leftright(a, 1));
   assert(leftright(c, 9));

   c[5][4] = T;
   assert(leftright(c, 9));
   c[5][5] = T;
   assert(!leftright(c, 9));
   c[5][3] = T;
   assert(leftright(c, 9));
   for(y=0; y<9; y++){
      for(x=0; x<9; x++){
         if((y==x)||(y+x==8)){
            c[y][x] = T;
         }
      }
   }
   assert(leftright(c, 9));
     
   return F;
}

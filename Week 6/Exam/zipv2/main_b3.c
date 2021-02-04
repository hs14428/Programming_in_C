/*
   For a Boolean 2D array, a, of width 9 and height h, check whether
   any 2 rows are the same as each other or not.
*/

#include "common.h"

#define T true
#define F false
bool samerows(bool a[][9], int h);

int main(void)
{

   int i, x;
   bool a[1][9] = {{F,T,F,T,T,T,F,T,F}};
   bool b[3][9] = {{F}};
   bool c[3][9] = {{F}};
   bool d[2][9] = {{F,T,F,T,T,T,F,T,F},
                   {T,T,F,T,T,T,F,T,T}};

   assert(!samerows(a, 1));
   assert(samerows(b, 3));
   assert(samerows(c, 3));
   assert(!samerows(d, 2));

   for(i=0; i<100; i++){
      x = rand()%9;
      b[1][x] = true;
      b[0][x] = true;
      assert(samerows(b, 3));
      b[0][x] = false;
      assert(!samerows(b, 2));
      b[1][x] = false;
      assert(samerows(b, 3));
   }

   return F;
}

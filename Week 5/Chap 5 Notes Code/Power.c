#include <stdio.h>
#include <assert.h>

int powerIter(unsigned int a, unsigned int b);

int main(void)
{

   assert(power(0, 0) == 0);
   assert(power(0, 5) == 0);
   assert(power(5, 0) == 1);
   assert(power(5, 1) == 5);
   assert(power(9, 2) == 81);
   assert(power(2, 8) == 256);
   assert(power(2, 9) == 512);
   assert(power(2, 16) == 65536);


   int x = 2;
   int y = 16;

   printf("%d ^ %d = %d\n", x, y, power(x,y));
   return 0;
}
/*
int power(unsigned int a, unsigned int b)
{
   unsigned int i, tot;

   if (b == 1) {
      return a;
   }
   if (a == 0) {
      return 0;
   }
   if (b == 0) {
      return 1;
   }

   tot = a;
   for (i = 2; i <= b; i++) {
      tot = tot * a;
   }
   return tot;
}
*/
int power(unsigned int a, unsigned int b)
{
   unsigned int i, tot;

   if (b == 1) {
      return a;
   }
   if (a == 0) {
      return 0;
   }
   if (b == 0) {
      return 1;
   }

/* pow(a,4) = a*a*a*a = a*power(a,3);*/
/* even a; a*a*a*a*a*a = b*b if b == a*a*a */
   if (b%2 == 0) {
      t =  power(a, b/2);
      return t*t;
   }
   if (b%3 == 0) {
      t =  power(a, b/3);
      return t*t*t;
   }
   return a*power(a, b-1);
}

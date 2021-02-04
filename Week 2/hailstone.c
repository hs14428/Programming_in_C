#include <stdio.h>

#define MAXNUM 50000

int hail(int a);
int hailLong(int a);
int hailMax(int a);

int main(void)
{
   int x, cnt, length;
   int input;
   int max, large;

   for (x = 2; x < MAXNUM; ++x) {
      cnt = hailLong(x);
      if (cnt > input) {
         input = cnt;
         length = x;
      }
   }
   printf("Longest - Input number: %d\tLength: %d\n", input, length);

   for (x = 2; x < MAXNUM; ++x) {
      max = hailMax(x);
      if (max > large) {
         large = max;
         input = x;
      }
   }
   printf("Largest - Input number: %d\tLargest: %d\n", input, large);

}

int hail(int a)
{
   if ((a > 0 && a%2 == 0)) {
      return (a/2);
   }
   else if ((a > 0 && a%2 !=0)) {
      return ((3*a)+1);
   }
}

int hailLong(int a)
{
   int cnt = 1;

   while (a != 1) {
      a = hail(a);
      cnt++;
   }
   return cnt;
}

int hailMax(int a)
{
   int max = 1;

   while (a != 1) {
      a = hail(a);
      if (a > max) {
         max = a;
      }
   }
   return max;
}

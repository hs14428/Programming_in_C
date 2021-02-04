#include <stdio.h>
#include <math.h>

int length(int a, int b, int c);

int main(void)
{
   int x, y, z;

   printf("Please input 3 triangle side lengths: \n");
   scanf("%d%d%d", &x, &y, &z);
   while(x > 0 && y > 0 && z > 0) {
      if (x < 0 || y < 0 || z < 0) {
         printf("Negative numbers do not work, start again.\n");
      }
      else {
         length(x, y, z);
         printf("Please enter some new lengths: \n");
         scanf("%d%d%d", &x, &y, &z);
      }
   }
   printf("Negative numbers do not work, start again.\n");
   return 0;
}

int length(int a, int b, int c)
{

   if (a == b && b == c) {
      printf("This is an equilateral triangle\n");
   }
   /*else if (a == b && c !=a)*/
   else if (a == b || b == c || c == a) {
      printf("This is an isosceles triangle\n");
   }
   else if (a != b && a != c && b != c) {
      printf("This is a scalene triangle\n");
   }
   else if (c > a && c > b && (c == sqrt(a*a + b*b))) {
      printf("This is a right angled triangle");
   }
   return 0;
}

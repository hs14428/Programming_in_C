#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

typedef enum bool {false, true} bool;

bool isPrime(int num);

int main(void)
{
   bool test;

   isPrime(2);

}

bool isPrime(int num)
{
   int i = 2;

   if (num == 1) {
      return false;
   }

   while (i <= num/2) {
      if (num%i == 0) {
         printf("False\n");
         return false;
      }
      else {
         i++;
      }
   }
   printf("True\n");
   return true;
}

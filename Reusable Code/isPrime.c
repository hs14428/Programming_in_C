#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
typedef enum bool {false, true} bool;

bool isPrime(int n);
int sieve(int n);

int main(void)
{
   bool test;
   int test2;

   test = isPrime(7);
   test2 = sieve(11*2);
   printf("%d\n", test2);

}

bool isPrime(int n)
{
   int f;
   if (n < 2) {
      return false;
   }
   if (n == 2) {
      return true;
   }
   if ((n%2) == 0) {
      return false;
   }
   for (f = 3; f*f <= n; f += 2) {
      if ((n%f) == 0) {
         return false;
      }
   }
   return true;
}

int sieve(int n)
{
   int i, p;
   bool *prime = malloc(n * sizeof(bool));
   for (i = 0; i <= n; i++) {
      prime[i] = true;
   }
   for (p = 2; p*p < n; p++) {
      if (prime[p] == true) {
         for (i = p*p; i<=n; i +=p) {
            prime[i] = false;
         }
      }
   }
   for (p = n; p >= 2; p--) {
      if (prime[p]) {
         return p;
      }
   }
   return 0;
}

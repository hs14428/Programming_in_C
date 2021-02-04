#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

typedef enum bool {false, true} bool;

#define MAXLEN 100

int primeFact(int primes[MAXLEN], int num);
void fillPrimes(int primes[MAXLEN], int maxLen);
int sumArray(int a[MAXLEN], int maxLen);
bool isPrime(int num);

void test();

int main(int argc, char *argv[MAXLEN])
{
   int maxLen;
   int num;
   int primes[MAXLEN] = {0};

   test();
   sscanf(argv[1], "%d", &num);
   maxLen = MAXLEN;
   fillPrimes(primes, maxLen);
   primeFact(primes, num);

   return 0;

}

int primeFact(int primes[MAXLEN], int num)
{
   int i, j, k, iter, rem, maxPrime;
   int product[MAXLEN] = {0};
   int count[MAXLEN] = {0};

   i = 0;
   j = 0;
   iter = num;

   while ((isPrime(iter) != true) || (iter == num)) {
      rem = 0;
      while (rem == 0) {
         rem = iter%primes[i];
         if (rem == 0) {
            iter /= primes[i];
            product[j] = primes[i];
            j++;
         }
      }
      i++;
   }
   product[j] = iter;

   printf("Prime factorization of %d: ", num);
   for (k = 0; k <= j; k++){
      printf("%d ", product[k]);
   }

   maxPrime = product[k-1];
   countFact(product, count, k, maxPrime);
   printf("Prime factorization of %d (shorthand): ", num);
   for (k = 0; k <= j; k++) {
      if (product[k+1] != product[k]) {
         printf("%d^%d ", product[k], count[product[k]]);
      }
   }
   printf("\n");
}

void countFact(int product[MAXLEN], int count[MAXLEN], int maxCell, int maxPrime)
{
   int i, j;

   for (i = 0; i < maxCell; i++) {
      for (j = 1; j <= maxPrime; j++) {
         if (product[i] == j) {
            count[j]++;
         }
      }
   }
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

void fillPrimes(int primes[MAXLEN], int maxLen)
{
   int i, num, cnt;
   bool prime;

   for (num = 2, cnt = 0; cnt < maxLen; num++) {
      prime = true;
      for (i = 2; i < num; i++) {
         if (num%i == 0) {
            prime = false;
         }
      }
      if (prime == 1) {
         primes[cnt] = num;
         cnt++;
      }
   }
}

int sumArray(int a[MAXLEN], int maxLen)
{
   int i, sum = 0;

   for (i = 0; i < maxLen; i++) {
      sum += a[i];
   }
   return sum;
}

void test()
{
   int i, maxLen;
   int j;
   int primes[MAXLEN] = {0};

   fillPrimes(primes, 10);
   assert(sumArray(primes, 5) == 2+3+5+7+11);
   assert(primes[0] == 2);
   assert(primes[3] == 7);
   assert(primes[9] == 29);

   assert(isPrime(3));
   assert(isPrime(3));
   assert(isPrime(19));
   assert(isPrime(269));
   assert(!isPrime(4));
   assert(!isPrime(16));
   assert(!isPrime(32));
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

typedef enum bool {false, true} bool;

#define MAXLEN 100

int primeFact(int primes[MAXLEN], int maxNum, float num);
void fillPrimes(int primes[MAXLEN], int maxNum);
int sumArray(int a[MAXLEN], int maxNum);
bool intCheck(float num);
bool isPrime(int num);

void test();

int main(int argc, char *argv[MAXLEN])
{
   int i, maxNum;
   float num;
   int primes[MAXLEN] = {0};

   test();
   sscanf(argv[1], "%f", &num);
   maxNum = MAXLEN;
   fillPrimes(primes, maxNum);
   primeFact(primes, maxNum, num);

   return 0;

}

int primeFact(int primes[MAXLEN], int maxNum, float num)
{
   int i, j;
   float iter, sum;
   int product[MAXLEN] = {0};

   i = 0;
   j = 0;
   sum = 1;
   iter = num;
   while (!isPrime((int)iter) || iter == num) {
      printf("Prime no: %d\n", primes[i]);
      while (intCheck(iter) && !isPrime((int)iter)) {
         printf("Iter[%d]: %.2f\n", i, iter);
         iter /= primes[i];
         if (intCheck(iter)) {
            product[j] = primes[i];
            sum *= primes[i];
            printf("Sum[%d]: %.2f\n", i, sum);
            j++;
         }
      }
      if (!intCheck(iter)) {
         iter *= primes[i];
         i++;
      }
   }
   product[j] = iter;
   sum *= iter;
   printf("Sum[%d]: %.2f\n", i, sum);
   printf("Product: ");
   for (i = 0; i <= j; i++){
      printf("%d, ", product[i]);
   }

}


bool intCheck(float num)
{
   if (num - (int)num == 0) {
      return true;
   }
   return false;
}

bool isPrime(int num)
{
   int i = 2;

   if (num == 1) {
      return false;
   }

   while (i <= num/2) {
      if (num%i == 2) {
         return false;
      }
      else {
         i++;
      }
   }
   return true;
}

void fillPrimes(int primes[MAXLEN], int maxNum)
{
   int i, num, cnt;
   bool prime;

   for (num = 2, cnt = 0; cnt < maxNum; num++) {
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

int sumArray(int a[MAXLEN], int maxNum)
{
   int i, sum = 0;

   for (i = 0; i < maxNum; i++) {
      sum += a[i];
   }
   return sum;
}

void test()
{
   int i, maxNum;
   float j;
   int primes[MAXLEN] = {0};

   fillPrimes(primes, 10);
   assert(sumArray(primes, 5) == 2+3+5+7+11);
   assert(primes[0] == 2);
   assert(primes[3] == 7);
   assert(primes[9] == 29);

   assert(intCheck(15.0));
   assert(!intCheck(15.5));
}

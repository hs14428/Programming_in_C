#include <stdio.h>

#define N 5

long sum_two_smallest_numbers(int n, const int numbers[N]);

int main(void)
{
   const int numbers[N] = {19, 5, 42, 2, 77};

   sum_two_smallest_numbers(5, numbers);

   return 0;
}

long sum_two_smallest_numbers(int n, const int numbers[N])
{
   int i, tmp, changes;
   long sum;
   int buffer[N];

   for (i = 0; i < n; i++) {
      buffer[i] = numbers[i];
   }

   do {
      changes = 0;
      for (i = 0; i < n-1; i++) {
         if (buffer[i] > buffer[i+1]) {
            tmp = buffer[i];
            buffer[i] = buffer[i+1];
            buffer[i+1] = tmp;
            changes++;
         }
      }
   } while (changes != 0);

   sum = buffer[0] + buffer[1];
   printf("sum: %ld", sum);
   return sum;
}

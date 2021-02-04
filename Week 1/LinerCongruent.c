#include <stdio.h>

#define A 7
#define C 5
#define M 11
#define LOOPS 1000

/*
int main(void)
{
   int seed = 0;
   int i;

   for(i = 0; i < LOOPS; i++) {
      seed = ((A * seed) + C) % M;
      printf("%d\n", seed);
   }
   return 0;
}
*/

int main(void)
{
   int seed = 0;
   int i, cnt = 0;

   do {
      seed = ((A * seed) + C) % M;
      printf("%d\n", seed);
      ++cnt;
   } while(seed != 0);

   printf("The period is: %d\n", cnt);
}

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAXNUM 1000
#define MAXGUESS 10
#define KILLGAME 11

int random(void);
int highLow(void);
void test(void);

int main(void)
{
   test();
   printf("Please guess a random number (<1000): \n");
   return highLow();
}

int highLow(void)
{
   int g, a, rand, left;
   rand = random();

   for (a = 1; a <= MAXGUESS; a++) {
      scanf("%d", &g);
      left = MAXGUESS - a;
      if (g == rand) {
         printf("\nCongrats, you won!\n");
         a = KILLGAME;
      }
      else if (g > rand) {
         printf("\nUnlucky, too high; please guess again. (%d guesses left)\n", left);
      }
      else if (g < rand) {
         printf("\nUnlucky, too low; please guess again. (%d guesses left)\n", left);
      }
   }
   return 0;
}

int random(void)
{
   return (rand() % (MAXNUM + 1));
}

void test(void)
{
   assert(random() <= MAXNUM);
   assert(random() >= 0);
}

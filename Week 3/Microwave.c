#include <stdio.h>
#include <assert.h>

#define N 3
#define TIMESHORT 10
#define TIMEMED 60
#define TIMELONG 600

int buttonpress(int time_in);
void test(void);

int main(void)
{
   test();
   int t, presses;

   printf("Type time required (in seconds):\n");

   if (scanf("%d", &t) != 1) {
      printf("Only enter numbers.\n)");
   }
   presses = buttonpress(t);
   printf("Presses required %d", presses);
}

int buttonpress(int time_in)
{
   int a[N] = {TIMESHORT, TIMEMED, TIMELONG};
   int presses = 0;

   if (time_in / a[2] >= 1) {
      presses = time_in / a[2];
      time_in = time_in % a[2];
   }
   if (time_in / a[1] >= 1) {
      presses = presses + (time_in / a[1]);
      time_in = time_in % a[1];
   }
   if (time_in / a[0] >= 1) {
      presses = presses + (time_in / a[0]);
      time_in = time_in % a[0];
   }
   if (time_in != 0) {
      presses++;
   }
   return presses;
}

void test(void)
{
   assert(buttonpress(25) == 3);
   assert(buttonpress(705) == 7);
   assert(buttonpress(5) == 1);
}

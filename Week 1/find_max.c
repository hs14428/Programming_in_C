#include <stdio.h>

int main(void)
{
   int num;
   int j;
   float i;
   float max;

   printf("How many numbers do you wish to enter?");
   if (scanf("%d", &num) !=1) {
      printf("Please only enter numbers.\n");
      return 0;
   }

   printf("Enter %d real numbers: ", num);

   if (scanf("%f", &max) !=1) {
      printf("Please only enter numbers.\n");
      return 0;
   }

   for(j = 0; j < num-1; ++j) {
      if (scanf("%f", &i) !=1) {
         printf("Please only enter numbers.\n");
         return 0;
      }
      if (i > max) {
         max = i;
      }
   }

   printf("Max value: %.3f\n", max);
   return 0;
}

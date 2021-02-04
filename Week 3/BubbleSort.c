#include <stdio.h>

void bubble_sort(int b[], int s);

int main(void)
{
   int i;
   int b[10] = {1, 10, 7, 4, 5, 6, 3, 8, 9 , 2};

   for (i = 0; i < 10; i++) {
   printf("%d ", b[i]);
   }
   printf("\n");

   bubble_sort(b, 10);

   for (i = 0; i < 10; i++) {
   printf("%d ", b[i]);
   }
   return 0;
}

void bubble_sort(int b[], int s)
{
   int i, tmp;
   int changes;

   do {
      changes = 0;
      for (i = 0; i < s-1; i++) {
         if (b[i] > b[i + 1]) {
            tmp = b[i];
            b[i] = b[i+1];
            b[i+1] = tmp;
            changes++;
         }
      }
   } while (changes != 0);
}

#include <stdio.h>

int main(void)
{
   FILE *file;
   int i, j, k;
   int array[200] = {0};

   file = fopen("Expense.txt", "r");

   for (i = 0; i < 200; i++) {
      fscanf(file, "%d", &array[i]);
      printf("%d\n", array[i]);
   }

   for (i = 0; i < 200; i++) {
      for (j = 1; j < 200; j++) {
         for (k = j+1   ; k < 200; k++) {
            if ((array[i] + array[j] + array[k]) == 2020) {
               printf("%d + %d + %d = 2020\n", array[i], array[j], array[k]);
               return 0;
            }
         }
      }
   }
   fclose(file);
   return 0;
}

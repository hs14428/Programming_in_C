#include <stdio.h>
#include <stdlib.h>

int randomList(int[], int s);

int main(void)
{
   int i, songs;
   int *array;

   printf("How many songs do you want to shuffle?:\n");
   scanf("%d", &songs);
   array = (int*)malloc(sizeof((int*)songs));
   randomList(array, songs);

   for (i = 0; i < songs-1; i++) {
      printf("%d ", array[i]);
   }
   free(array);
   return 0;
}

int randomList(int a[], int s)
{
   int i;
   for (i = 0; i < s; i++) {
      a[i] = (rand() % (s + 1));
   }
   return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef enum bool {false, true} bool;

int randomList(int a[], int s);
/* void bubble_sort(int b[], int s); */
void randomize(int a[], int s);
void swap(int *q, int *p);

int main(void)
{
   int i, songs;
   int *array;

   printf("How many songs do you want to shuffle?:\n");
   scanf("%d", &songs);
   array = (int*)malloc(sizeof((int*)songs));

   randomList(array, songs);
   for (i = 0; i < songs; i++) {
      printf("%d ", array[i]);
   }
   printf("\n");

/* bubble_sort(array, songs); */
   randomize(array, songs);
   for (i = 0; i < songs; i++) {
      printf("%d ", array[i]);
   }

   free(array);
   return 0;
}

int randomList(int a[], int s)
{
   int i;
   for (i = 0; i < s; i++) {
      a[i] = i;
   }
   return 0;
}

void randomize(int b[], int s)
{
   int i, j;

   for (i = 0; i < s; i++) {
      j = (rand() % s);
      printf("i:%d  j:%d\n", b[i], b[j]);
      swap(&b[i], &b[j]);
      printf("i:%d  j:%d\n", b[i], b[j]);
   }
}

void swap(int *q, int *p)
{
   int tmp;
   tmp = *p;
   *p = *q;
   *q = tmp;
}

/* void randomize(int b[], int s)
{
   int i, j, count = 0;
   bool check = true;

   while (count < s) {
      for (i = 0; i < s; i++) {
         j = (rand() % s + 1);
         if (b[i] == b[j]) {
            check = false;
         }
      }
      if (check == true)
      {
         count +=1;
         swap(&b[i], &b[j]);
      }
   }
}
*/

/* void bubble_sort(int b[], int s)
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
}*/

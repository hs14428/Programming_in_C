#include <stdio.h>
#include <stdlib.h>

#define ARRAYSIZE 34
#define INDENT 20

void printPattern(int input);
void printArray(int a[]);
void randomNumber(int a[]);
void rules(int a[], int b[], int j);
void copyArray(int a[], int b[]);

int main(void)
{
   int input;

   printf("Enter 1 for a random first line, or 0 for"
          " a structured first line: ");

   if (scanf("%d", &input) != 1) {
      printf("\nPlease enter only numbers.\n");
   }

   printPattern(input);
   return 0;
}

void printPattern(int input)
{
   int array1[ARRAYSIZE] = {0};
   int array2[ARRAYSIZE] = {0};
   int i, j;

   if (input == 1) {
      randomNumber(array1);
   }
   else if (input == 0) {
      array1[ARRAYSIZE - INDENT] = 1;
   }

   printArray(array1);
   printf("\n");

   for (i = 0; i < ARRAYSIZE - INDENT; i++) {
      /*Set left hand print limit/border/padding*/
      printf("%d", array2[0]);

      for (j = 1; j < ARRAYSIZE - 1; j++) {
         rules(array1, array2, j);
         printf("%d", array2[j]);
      }
      /*Set right hand print limit/border/padding*/
      printf("%d\n", array2[ARRAYSIZE - 1]);
      copyArray(array1, array2);
   }
}

void copyArray(int a[], int b[])
{
   int i;
   for (i = 0; i < ARRAYSIZE; i++) {
      a[i] = b[i];
   }
}

void randomNumber(int a[])
{
  int i;
  double randomNumber;
  for (i = 0; i < ARRAYSIZE; i++) {
     randomNumber = ((double)rand() / (double)(RAND_MAX) * (1 + 1));
     a[i] = randomNumber;
  }
}

void printArray(int a[])
{
   int i;
   for (i = 0; i < ARRAYSIZE; i++) {
      printf("%d", a[i]);
   }
}

void rules(int a[], int b[], int j)
{
   if (a[j] == 1 && a[j-1] == 1 && a[j+1] == 1) {
      b[j] = 0;
   }
   else if (a[j] == 0 && a[j-1] == 1 && a[j+1] == 0) {
      b[j] = 0;
   }
   else if (a[j] == 0 && a[j-1] == 0 && a[j+1] == 0) {
      b[j] = 0;
   }
   else {
      b[j] = 1;
   }
}

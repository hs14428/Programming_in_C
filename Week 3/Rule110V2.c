#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define CELL 34

void automaton               (int userInput);
void copyArray            (int a[], int b[]);
void sendOneOrZero (int a[], int b[], int j);
void printFirstArray               (int a[]);
void randomNumberGenerator         (int a[]);
void printFirstAndLastInt (int a[], int b[]);

int main(void){

    int userInput;
    srand((unsigned int) time(NULL));

    printf("%s", "Enter 1 for a Random First Line"
                 "or 0 for a Structured First Line: " );

    if (scanf("%d", &userInput) == 1){

      printf("\nYour pattern: \n");
      automaton (userInput);
    }
return 0;
}

void automaton (int userInput){

    int A [CELL] = {0};
    int B [CELL] = {0};
    int i, j;

   if (userInput == 1){
    randomNumberGenerator (A);
   }
   else if (userInput == 0){
     A[CELL-4] = 1;
   }

      printFirstArray (A);
      printf("\n");

         for (i = 0; i < CELL-4; i++){
            /*printFirstAndLastInt(A,B);*/
            printf("%d", B[0]);

            for (j = 1; j < CELL-1; j++){
               sendOneOrZero(A, B, j);
               printf("%d", B[j]);
            }
            printf("%d\n", B[CELL-1]);
            /*printf("\n");*/
            copyArray(A, B);
         }
}

void copyArray (int a[], int b[]){

   int i;
      for (i = 0; i < CELL; i++){
         a[i] = b[i];
  }
}

void sendOneOrZero (int a[], int b[], int j){

  if (a[j] == 0){
    if ((a[j-1] == 0) && (a[j+1] == 0)){
      b[j]=0;
    }  else if ((a[j-1] == 1) && (a[j+1] == 0)){
         b[j] = 0;
    }    else{
           b[j] = 1;
    }
    }
  else if (a[j] == 1){
    if((a[j-1] == 1) && (a[j+1] == 1)){
      b[j] = 0;
    }   else{
          b[j] = 1;
    }
  }
}

void printFirstArray (int a[]){

   int i;
      for (i = 0; i < CELL; i++){
         printf("%d", a[i]);
}
}

void printFirstAndLastInt (int a[], int b[]){

  if (a[0] == 0){
    b[0] = 0;

  } else if (a[CELL-1] == 0){
    b[CELL-1] = 0;
  }
  if (a[0] == 1){
    b[0] = 1;

  } else if (a[CELL-1] == 1){
      b[CELL-1] = 1;
    }
}

void randomNumberGenerator (int a[]){

  int i;
  double randomNumber;
    for (i = 0; i < CELL; i++){
      randomNumber = ((double)rand() / (double)(RAND_MAX)) * (1 + 1);
      a[i] = randomNumber;
    }
  }

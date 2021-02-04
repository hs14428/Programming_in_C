#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define ITER 5000000
#define MAXSCORE 20
#define MINSCORE 1

void initBoard(int dartBoard[MAXSCORE]);
void randSwap (int dartBoard[MAXSCORE]);
int printBoard(int dartBoard[MAXSCORE]);
int lowBoard(int low, int temp);
int cost(int dartBoard[MAXSCORE]);
int inbounds(int max, int pos);

int main()
{
   long i;
   int low, temp;
   int dartBoard[MAXSCORE] = {0};

   srand((unsigned)time(NULL));

   initBoard(dartBoard);
   low = cost(dartBoard);
   for (i = 0; i < ITER; i++) {
      randSwap(dartBoard);
      temp = cost(dartBoard);
      low = lowBoard(low, temp);
   }

   printf("Total = %d: ", low);
   printBoard(dartBoard);

   return 0;
}

void initBoard(int dartBoard[MAXSCORE])
{
   int i;
   int n = 0;

   for (i = 0; i < MAXSCORE; i++) {
      dartBoard[i] = 1 + n;
      n++;
   }
}

void randSwap(int dartBoard[MAXSCORE])
{
   int p1, p2, tmp;

   p1 = rand()%MAXSCORE;
/*   printf("p1:%d\n", p1);*/
   p2 = rand()%MAXSCORE;
   /*printf("p2:%d\n", p2);*/

   tmp = dartBoard[p1];
   dartBoard[p1] = dartBoard[p2];
   dartBoard[p2] = tmp;
}

int lowBoard(int low, int temp)
{
   if (temp < low) {
      low = temp;
   }
   return low;
}

int printBoard(int dartBoard[MAXSCORE])
{
   int i;

   for (i = 0; i < MAXSCORE; i++) {
      printf("%d ", dartBoard[i]);
   }
   printf("\n");
   return 0;
}

int cost(int dartBoard[MAXSCORE])
{
   int n, tot;
   int p1, p2, p3;

   tot = 0;
   for (n = 0; n < MAXSCORE; n++) {
      p1 = inbounds(MAXSCORE, n);
      p2 = inbounds(MAXSCORE, n + 1);
      p3 = inbounds(MAXSCORE, n + 2);
      tot += pow((double)(dartBoard[p1]+dartBoard[p2]+dartBoard[p3]), (double)2);
   }
   return tot;
}

int inbounds(int max, int pos)
{
   if (pos >= max) {
      return abs(max - pos);
   }
   else {
      return pos;
   }
}

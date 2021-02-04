#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "neillsimplescreen.h"

#define SIZE 3
#define BLANK '\0'

typedef enum bool {false, true} bool;


bool valid(char phone[SIZE][SIZE], int x, int y);
int possPath(char phone[SIZE][SIZE], int x, int y, int len);
void printPhone(char phone[SIZE][SIZE]);

int main(void)
{
   int n;
   char phone[SIZE][SIZE] = {{BLANK}};

   possPath(phone, 0, 0, SIZE*SIZE);

   return 0;
}

bool valid(char phone[SIZE][SIZE], int x, int y)
{
   if ((x < 0) || (y < 0) || (x >= SIZE) || (y >= SIZE)) {
      return false;
   }
   if (phone[y][x] != BLANK) {
      return false;
   }

   return true;
}

int possPath(char phone[SIZE][SIZE], int x, int y, int len)
{
   static int c = 0;
   if (len == 1) {
      phone[y][x] = 'x';
      printPhone(phone);
      phone[y][x] = BLANK;
      printf("%d\n", ++c);
      return 1;
   }
   if (valid(phone, x-1, y)) {
      phone[y][x] = '<';
      possPath(phone, x-1, y, len-1);
      phone[y][x] = BLANK;
   }
   if (valid(phone, x+1, y)) {
      phone[y][x] = '>';
      possPath(phone, x+1, y, len-1);
      phone[y][x] = BLANK;
   }
   if (valid(phone, x, y-1)) {
      phone[y][x] = '^';
      possPath(phone, x, y-1, len-1);
      phone[y][x] = BLANK;
   }
   if (valid(phone, x, y+1)) {
      phone[y][x] = 'V';
      possPath(phone, x, y+1, len-1);
      phone[y][x] = BLANK;
   }

   return 0;
}

void printPhone(char phone[SIZE][SIZE])
{
   int i, j;
   char bar[SIZE*2+2];

   neillfgcol(white);
   memset(bar, '-', SIZE*2+1);
   bar[SIZE*2+1] = '\0';
   for (j = 0; j < SIZE; j++) {
      neillfgcol(white);
      printf("%s\n", bar);
      for (i = 0; i < SIZE; i++) {
         neillfgcol(white);
         printf("|");
         neillfgcol(red);
         printf("%c", phone[j][i]);
      }
      neillfgcol(white);
      printf("|\n");
   }
   printf("%s\n", bar);
}

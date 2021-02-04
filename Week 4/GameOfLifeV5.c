#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "neillsimplescreen.h"

#define BOARDCOLS 150
#define BOARDROWS 30
#define STRMAX 1000
#define START 2
#define BEHIND -1
#define AHEAD 1
#define DELAY 0.5

typedef enum bool {false, true} bool;

bool survival(int aliveCount);
bool birth(int aliveCount);
bool inbounds(int currentRow, int currentCol);
int countAlive(int board[BOARDROWS][BOARDCOLS], int currentRow, int currentCol);
void fillBoard(int board[BOARDROWS][BOARDCOLS], int newBoard[BOARDROWS][BOARDCOLS]);
int printBoard(int board[BOARDROWS][BOARDCOLS]);
int importFile(int board[BOARDROWS][BOARDCOLS], const char *filename);
void test();

int main(int argc, char *argv[])
{
   /*test();*/
   int i, iter, len;
   int board[BOARDROWS][BOARDCOLS] = {{0}};
   int newBoard[BOARDROWS][BOARDCOLS] = {{0}};
   char buffer[BOARDROWS][BOARDCOLS] = {{0}};
   int coord[BOARDROWS][BOARDCOLS] =  {{0}};

   iter = atoi(argv[2]);
   importFile(board, argv[1]);

   neillclrscrn();
   for (i = 0; i <= iter; i++) {
      neillcursorhome();
      printBoard(board);
      fillBoard(board, newBoard);
      printf("Print no. %d done.\n", i);
      neillbusywait(DELAY);
   }

   return 0;
}

int importFile(int board[BOARDROWS][BOARDCOLS], const char *filename)
{
   FILE *file;
   int x, y;
   int len = 0;
   char str[STRMAX];
   bool finished = false;

   if ((file = fopen(filename, "r")) == NULL) {
      fprintf(stderr, "Failed to load %s.\n", filename);
      exit(EXIT_FAILURE);
   }

   do {
      finished = false;
      if (fgets(str, STRMAX, file) == NULL) {
         finished = true;
      }
      else {
         if (str[0] != '#') {
            if (sscanf(str, "%d %d", &x, &y) != 2) {
               fprintf(stderr, "Cannot read %s\n", filename);
               exit(EXIT_FAILURE);
            }
            len++;
            board[START + y][START + x] = 1;
         }
      }
   } while (!finished);
}

void fillBoard(int board[BOARDROWS][BOARDCOLS], int newBoard[BOARDROWS][BOARDCOLS])
{
   int r, c, aliveCount = 0;

   for (r = 0; r < BOARDROWS; r++) {
      for (c = 0; c < BOARDCOLS; c++) {
         aliveCount = countAlive(board, r, c);
         if (board[r][c] == 1) {
            if (survival(aliveCount) == true) {
               newBoard[r][c] = 1;
            }
            else {
               newBoard[r][c] = 0;
            }
         }
         else {
            if (birth(aliveCount) == true) {
               newBoard[r][c] = 1;
            }
            else {
               newBoard[r][c] = 0;
            }
         }
      }
   }
   for (r = 0; r < BOARDROWS; r++) {
      for (c = 0; c < BOARDCOLS; c++) {
         board[r][c] = newBoard[r][c];
         newBoard[r][c] = 0;
      }
   }
}

int countAlive(int board[BOARDROWS][BOARDCOLS], int currentRow, int currentCol)
{
   int r, c, cnt = 0;

   for (r = -1; r <= 1; r++) {
      for (c = -1; c <= 1; c++) {
         if (inbounds(currentRow + r, currentCol + c) == true) {
            if (board[currentRow + r][currentCol + c] == 1) {
               cnt++;
            }
         }
      }
   }
   /*Have to subtract cell position as loop will count it*/
   return cnt - board[currentRow][currentCol];
}

bool inbounds(int currentRow, int currentCol)
{
   if (currentRow >= 0 && currentRow < BOARDROWS && \
       currentCol >= 0 && currentCol < BOARDCOLS) {
      return true;
   }
   return false;
}

bool survival(int aliveCount)
{
   if (aliveCount == 2 || aliveCount == 3) {
      return true;
   }
   return false;
}

bool birth(int aliveCount)
{
   if (aliveCount == 3) {
      return true;
   }
   return false;
}

int printBoard(int board[BOARDROWS][BOARDCOLS])
{
   int r, c;

   for (r = 0; r < BOARDROWS - 1; r++) {
      for (c = 0; c < BOARDROWS - 1; c++) {
         printf("%d", board[r][c]);
      }
      printf("\n");
   }
   return 0;
}

void test()
{
   int i, aliveCount, currentRow, currentCol;
   int board[BOARDROWS][BOARDCOLS] = {{0,0,0,0,0},
                             {0,0,1,0,0},
                             {0,0,1,0,0},
                             {0,0,1,0,0},
                             {0,0,0,0,0}};

   int newBoard[BOARDROWS][BOARDCOLS] = {{0,0,0,0,0},
                                 {0,0,0,0,0},
                                 {0,0,0,0,0},
                                 {0,0,0,0,0},
                                 {0,0,0,0,0}};

   aliveCount = 2;
   assert(survival(aliveCount) == true);
   assert(birth(aliveCount)    == false);
   aliveCount = 3;
   assert(survival(aliveCount) == true);
   assert(birth(aliveCount)    == true);
   aliveCount = 5;
   assert(survival(aliveCount) == false);
   assert(birth(aliveCount)    == false);

   currentRow = 10;
   currentCol = 10;
   assert(inbounds(currentRow, currentCol) == true);
   currentRow = 91;
   currentCol = 151;
   assert(inbounds(currentRow, currentCol) == false);

   assert(countAlive(board, 1, 1) == 2);
   assert(countAlive(board, 2, 2) == 2);
   assert(countAlive(board, 4, 2) == 1);
   assert(countAlive(board, 4, 4) == 0);

   printf("Tests successful\n");

   for (i = 0; i < 10; i++) {
      printBoard(board);
      fillBoard(board, newBoard);
      printf("Print no. %d done.\n", i);
   }
}

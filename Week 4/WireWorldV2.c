#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "neillsimplescreen.h"

#define HEAD 'H'
#define TAIL 't'
#define COPPER 'c'
#define EMPTY ' '
#define GRIDROWS 41
#define GRIDCOLS 40
#define ITER 1000
#define LEFT -1
#define RIGHT 1
#define ABOVE -1
#define BELOW 1
#define DELAY 0.5

typedef enum bool {false, true} bool;

void importFile(char grid[GRIDROWS][GRIDCOLS], int numRows, int numCols, const char *filename);
void nextGen(char grid[GRIDROWS][GRIDCOLS], char newGrid[GRIDROWS][GRIDCOLS], int numRows, int numCols);
int headCnt(char grid[GRIDROWS][GRIDCOLS], int currentRow, int currentCol, int numRows, int numCols);
bool inbounds(int currentRow, int currentCol, int numRows, int numCols);
void copyGrid(char grid[GRIDROWS][GRIDCOLS], char newGrid[GRIDROWS][GRIDCOLS], int numRows, int numCols);
char printGrid(char grid[GRIDROWS][GRIDCOLS], int numRows, int numCols);
void resetGrid(char grid[GRIDROWS][GRIDCOLS], int numRows, int numCols);
void test();

int main(int argc, char *argv[])
{
   int i, numRows, numCols;
   char grid   [GRIDROWS][GRIDCOLS] = {{0}};
   char newGrid[GRIDROWS][GRIDCOLS] = {{0}};

   /*test();
   srand((unsigned)time(NULL));*/
   numRows = GRIDROWS;
   numCols = GRIDCOLS;

   importFile(grid, numRows, numCols, argv[1]);
   printGrid(grid, numRows, numCols);
   printf("Start WireWorld\n");

   neillclrscrn();
   neillfgcol(black);
   for (i = 0; i <= ITER; i++) {
      neillcursorhome();
      printGrid(grid, numRows, numCols);
      nextGen(grid, newGrid, numRows, numCols);
      printf("Print no. %d done.\n", i);
      neillbusywait(DELAY);
   }
   return 0;
}

void importFile(char grid[GRIDROWS][GRIDCOLS], int numRows, int numCols, const char *filename)
{
   FILE *file;
   int i, r, c;
   char cell;
   char buffer[(GRIDROWS*GRIDCOLS) + 1] = {0};

   if ((file = fopen(filename, "r")) == NULL) {
      fprintf(stderr, "Failed to load %s.\n", filename);
      exit(EXIT_FAILURE);
   }
   i = 0;
   while ((cell = fgetc(file)) != EOF) {
      if (cell != '\n') {
         buffer[i] = cell;
         i++;
      }
      /*printf("%d\n", cell);*/
   }
   buffer[i] = '\0';

   i = 0;
   for (r = 0; r < numRows; r++) {
      for (c = 0; c < numCols; c++) {
         grid[r][c] = buffer[i];
         i++;
      }
   }
   fclose(file);
}
/*
bool validChar(buffer[(GRIDROWS*GRIDCOLS) + 1])
{

}
*/
void nextGen(char grid[GRIDROWS][GRIDCOLS], char newGrid[GRIDROWS][GRIDCOLS], int numRows, int numCols)
{
   int r, c, h;

   for (r = 0; r < numRows; r++) {
      for (c = 0; c < numCols; c++) {
         h = headCnt(grid, r, c, numRows, numCols);
         /*printf("grid[%d][%d]headcnt: %d\n", r, c, h);*/
         if (grid[r][c] == HEAD) {
            newGrid[r][c] = TAIL;
         }
         else if (grid[r][c] == TAIL) {
            newGrid[r][c] = COPPER;
         }
         else if (grid[r][c] == COPPER) {
            if (h == 1 || h == 2) {
               newGrid[r][c] = HEAD;
            }
            else {
               newGrid[r][c] = COPPER;
            }
         }
         else {
            newGrid[r][c] = EMPTY;
         }
      }
   /*   grid[r][c-1] = '\0'; */
   }
   copyGrid(grid, newGrid, numRows, numCols);
   /*resetGrid(newGrid, numRows, numCols);*/
}

int headCnt(char grid[GRIDROWS][GRIDCOLS], int currentRow, int currentCol, int numRows, int numCols)
{
   int r, c;
   int cnt = 0;

   for (r = ABOVE; r <= BELOW; r++) {
      for (c = LEFT; c <= RIGHT; c++) {
         if (inbounds(currentRow + r, currentCol + c, numRows, numCols)) {
            if (grid[currentRow + r][currentCol + c] == HEAD) {
               cnt++;
            }
         }
      }
   }
   if (grid[currentRow][currentCol] == HEAD) {
      return cnt - 1;
   }
   else {
      return cnt;
   }
}

bool inbounds(int currentRow, int currentCol, int numRows, int numCols)
{
   if (currentRow >=0 && currentRow < numRows &&
       currentCol >=0 && currentCol < numCols) {
      return true;
   }
   return false;
}

void copyGrid(char grid[GRIDROWS][GRIDCOLS], char newGrid[GRIDROWS][GRIDCOLS], int numRows, int numCols)
{
   int r, c;

   for (r = 0; r < numRows; r++) {
      for (c = 0; c < numCols; c++) {
         grid[r][c] = newGrid[r][c];
      }
   }
}

char printGrid(char grid[GRIDROWS][GRIDCOLS], int numRows, int numCols)
{
   int r, c;

   for (r = 0; r < numRows; r++) {
      for (c = 0; c < numCols; c++) {
         if (grid[r][c] == TAIL) {
            neillfgcol(red);
         }
         else if (grid[r][c] == HEAD) {
            neillfgcol(blue);
         }
         else if (grid[r][c] == COPPER) {
            neillfgcol(yellow);
         }
         printf("%c", grid[r][c]);
      }
      printf("\n");
   }
   return 0;
}

void resetGrid(char grid[GRIDROWS][GRIDCOLS], int numRows, int numCols)
{
   int r, c;

   for (r = 0; r < numRows; r++) {
      for (c = 0; c < numCols; c++) {
         grid[r][c] = EMPTY;
      }
   }
}

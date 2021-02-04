/*
forest.c assignment for the Programming in C module
By Harry Smith
On 28/10/2020
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "neillsimplescreen.h"

#define TREE '@'
#define FIRE '*'
#define EMPTY ' '
#define G 250
#define L (10*G)
#define GRIDCOLS 80
#define GRIDROWS 30
#define ITER 1000
#define LOOPS 5
#define LEFT -1
#define RIGHT 1
#define ABOVE -1
#define BELOW 1

typedef enum bool {false, true} bool;

void initGrid (int grid[GRIDROWS][GRIDCOLS], int numRows, int numCols);
char printGrid(int grid[GRIDROWS][GRIDCOLS], int numRows, int numCols);
void fillGrid (int grid[GRIDROWS][GRIDCOLS], int newGrid[GRIDROWS][GRIDCOLS], int numRows, int numCols);
void copyGrid (int grid[GRIDROWS][GRIDCOLS], int newGrid[GRIDCOLS][GRIDCOLS], int numRows, int numCols);
void fireOut  (int grid[GRIDROWS][GRIDCOLS], int newGrid[GRIDROWS][GRIDCOLS], int numRows, int numCols);
bool neighFire(int grid[GRIDROWS][GRIDCOLS], int currentRow, int currentCol, int numRows, int numCols);
bool inbounds (int currentRow, int currentCol, int numRows, int numCols);
bool treeGrowth();
int  treeGrowthCheck(int growthRate);
bool lightningStrike();
int  lightningStrikeCheck(int strikeRate);
void test();

int main(void)
{

   int i, numRows, numCols;
   int grid[GRIDROWS][GRIDCOLS];
   int newGrid[GRIDROWS][GRIDCOLS];

   test();
   srand((unsigned)time(NULL));

   numRows = GRIDROWS;
   numCols = GRIDCOLS;

   initGrid(grid, numRows, numCols);
   neillclrscrn();
   neillfgcol(black);
   for (i = 0; i < ITER; i++) {
      neillcursorhome();
      printGrid(grid, numRows, numCols);
      fillGrid(grid, newGrid, numRows, numCols);
      neillbusywait(0.5);
   }
   return 0;
}

/* Function to initialise grid with blank spaces */
void initGrid(int grid[GRIDROWS][GRIDCOLS], int numRows,
              int numCols)
{
   int r, c;

   for (r = 0; r < numRows; r++) {
      for (c = 0; c < numCols; c++) {
         grid[r][c] = EMPTY;
      }
   }
}

/* Function to print out grid values */
char printGrid(int grid[GRIDROWS][GRIDCOLS], int numRows,
               int numCols)
{
   int r, c;

   for (r = 0; r < numRows; r++) {
      for (c = 0; c < numCols; c++) {
         if (grid[r][c] == TREE) {
            neillfgcol(green);
         }
         else if (grid[r][c] == FIRE) {
            neillfgcol(red);
         }
         printf("%c", grid[r][c]);
      }
   }
   return 0;
}

/* Grow trees('@'), set/spread fires(FIRE), burn trees (' ')
   and put out any fires that have lived their life       */
void fillGrid(int grid[GRIDROWS][GRIDCOLS], int newGrid[GRIDROWS][GRIDCOLS], int numRows, int numCols)
{
   int r, c;
   for (r = 0; r < numRows; r++) {
      for (c = 0; c < numCols; c++) {
         if (grid[r][c] == EMPTY) {
            if (treeGrowth()) {
               newGrid[r][c] = TREE;
            }
            else {
               newGrid[r][c] = EMPTY;
            }
         }
         else if (grid[r][c] == TREE) {
            if (lightningStrike() || \
               (neighFire(grid, r, c, numRows, numCols))) {
               newGrid[r][c] = FIRE;
            }
            else {
               newGrid[r][c] = TREE;
            }
         }
      }
   }
   fireOut(grid, newGrid, numRows, numCols);
   copyGrid(grid, newGrid, numRows, numCols);
   initGrid(newGrid, numRows, numCols);
}

/* Function to put out old fires after tree growth lightning
   strikes, and fire transfer has occured in the grid  */
void fireOut(int grid[GRIDROWS][GRIDCOLS], int newGrid[GRIDROWS][GRIDCOLS], int numRows, int numCols)
{
   int r, c;

   for (r = 0; r < numRows; r++) {
      for (c = 0; c < numCols; c++) {
         if (grid[r][c] == FIRE) {
            newGrid[r][c] = EMPTY;
         }
      }
   }
}

/* Function used to copy grid into different grid */
void copyGrid(int grid[GRIDROWS][GRIDCOLS], int newGrid[GRIDCOLS][GRIDCOLS], int numRows, int numCols)
{
   int r, c;

   for (r = 0; r < numRows; r++) {
      for (c = 0; c < numCols; c++) {
         grid[r][c] = newGrid[r][c];
      }
   }
}

/* Function to check if any of the 8-cell neighbourhood of
   target cell is on fire */
bool neighFire(int grid[GRIDROWS][GRIDCOLS], int currentRow, int currentCol, int numRows, int numCols)
{
   int r, c;

   for (r = LEFT; r <= RIGHT; r++) {
      for (c = ABOVE; c <= BELOW; c++) {
         if (inbounds(currentRow + r, currentCol + c,
                      numRows, numCols)) {
            if (grid[currentRow + r][currentCol + c]==FIRE){
               return true;
            }
         }
      }
   }
   return false;
}

/* Function to check array pointer doesnt go out of bounds*/
bool inbounds(int currentRow, int currentCol, int numRows, int numCols)
{
   if (currentRow >= 0 && currentRow < numRows &&
       currentCol >= 0 && currentCol < numCols) {
      return true;
   }
   return false;
}

/* Function to simulate tree growth 1 in G times */
bool treeGrowth()
{
   if ((rand() % G) == 1) {
      return true;
   }
   return false;
}

/* Function to simulate lightning strike 1 in L times */
bool lightningStrike()
{
   if ((rand() % L) == 1) {
      return true;
   }
   return false;
}

/* Function for testing initGrid function */
int sumGrid(int grid[GRIDROWS][GRIDCOLS], int numRows, int numCols)
{
   int r, c, sum = 0;

   for (r = 0; r < numRows; r++) {
      for (c = 0; c < numCols; c++) {
         sum += grid[r][c];
      }
   }
   return sum;
}

/* Function to check the logic used in lightningStrikeCheck
   and treeGrowthCheck function is sound and usable */
bool sumCheck()
{
   int i, cnt = 0;
   for (i = 1; i <= 10; i++) {
      if (i%2 == 0) {
         cnt++;
      }
   }
   if (cnt == 5) {
      return true;
   }
   return false;
}

/* Function for testing random treeGrowth function
   tested by testing generic sumCheck function */
int treeGrowthCheck(int growthRate)
{
   int i, cnt = 0;
   for (i = 0; i < growthRate * LOOPS; i++) {
      if (treeGrowth()) {
         cnt++;
      }
   }
   return cnt;
}

/* Function for testing random lightningStrike function
   tested by testing generic sumCheck function */
int lightningStrikeCheck(int strikeRate)
{
   int i, cnt = 0;
   for (i = 0; i < strikeRate * LOOPS; i++) {
      if (lightningStrike()) {
         cnt++;
      }
   }
   return cnt;
}

void test()
{
   int numRows = 10, numCols = 10;
   int gridSize, gridSum, gridSum2, currentRow, currentCol;
   int testGrid1[GRIDROWS][GRIDCOLS] = {{0,0,5,0,0},
                                        {0,1,0,0,0},
                                        {0,0,0,3,0},
                                        {0,0,1,0,9},
                                        {0,0,0,0,0}};
   int testGrid2[GRIDROWS][GRIDCOLS];
   int testGrid3[GRIDROWS][GRIDCOLS] = {{'T','E','S','T'},
                                        {'F','U','N','C'}};
   int testGrid4[GRIDROWS][GRIDCOLS] = {{' ',' ',' ',' '},
                                        {' ',' ',' ',' '}};
   int testGrid5[GRIDROWS][GRIDCOLS] = {{' ',' ',FIRE,' '}};
   int testGrid6[GRIDROWS][GRIDCOLS] = {{' ',' ',' ',' '}};
   int testGrid7[GRIDROWS][GRIDCOLS] = {{' ',' ',' ',' '},
                                        {' ',' ',' ',FIRE}};
   int testGrid8[GRIDROWS][GRIDCOLS] = {{FIRE,' ',' ',' '},
                                        {' ',FIRE,' ',' '}};
   int testGrid9[GRIDROWS][GRIDCOLS] = {{' ',' ',' ',' '},
                                        {' ',' ',' ',' '}};

/* Test sumGrid function; testGrid1 should = 19 */
   assert(sumGrid(testGrid1, numRows, numCols) == 19);
   assert(sumGrid(testGrid1, numRows, numCols) !=  0);

/* Test initGrid function by summing all values. Blank space
   has ASCII value of 32, multiplied by gridSize, gives int.
   Check ' ' initalised testGrid == gridSum */
   gridSize = (numRows * numCols);
   gridSum  = (gridSize * EMPTY); /*EMPTY = ' '*/
   initGrid(testGrid2, numRows, numCols);
   assert(sumGrid(testGrid2, numRows, numCols)==gridSum);
   gridSize = (GRIDROWS * GRIDCOLS);
   gridSum  = (gridSize * EMPTY);
   initGrid(testGrid2, GRIDROWS, GRIDCOLS);
   assert(sumGrid(testGrid2, GRIDROWS, GRIDCOLS)==gridSum);


/* Test inbounds function - check cell index is inbounds  */
   currentRow = 9;
   currentCol = 9;
   assert(inbounds(currentRow, currentCol, numRows, numCols));
   currentRow = 11;
   currentCol = 11;
   assert(!inbounds(currentRow, currentCol, numRows, numCols));
   currentRow = GRIDROWS + 1;
   currentCol = GRIDCOLS + 1;
   assert(!inbounds(currentRow, currentCol, GRIDROWS, GRIDCOLS));

/* Check logic in lightningStrikeCheck and treeGrowthCheck
   functions is sound before using them to test treeGrowth
   and lightningStrike functions */
   assert(sumCheck());

/* Test treeGrowth and lightningStrike function by checking
   if in 5 loops we get between 0 and 10 successes.
   0-20 range accounts for randomness of rand() function. */
   assert(treeGrowthCheck(G) >= 0 && treeGrowthCheck(G) <= 20);
   assert(lightningStrikeCheck(L) >= 0 && lightningStrikeCheck(L) <= 20);

/* Test copyGrid function by copying an array into a
   new array, summing it and checking if equivalent       */
   copyGrid(testGrid4, testGrid3, numRows, numCols);
   gridSum  = sumGrid(testGrid3, numRows, numCols);
   gridSum2 = sumGrid(testGrid4, numRows, numCols);
   assert(gridSum == gridSum2);

/* Test neighFire function: check '*' located in array   */
   assert (neighFire(testGrid5,  0, 2, numRows, numCols));
   assert(!neighFire(testGrid6,  0, 2, numRows, numCols));
   assert (neighFire(testGrid7,  1, 4, numRows, numCols));

/* Test fireOut function: put out testGrid8 (on fire) and
   check sum value equals sum of testGrid9 (not on fire)  */
   fireOut(testGrid8, testGrid8, numRows, numCols);
   gridSum  = sumGrid(testGrid8, numRows, numCols);
   gridSum2 = sumGrid(testGrid9, numRows, numCols);
   assert(gridSum == gridSum2);

   printf("\nTesting complete.\n");
}

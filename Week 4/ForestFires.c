#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define G 250
#define L (2*G)
#define GRIDCOLS 80
#define GRIDROWS 30
#define ITER 1000
#define BEHIND -1
#define AHEAD 1

typedef enum bool {false, true} bool;

void initGrid  (int grid[GRIDROWS][GRIDCOLS], int numRows, int numCols);
char printGrid (int grid[GRIDROWS][GRIDCOLS], int numRows, int numCols);
void fillGrid  (int grid[GRIDROWS][GRIDCOLS], int newGrid[GRIDROWS][GRIDCOLS], int numRows, int numCols);
void copyGrid  (int grid[GRIDROWS][GRIDCOLS], int newGrid[GRIDCOLS][GRIDCOLS], int numRows, int numCols);
bool neighFires(int grid[GRIDROWS][GRIDCOLS], int currentRow, int currentCol, int numRows, int numCols);
bool inbounds  (int currentRow, int currentCol, int numRows, int numCols);
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
   /* srand((unsigned)time(NULL));*/

   numRows = GRIDROWS;
   numCols = GRIDCOLS;

   initGrid(grid, numRows, numCols);

   for (i = 0; i < ITER; i++) {
      printGrid(grid, numRows, numCols);
      fillGrid(grid, newGrid, numRows, numCols);
      printf("Grid no. %d done.\n", i);
   }

   return 0;
}

/*tested*/
void initGrid(int grid[GRIDROWS][GRIDCOLS], int numRows, int numCols)
{
   int r, c;

   for (r = 0; r < numRows; r++) {
      for (c = 0; c < numCols; c++) {
         grid[r][c] = ' ';
      }
   }
}

/* not tested*/
char printGrid(int grid[GRIDROWS][GRIDCOLS], int numRows, int numCols)
{
   int r, c;

   for (r = 0; r < numRows; r++) {
      for (c = 0; c < numCols; c++) {
         printf("%c", grid[r][c]);
      }
      printf("\nRow %d ", r + 1);
   }
   return 0;
}

/*Grow trees('^'), set/spread fires('*'), burn trees(' ')*/
void fillGrid(int grid[GRIDROWS][GRIDCOLS], int newGrid[GRIDROWS][GRIDCOLS], int numRows, int numCols)
{
   int r, c;

   for (r = 0; r < GRIDROWS; r++) {
      for (c = 0; c < GRIDCOLS; c++) {
         if (grid[r][c] == ' ') {
            if (treeGrowth()) {
               newGrid[r][c] = '^';
            }
            else {
               newGrid[r][c] = ' ';
            }
         }
         else if (grid[r][c] == '^') {
            if (lightningStrike() || \
               (neighFires(grid, r, c, numRows, numCols))) {
               newGrid[r][c] = '@';
            }
            else {
               newGrid[r][c] = '^';
            }
         }
         else if (grid[r][c] == '@') {
            newGrid[r][c] = ' ';
         }
       }
   }
   copyGrid(grid, newGrid, numRows, numCols);
   initGrid(newGrid, numRows, numCols);
}

void fireLoop(int grid[GRIDROWS][GRIDCOLS], int )

/*partially tested?*/
void copyGrid(int grid[GRIDROWS][GRIDCOLS], int newGrid[GRIDCOLS][GRIDCOLS], int numRows, int numCols)
{
   int r, c;
   for (r = 0; r < numRows; r++) {
      for (c = 0; c < numCols; c++) {
         grid[r][c] = newGrid[r][c];
      }
   }
}

/*tested*/
bool neighFires(int grid[GRIDROWS][GRIDCOLS], int currentRow, int currentCol, int numRows, int numCols)
{
   int r, c;

   for (r = BEHIND; r <= AHEAD; r++) {
      for (c = BEHIND; c <= AHEAD; c++) {
         if(inbounds(currentRow + r, currentCol + c, numRows, numCols)) {
            if (grid[currentRow + r][currentCol + c] == '*') {
               return true;
            }
         }
      }
   }
   return false;
}

/*tested*/
bool inbounds(int currentRow, int currentCol, int numRows, int numCols)
{
   if (currentRow >= 0 && currentRow < numRows && \
      currentCol >= 0 && currentCol < numCols) {
      return true;
   }
   return false;
}

/*tested*/
bool treeGrowth()
{
   if ((rand() % G) == 1) {
      return true;
   }
   return false;
}

/*tested*/
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

/* Function for testing random treeGrowth function */
int treeGrowthCheck(int growthRate)
{
   int i, cnt = 0;
   for (i = 0; i < growthRate * 5; i++) {
      if (treeGrowth()) {
         cnt++;
      }
   }
   return cnt;
}

/* Function for testing random lightningStrike function */
int lightningStrikeCheck(int strikeRate)
{
   int i, cnt = 0;
   for (i = 0; i < strikeRate * 5; i++) {
      if (lightningStrike()) {
         cnt++;
      }
   }
   return cnt;
}

void test()
{
   int numRows = 10, numCols = 10;
   int gridSize, gridSum, currentRow, currentCol;
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

   int testGrid5[GRIDROWS][GRIDCOLS] = {{' ',' ','*',' '}};
   int testGrid6[GRIDROWS][GRIDCOLS] = {{' ',' ',' ',' '}};
   int testGrid7[GRIDROWS][GRIDCOLS] = {{' ',' ',' ',' '},
                                         {' ',' ',' ','*'}};

/* Test sumGrid function; testGrid1 should = 19 */
   assert(sumGrid(testGrid1, numRows, numCols) == 19);
   assert(sumGrid(testGrid1, numRows, numCols) != 0);

/* Test initGrid function by summing all values: 32*25 = 0 */
   gridSize = (numRows * numCols);
   gridSum  = (gridSize * ' ');
   initGrid(testGrid2, numRows, numCols);
   assert(sumGrid(testGrid2, numRows, numCols) == gridSum);

/* Test inbounds function - checks if cell index is inbounds*/
   currentRow = 9;
   currentCol = 9;
   assert(inbounds(currentRow, currentCol, numRows, numCols));
   currentRow = 81;
   currentCol = 31;
   assert(!inbounds(currentRow, currentCol, numRows, numCols));

/* Test treeGrowth function by checking if in 5 loops we get*/
/* between 0 and 10 tree growths - allows for some range.   */
/* Same again for lightningStrike function check            */
   assert(treeGrowthCheck(G) > 0 && \
          treeGrowthCheck(G) <= 10);
   assert(lightningStrikeCheck(L) > 0 && \
          lightningStrikeCheck(L) <= 10);

/* Test copyGrid function by copying a new array into a     */
/* new array and compare the strings in the two arrays      */
/*needs fixing for flags */
   copyGrid(testGrid4, testGrid3, numRows, numCols);
   assert(strcmp(testGrid3, testGrid4) == 0);

/*Test neighFires function: check if '*' is located in array*/
   assert (neighFires(testGrid5,  0, 2, numRows, numCols));
   assert(!neighFires(testGrid6,  0, 2, numRows, numCols));
   assert (neighFires(testGrid7,  1, 4, numRows, numCols));

   printf("\nTesting complete.\n");
}

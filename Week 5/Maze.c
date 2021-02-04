#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

typedef enum bool {false, true} bool;

struct mazeGrid {
   int rows, cols;
};

typedef struct mazeGrid grid;

#define BUFFERSIZE 1000
#define ROWS 20
#define COLS 20
#define PATH ' '
#define WALL '#'
#define BEEN '.'

void fillMaze(char maze[ROWS][COLS]);
grid importFile (char *filename, char maze[ROWS][COLS]);
int explore(char maze[ROWS][COLS], int r, int c);
bool validMove(char maze[ROWS][COLS], int r, int c);
int printMaze(char maze[ROWS][COLS], int rows, int cols);

int main(void)
{
   int rows, cols;
   grid rowCols;
   char maze[ROWS][COLS];

   fillMaze(maze);
   rowCols = importFile("maze.txt", maze);
   rows = rowCols.rows;
   cols = rowCols.cols;
   printMaze(maze, rows, cols);
   explore(maze, 1, 0);
   printMaze(maze, rows, cols);
   return 0;
}

void fillMaze(char maze[ROWS][COLS])
{
   int r, c;

   for (r = 0; r < ROWS; r++) {
      for (c = 0; c < COLS; c++) {
         maze[r][c] = ' ';
      }
   }
}

int printMaze(char maze[ROWS][COLS], int rows, int cols)
{
   int r, c;
   for (r = 0; r < rows; r++) {
      for (c = 0; c < cols; c++) {
         printf("%c", maze[r][c]);
      }
   printf("\n");
   }
}

int explore(char maze[ROWS][COLS], int r, int c)
{
   /*printMaze(maze);*/
   if (maze[r-1][c] != PATH && maze[r+1][c] != PATH &&
       maze[r][c-1] != PATH && maze[r][c+1] != PATH) {
      return true;
   }

   if (maze[r][c] == PATH) {
      maze[r][c] = BEEN;

      if (validMove(maze, r-1, c)) {
         if (explore(maze, r-1, c)) {
         }
      }
      if (validMove(maze, r+1, c)) {
         if (explore(maze, r+1, c)) {
         }
      }
      if (validMove(maze, r, c-1)) {
         if (explore(maze, r, c-1)) {
         }
      }
      if (validMove(maze, r, c+1)) {
         if (explore(maze, r, c+1)) {
         }
      }
   }
   return false;
}

bool validMove(char maze[ROWS][COLS], int r, int c)
{
   if ((r < 0) || (c < 0) || (r >= ROWS) || (c >= COLS)) {
      return false;
   }
   if (maze[r][c] == '.' || maze[r][c] == '#') {
      return false;
   }
   return true;
}

grid importFile (char *filename, char maze[ROWS][COLS])
{
   FILE *file;
   char str[1000];
   char buffer[50];
   char cell;
   int i, r, c;
   int rows, cols;
   grid grid;

   if ((file = fopen(filename, "r")) == NULL) {
      fprintf(stderr, "Failed to load %s.\n", filename);
      exit(EXIT_FAILURE);
   }

   if ((fgets(buffer, BUFFERSIZE, file) == NULL)) {
      fprintf(stderr,"Failed to read file.\n");
      exit(EXIT_FAILURE);
   }
   sscanf(buffer, "%d %d", &rows, &cols);

   i = 0;
   while ((cell = fgetc(file)) != EOF) {
      if (cell != '\n') {
         str[i] = cell;
         i++;
      }
/*      printf("%c", cell);*/
   }
   str[i] = '\0';

   i = 0;
   for (r = 0; r < rows; r++) {
      for (c = 0; c < cols; c++) {
         maze[r][c] = str[i];
         i++;
      }
   }
   grid.rows = rows;
   grid.cols = cols;
   return grid;
   fclose(file);
}

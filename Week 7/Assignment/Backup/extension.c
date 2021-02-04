#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "neillsimplescreen.h"

#define HEIGHT 9
#define WIDTH 9
#define ALPHANUM 26
#define MAXDIFF 2
#define MAXLEN 50
#define MAXCH 100000
#define BUFFERSIZE 1000
#define MAXSTR (HEIGHT*WIDTH)+1
#define MAXBC 10000000
#define BLACK 'k'
#define RED 'r'
#define GREEN 'g'
#define YELLOW 'y'
#define BLUE 'b'
#define MAGNETA 'm'
#define CYAN 'c'
#define WHITE 'w'
#define EMPTY '.'
#define NOPARENT -1
#define FLAG "verbose"

typedef enum bool {false, true} bool;

typedef struct bookcase {
   char svs[HEIGHT][WIDTH];
   int  parent;
} Bookcase;

typedef struct dimensions {
   int height;
   int width;
} Dimensions;

bool boundaryCheck (Dimensions dim);
bool dupeShelves   (Bookcase bcArr[MAXBC], int c, Dimensions dim);
bool isValid       (char str[HEIGHT*WIDTH+1], int i);
bool isBook        (char svs[HEIGHT][WIDTH], int h, int w, Dimensions dim);
bool isShelfFull   (char svs[HEIGHT][WIDTH], int h, Dimensions dim);
bool isShelfEmpty  (char svs[HEIGHT][WIDTH], int h, Dimensions dim);
bool happyShelf    (char svs[HEIGHT][WIDTH], int h, Dimensions dim);
bool happyShelf2   (char svs[HEIGHT][WIDTH], Dimensions dim);
bool colourMatch   (char svs[HEIGHT][WIDTH], int h1, int w1, int h2, int w2, Dimensions dim);
bool colourCount   (char svs[HEIGHT][WIDTH], Dimensions dim);
bool happyBookPos  (char svs[HEIGHT][WIDTH], Dimensions dim);
bool happyBookcase (char svs[HEIGHT][WIDTH], Dimensions dim);
void stringify     (char svs[HEIGHT][WIDTH], char string[MAXSTR], Dimensions dim);
void printBookcase (char svs[HEIGHT][WIDTH], Dimensions dim);
void printColour   (char svs[HEIGHT][WIDTH], int h, int w);
void copyShelves   (char svs2[HEIGHT][WIDTH], char svs[HEIGHT][WIDTH], Dimensions dim);
void validFile     (char str[HEIGHT*WIDTH+1], int cntH, int w, Dimensions dim);
void createChild   (Bookcase bcArr[MAXBC], int c, int p, int h2, int w2, int h1, int w1, Dimensions dim);
void clearShelves  (Bookcase bcArr[MAXBC], int c, Dimensions dim);
void initBookcase  (Bookcase bcArr[MAXBC], int max, Dimensions dim);
void printRoute    (Bookcase bcArr[MAXBC], int route[MAXCH], int cnt, Dimensions dim);
void flagOutput    (Bookcase bcArr[MAXBC], int route[MAXCH], int argc, int diff, Dimensions dim);
void copyFile      (Bookcase bcArr[MAXBC], FILE *file, Dimensions dim);
int  testInit      (Bookcase bcArr[MAXBC], Dimensions dim);
int  runHappy      (Bookcase bcArr[MAXBC], int route[MAXCH], Dimensions dim);
int  bruteForce    (Bookcase bcArr[MAXBC], int c, int p, Dimensions dim);
int  rightCell     (char svs[HEIGHT][WIDTH], int h, Dimensions dim);
int  bcWidthCheck  (int cntW, Dimensions dim);
int  shelfWrap     (int h,    Dimensions dim);
Dimensions readFile(Bookcase bcArr[MAXBC], char *filename);
void test();

int main(int argc, char* argv[MAXLEN])
{
   int  diff;
   char flag[WIDTH];
   int  route[MAXCH];
   Dimensions dim;
   Bookcase* bcArr;
   bcArr = (Bookcase*)calloc(MAXBC+1, sizeof(Bookcase));
   if (bcArr == NULL) {
      fprintf(stderr, "Failed to allocate memory.\n");
      exit(EXIT_FAILURE);
   }
   test();

   dim = readFile(bcArr, argv[1]);
   diff = strcmp(strcpy(flag, argv[2]), FLAG);
   flagOutput(bcArr, route, argc, diff, dim);
   return 0;
}

/* Tests the initial bookcase to see if it is full already and cannot be changed */
int testInit(Bookcase bcArr[MAXBC], Dimensions dim)
{
   int h, cnt;
   cnt = 0;
   for (h = 0; h < dim.height; h++) {
      if (isShelfFull(bcArr[0].svs, h, dim)) {
         cnt++;
      }
   }
   if (cnt == dim.height) {
      if (happyBookcase(bcArr[0].svs, dim)) {
         printf("1\n\n");
         printBookcase(bcArr[0].svs, dim);
         exit(EXIT_FAILURE);
      }
      else {
         printf("No solution?");
         exit(EXIT_FAILURE);
      }
   }
   return 0;
}

/* Print out results based on the user input; argv[2] = verbose prints out printRoute. Any other
   combination of command line inputs will result in just number of shelves req to get happy */
void flagOutput(Bookcase bcArr[MAXBC], int route[MAXCH], int argc, int diff, Dimensions dim)
{
   int cnt;
   testInit(bcArr, dim);
   cnt = runHappy(bcArr, route, dim);
   if (argc == 1 || argc < 1) {
      fprintf(stderr, "Please enter a filename to import.\n");
      exit(EXIT_FAILURE);
   }
   else if (argc == 2) {
      printf("%d\n\n", cnt-1);
   }
   else if (argc >= 3) {
      if (diff == 0) {
         printf("%d\n\n", cnt-1);
         printRoute(bcArr, route, cnt, dim);
      }
      else {
         printf("%d\n\n", cnt-1);
      }
   }
   free(bcArr);
}

/* Copies initial shelves from file. Also checks the shelves dimensions match the
   top line dimensions from the file and that there are only valid chars in file */
Dimensions readFile(Bookcase bcArr[MAXBC], char *filename)
{
   FILE *file;
   char buffer[BUFFERSIZE];
   Dimensions dim;

   if ((file = fopen(filename, "r")) == NULL) {
      fprintf(stderr, "Failed to load %s.\n", filename);
      exit(EXIT_FAILURE);
   }
   if ((fgets(buffer, BUFFERSIZE, file) == NULL)) {
      fprintf(stderr,"Failed to read file.\n");
      exit(EXIT_FAILURE);
   }
   sscanf(buffer, "%d %d", &dim.height, &dim.width);
   if (!boundaryCheck(dim)) {
      fprintf(stderr, "Bookcase dimensions too large/small.\n");
      exit(EXIT_FAILURE);
   }
   initBookcase(bcArr, MAXBC, dim);
   copyFile(bcArr, file, dim);
   return dim;
}

void copyFile(Bookcase bcArr[MAXBC], FILE *file, Dimensions dim)
{
   int i, h, w, cntW, cntH;
   char cell;
   char str[HEIGHT*WIDTH+1];

   i = 0; cntW = 0; cntH = 0;
   while ((cell = fgetc(file)) != EOF) {
      if (cell != '\n') {
         str[i++] = tolower(cell);
         ++cntW;
      }
      else if (cell == '\n') {
         cntW = bcWidthCheck(cntW, dim);
         ++cntH;
      }
   }
   validFile(str, cntH, i, dim);
   i = 0;
   for (h = 0; h < dim.height; h++) {
      for (w = 0; w < dim.width; w++) {
         bcArr[0].svs[h][w] = str[i++];
      }
   }
   fclose(file);
}

int bcWidthCheck(int cntW, Dimensions dim)
{
   if (cntW > dim.width) {
      fprintf(stderr, "Bookcase width doesn't match the"
                      " files pre-set dimensions.\n");
      exit(EXIT_FAILURE);
   }
   else {
      return cntW = 0;
   }
}

void validFile(char str[HEIGHT*WIDTH+1], int cntH, int w, Dimensions dim)
{
   int i;

   if (cntH > dim.height) {
      fprintf(stderr, "Bookcase height doesn't match the"
                      " files pre-set dimensions.\n");
      exit(EXIT_FAILURE);
   }

   for (i = 0; i < w; i++) {
      if (!isValid(str, i)) {
         fprintf(stderr, "Invalid file input format.\n");
         exit(EXIT_FAILURE);
      }
   }
}

void initBookcase(Bookcase bcArr[MAXBC], int max, Dimensions dim)
{
   int i, h, w;

   for (i = 0; i < max; i++) {
      bcArr[i].parent = 0;
      for (h = 0; h < dim.height; h++) {
         for (w = 0; w < dim.width; w++) {
            bcArr[i].svs[h][w] = EMPTY;
         }
      }
   }
   bcArr[0].parent = NOPARENT;
}

/* Function checks that only valid characters are being read into bookshelf */
bool isValid(char str[HEIGHT*WIDTH+1], int i)
{
   switch(str[i]) {
      case BLACK:
         return true;
      case RED:
         return true;
      case GREEN:
         return true;
      case YELLOW:
         return true;
      case BLUE:
         return true;
      case MAGNETA:
         return true;
      case CYAN:
         return true;
      case WHITE:
         return true;
      case EMPTY:
            return true;
      default:
         return false;
   }
}

bool isBook(char svs[HEIGHT][WIDTH], int h, int w, Dimensions dim)
{
   if (h >= dim.height || h < 0 || w >= dim.width || w < 0) {
      return false;
   }
   switch(svs[h][w]) {
      case BLACK:
         return true;
      case RED:
         return true;
      case GREEN:
         return true;
      case YELLOW:
         return true;
      case BLUE:
         return true;
      case MAGNETA:
         return true;
      case CYAN:
         return true;
      case WHITE:
         return true;
      default:
         return false;
   }
}

bool isShelfEmpty(char svs[HEIGHT][WIDTH], int h, Dimensions dim)
{
   int w;
   if (h < 0 || h > dim.height) {
      return false;
   }

   for (w = 0; w < dim.width; w++) {
      if (svs[h][w] != EMPTY) {
         return false;
      }
   }
   return true;
}

bool isShelfFull(char svs[HEIGHT][WIDTH], int h, Dimensions dim)
{
   int w;
   if (h < 0 || h > dim.height) {
      return false;
   }

   for (w = 0; w < dim.width; w++) {
      if (svs[h][w] == EMPTY) {
         return false;
      }
   }
   return true;
}

bool colourMatch(char svs[HEIGHT][WIDTH], int h1, int w1, int h2, int w2, Dimensions dim)
{
   if ((isBook(svs, h1, w1, dim)) &&
       (isBook(svs, h2, w2, dim))) {
      if (svs[h1][w1] == svs[h2][w2]) {
         return true;
      }
      return false;
   }
   return false;
}

bool happyShelf(char svs[HEIGHT][WIDTH], int h, Dimensions dim)
{
   int w;
   if (h < 0 || h > dim.height) {
      return false;
   }

   if (isShelfEmpty(svs, h, dim)) {
      return true;
   }

   for (w = 0; w < dim.width; w++) {
      if (!colourMatch(svs, h, 0, h, w, dim)) {
         if (isBook(svs, h, w, dim)) {
            return false;
         }
      }
   }
   return true;
}

/* Part two of happy shelf. Checks if a single colour
   is split over multiple shelves. If so == not happy */
bool happyShelf2(char svs[HEIGHT][WIDTH], Dimensions dim)
{
   int h, h2, w, w2;

   for (h = 0; h < dim.height; h++) {
      if (!isShelfEmpty(svs, h, dim)) {
         h2 = shelfWrap(h, dim);
         while (h2 != h) {
            for (w = 0; w < dim.width; w++) {
               for (w2 = 0; w2 < dim.width; w2++) {
                  if (colourMatch(svs, h, w, h2, w2, dim)) {
                     return false;
                  }
               }
            }
            h2 = shelfWrap(h2, dim);
         }
      }
   }
   return true;
}

/* Function that counts num of each colour; if more than
   the bookcase width, will fail happy bookcase test      */
bool colourCount(char svs[HEIGHT][WIDTH], Dimensions dim)
{
   int i, h, w;
   int colours[ALPHANUM];

   for (i = 0; i < ALPHANUM; i++) {
      colours[i] = 0;
   }
   for (h = 0; h < dim.height; h++) {
      for (w = 0; w < dim.width; w++) {
         if (isBook(svs, h, w, dim)) {
            colours[svs[h][w] - 'a']++;
         }
      }
   }
   for (i = 0; i < ALPHANUM; i++) {
      if (colours[i] > dim.width) {
         return false;
      }
   }
   return true;
}

bool happyBookPos(char svs[HEIGHT][WIDTH], Dimensions dim)
{
   int h, w, w2;

   for (h = 0; h < dim.height; h++) {
      for (w = 0; w < dim.width; w++) {
         if (svs[h][w] == EMPTY) {
            for (w2 = w; w2 < dim.width; w2++) {
               if (isBook(svs, h, w2, dim)) {
                  return false;
               }
            }
         }
      }
   }
   return true;
}

bool happyBookcase(char svs[HEIGHT][WIDTH], Dimensions dim)
{
   int h;

   if (!colourCount(svs, dim)) {
      return false;
   }
   if (!happyBookPos(svs, dim)) {
      return false;
   }
   for (h = 0; h < dim.height; h++) {
      if (!happyShelf(svs, h, dim)) {
         return false;
      }
   }
   if (!happyShelf2(svs, dim)) {
      return false;
   }
   return true;
}

/* Returns the index of the right most free cell in a shelf
   that isnt full */
int rightCell(char svs[HEIGHT][WIDTH], int h, Dimensions dim)
{
   int w;
   int index = 0;

   for (w = 0; w < dim.width; w++) {
      if (svs[h][w] == EMPTY) {
         index = w;
         return index;
      }
   }
   return index;
}

int shelfWrap(int h, Dimensions dim)
{
   int h2;

   h2 = ((h == dim.height - 1) ? 0:(h + 1));
   return h2;
}

void copyShelves(char svs2[HEIGHT][WIDTH], char svs[HEIGHT][WIDTH], Dimensions dim)
{
   int h, w;

   for (h = 0; h < dim.height; h++) {
      for (w = 0; w < dim.width; w++) {
         svs2[h][w] = svs[h][w];
      }
   }
}

void createChild(Bookcase bcArr[MAXBC], int c, int p, int h2, int w2, int h1, int w1, Dimensions dim)
{
   bcArr[c].parent = p;

   copyShelves(bcArr[c].svs, bcArr[p].svs, dim);
   bcArr[c].svs[h2][w2] = bcArr[p].svs[h1][w1];
   bcArr[c].svs[h1][w1] = EMPTY;
}

bool dupeShelves(Bookcase bcArr[MAXBC], int c, Dimensions dim)
{
   int i;
   char str1[MAXSTR];
   char str2[MAXSTR];

   stringify(bcArr[c].svs, str1, dim);
   for (i = 0; i < c; i++) {
      stringify(bcArr[i].svs, str2, dim);
      if (strcmp(str1, str2) == 0) {
         clearShelves(bcArr, c, dim);
         return true;
      }
   }
   return false;
}

void clearShelves(Bookcase bcArr[MAXBC], int c, Dimensions dim)
{
   int h, w;

   bcArr[c].parent = 0;
   for (h = 0; h < dim.height; h++) {
      for (w = 0; w < dim.width; w++) {
         bcArr[c].svs[h][w] = EMPTY;
      }
   }
}

int bruteForce(Bookcase bcArr[MAXBC], int c, int p, Dimensions dim)
{
   int h, w, h2, w2, cnt;
   for (h = 0; h < dim.height; h++) {
      if (!isShelfEmpty(bcArr[p].svs, h, dim)) {
         cnt = 0;
         w = dim.width - 1;
         while (cnt != 1 && w >= 0) {
            if (isBook(bcArr[p].svs, h, w, dim)) {
               h2 = shelfWrap(h, dim);
               while (h2 != h) {
                  if (!isShelfFull(bcArr[p].svs, h2, dim)) {
                     w2 = rightCell(bcArr[p].svs, h2, dim);
                     createChild(bcArr, c+1, p, h2, w2, h, w, dim);
                     if (!dupeShelves(bcArr, c+1, dim)) {
                        c++;
                     }
                  }
                  h2 = shelfWrap(h2, dim);
               }
               cnt++;
            }
            w--;
         }
      }
   }
   return c;
}

void printRoute(Bookcase bcArr[MAXBC], int route[MAXCH], int cnt, Dimensions dim)
{
   int i;
   for (i = cnt-1; i >= 0; i--) {
      if (route[i] != NOPARENT) {
         printBookcase(bcArr[route[i]].svs, dim);
      }
   }
}

int runHappy(Bookcase bcArr[MAXBC], int route[MAXCH], Dimensions dim)
{
   int i = 0;
   int c = 0;
   int p = 0;
   int cnt = 0;
   while ((!happyBookcase(bcArr[i++].svs, dim)) || (i != MAXBC)) {
      c = bruteForce(bcArr, c, p++, dim);
      if (happyBookcase(bcArr[i].svs, dim)) {
         route[cnt++] = i;
         p = bcArr[i].parent;
         route[cnt++] = p;
         while (p != NOPARENT) {
            p = bcArr[p].parent;
            route[cnt++] = p;
         }
         return cnt;
      }
   }
   fprintf(stderr, "No solution?\n");
   exit(EXIT_FAILURE);
}

void stringify(char svs[HEIGHT][WIDTH], char string[MAXSTR], Dimensions dim)
{
   int i, h, w;

   i = 0;
   for (h = 0; h < dim.height; h++) {
      for (w = 0; w < dim.width; w++) {
         string[i++] = svs[h][w];
      }
   }
   string[i] = '\0';
}

bool boundaryCheck(Dimensions dim)
{
   if (dim.height > 0 && dim.height <= HEIGHT &&
       dim.width  > 0 && dim.width  <= WIDTH) {
      return true;
   }
   return false;
}

void printBookcase(char svs[HEIGHT][WIDTH], Dimensions dim)
{
   int h, w;
   for (h = 0; h < dim.height; h++) {
      for (w = 0; w < dim.width; w++) {
         neillfgcol(white);
         printf("|");
         printColour(svs, h, w);
      }
      neillfgcol(white);
      printf("|\n");
   }
   printf("\n");
}

void printColour(char svs[HEIGHT][WIDTH], int h, int w)
{
   switch(svs[h][w]) {
      case BLACK:
         neillbgcol(white);
         neillfgcol(black);
         printf("%c", toupper(svs[h][w]));
         neillbgcol(black);
         neillfgcol(white);
         break;
      case RED:
         neillbgcol(black);
         neillfgcol(red);
         printf("%c", toupper(svs[h][w]));
         break;
      case GREEN:
         neillbgcol(black);
         neillfgcol(green);
         printf("%c", toupper(svs[h][w]));
         break;
      case YELLOW:
         neillbgcol(black);
         neillfgcol(yellow);
         printf("%c", toupper(svs[h][w]));
         break;
      case BLUE:
         neillbgcol(black);
         neillfgcol(blue);
         printf("%c", toupper(svs[h][w]));
         break;
      case MAGNETA:
         neillbgcol(black);
         neillfgcol(magenta);
         printf("%c", toupper(svs[h][w]));
         break;
      case CYAN:
         neillbgcol(black);
         neillfgcol(cyan);
         printf("%c", toupper(svs[h][w]));
         break;
      case WHITE:
         neillbgcol(black);
         neillfgcol(white);
         printf("%c", toupper(svs[h][w]));
         break;
      default:
         neillbgcol(black);
         neillfgcol(white);
         printf("%c", toupper(svs[h][w]));
         break;
   }
}

void test()
{
   int h;
   Dimensions dim;
   char stringShelf1 [MAXSTR];
   char stringShelf2 [MAXSTR];
   char strcmp1      [MAXSTR] = "......kck";
   char strcmp2      [MAXSTR] = ".........";
   char shelfCopy1   [MAXSTR];
   char shelfCopy2   [MAXSTR];
   char parentStr    [MAXSTR];
   char childStr1    [MAXSTR];
   char childStr2    [MAXSTR] = "kkk.w...g";
   char bruteForceP  [MAXSTR];
   char bruteForceC1 [MAXSTR];
   char bruteForceC2 [MAXSTR];
   char bruteForceC3 [MAXSTR];
   char bruteForceC4 [MAXSTR];
   char testShelvesP [MAXSTR] = "y..kkyyk.";
   char testShelvesC1[MAXSTR] = "...kkyyky";
   char testShelvesC2[MAXSTR] = "y..kk.yky";
   char testShelvesC3[MAXSTR] = "yy.kk.yk.";
   char testShelvesC4[MAXSTR] = "yk.kkyy..";
   char happyEndShelf[MAXSTR] = "yyykkk...";
   char isValidStr   [MAXSTR] = "y..b..x@^";
   char happyEndComp [MAXSTR];
   char clearedBc    [MAXSTR];
   int  route[MAXCH];
   Bookcase* bcArr1 = (Bookcase*) calloc(1000, sizeof(Bookcase));
   Bookcase* bcArr2 = (Bookcase*) calloc(1000, sizeof(Bookcase));
   Bookcase* bcArr3 = (Bookcase*) calloc(1000, sizeof(Bookcase));
   /*Bookcase bcArr3[MAXBC2];*/
   char happyBookcase1[HEIGHT][WIDTH] = {{BLACK, BLACK, BLACK},
                                         {CYAN,  CYAN,  EMPTY},
                                         {BLUE,  EMPTY, EMPTY}};
   char happyBookcase2[HEIGHT][WIDTH] = {{BLUE,  BLUE,  EMPTY},
                                         {EMPTY, EMPTY, EMPTY},
                                         {CYAN,  CYAN,  CYAN}};
   char sadBookcase1[HEIGHT][WIDTH]   = {{BLUE,  BLUE,  BLUE},
                                         {BLUE,  EMPTY, EMPTY},
                                         {CYAN,  CYAN,  CYAN}};
   char sadBookcase2[HEIGHT][WIDTH]   = {{BLUE,  BLUE,  BLUE},
                                         {EMPTY, CYAN,  CYAN},
                                         {CYAN,  CYAN,  EMPTY}};
   char sadBookcase3[HEIGHT][WIDTH]   = {{BLUE,  BLUE,  EMPTY},
                                         {RED,   RED,   EMPTY},
                                         {CYAN,  EMPTY, CYAN}};
   char sadBookcase4[HEIGHT][WIDTH]   = {{BLUE,  BLUE,  EMPTY},
                                         {BLACK, BLACK, BLACK},
                                         {BLUE,  EMPTY, EMPTY}};
   char sadBookcase5[HEIGHT][WIDTH]   = {{RED,   RED,   EMPTY},
                                         {RED,   EMPTY, EMPTY},
                                         {BLUE,  BLUE,  BLUE}};

   dim.height = HEIGHT + 1;
   dim.width  = WIDTH  + 1;
   assert(!boundaryCheck(dim));
   dim.height = -1;
   dim.width  = -1;
   assert(!boundaryCheck(dim));
   dim.height = HEIGHT;
   dim.width  = WIDTH;
   assert( boundaryCheck(dim));
   /* Test initBookcase function */
   initBookcase(bcArr1, 1000, dim);
   assert(bcArr1[0].svs[0][0] == EMPTY);
   assert(bcArr1[0].parent        == NOPARENT);
   assert(bcArr1[1].parent        == 0);
/* Setup and test isBook function */
   bcArr1[0].svs[0][0] = BLACK;
   bcArr1[1].svs[3][3] = CYAN;
   bcArr1[2].svs[4][4] = MAGNETA;
   bcArr1[3].svs[1][8] = 'p';
   assert( isBook(bcArr1->svs,   0, 0, dim));
   assert( isBook(bcArr1[1].svs, 3, 3, dim));
   assert( isBook(bcArr1[2].svs, 4, 4, dim));
   assert(!isBook(bcArr1[3].svs, 1, 8, dim));
   assert(!isBook(bcArr1[3].svs, 9, 9, dim));

   dim.height = 3;
   dim.width  = 3;
   /* Setup and test isShelfEmpty function */
   initBookcase(bcArr2, 1000, dim);
   bcArr2[0].svs[2][0] = BLACK;
   bcArr2[1].svs[2][0] = CYAN;
   assert( isShelfEmpty(bcArr2->svs,   0, dim));
   assert( isShelfEmpty(bcArr2->svs,   1, dim));
   assert(!isShelfEmpty(bcArr2->svs,  -1, dim));
   assert(!isShelfEmpty(bcArr2->svs,   2, dim));
   assert(!isShelfEmpty(bcArr2[1].svs, 2, dim));
   assert(!isShelfEmpty(bcArr2[1].svs, 9, dim));

/* Setup and test isShelfFull function */
   bcArr2[0].svs[2][0] = BLACK;
   bcArr2[0].svs[2][1] = CYAN;
   bcArr2[0].svs[2][2] = BLACK;
   assert( isShelfFull(bcArr2->svs,   2, dim));
   assert(!isShelfFull(bcArr2->svs,  -1, dim));
   assert(!isShelfFull(bcArr2->svs,   1, dim));
   assert(!isShelfFull(bcArr2->svs,   4, dim));

/* Setup and test stringify function */
   stringify(bcArr2[0].svs, stringShelf1, dim);
   stringify(bcArr2[2].svs, stringShelf2, dim);
   assert(strcmp(stringShelf1, strcmp1) == 0);
   assert(strcmp(stringShelf2, strcmp2) == 0);

/* Set bcArr4 to empty, and add books for colourMatch and happyShelf function tests */
   bcArr2[2].svs[0][0] = BLACK;
   bcArr2[2].svs[0][1] = BLACK;
   bcArr2[2].svs[0][2] = BLACK;
   bcArr2[2].svs[1][1] = WHITE;
   bcArr2[2].svs[1][2] = GREEN;
   assert( colourMatch(bcArr2[2].svs, 0, 0, 0, 1, dim));
   assert( colourMatch(bcArr2[2].svs, 0, 1, 0, 2, dim));
   assert(!colourMatch(bcArr2[2].svs, 1, 1, 1, 2, dim));
   assert(!colourMatch(bcArr2[2].svs, 1, 2, 2, 0, dim));
   assert(!colourMatch(bcArr2[2].svs, 2, 1, 2, 1, dim));
/* Test happyShelf function with already set up bcArr4  */
   assert( happyShelf(bcArr2[2].svs, 0, dim));
   assert( happyShelf(bcArr2[2].svs, 2, dim));
   assert(!happyShelf(bcArr2[2].svs, 1, dim));

/* Test colourCount function for use in happyBookcase function */
   assert( colourCount(happyBookcase1, dim));
   assert( colourCount(happyBookcase2, dim));
   assert(!colourCount(sadBookcase1,   dim));
   assert(!colourCount(sadBookcase2,   dim));
/* Test happyBookPos function for use in happyBookcase function */
   assert( happyBookPos(happyBookcase1, dim));
   assert( happyBookPos(happyBookcase2, dim));
   assert(!happyBookPos(sadBookcase2,   dim));
   assert(!happyBookPos(sadBookcase3,   dim));

/* Test happyShelf2 function for use in happyBookcase function */
   assert( happyShelf2(happyBookcase1,   dim));
   assert( happyShelf2(happyBookcase2,   dim));
   assert(!happyShelf2(sadBookcase4,   dim));
   assert(!happyShelf2(sadBookcase5,   dim));

/* Check if the preset happyBookcases are happy and test happyBookcase function */
   happyBookcase(happyBookcase1, dim);
   assert( happyBookcase(happyBookcase1, dim));
   assert( happyBookcase(happyBookcase2, dim));
   assert(!happyBookcase(sadBookcase1,   dim));
   assert(!happyBookcase(sadBookcase2,   dim));
   assert(!happyBookcase(sadBookcase3,   dim));
   assert(!happyBookcase(sadBookcase4,   dim));

/* Test that the rightCell functions gives last available cell in shelf */
   assert( rightCell(happyBookcase1, 0, dim) == 0);
   assert( rightCell(happyBookcase1, 1, dim) == 2);
   assert( rightCell(happyBookcase1, 2, dim) == 1);
   assert((!rightCell(happyBookcase1, 1, dim)) == 0);

/* Test that the copyShelves functions works by comparing the string values */
   copyShelves(bcArr2[3].svs, bcArr2[2].svs, dim);
   stringify(bcArr2[2].svs, shelfCopy1, dim);
   stringify(bcArr2[3].svs, shelfCopy2, dim);
   assert( strcmp(shelfCopy1, shelfCopy2) == 0);
   assert((!strcmp(shelfCopy1, strcmp1))  == 0);

/* Test shelfwrap function - function returns shelf refrence to 0 if it reaches end of svs*/
   h = 1;
   assert(shelfWrap(h, dim) == 2);
   h = 2;
   assert(shelfWrap(h, dim) == 0);
   assert(shelfWrap(h, dim) != 3);

/* Test createChild - should move parent book pos[1][2] to pos[2][2] and save as new, child bookcase.
   Test simulates parent number as 3 and child number as 4
   Check with stringify and strcmp that child only 1 different to parent */
   createChild(bcArr2, 4, 3, 2, 2, 1, 2, dim);
   stringify(bcArr2[3].svs, parentStr, dim);
   stringify(bcArr2[4].svs, childStr1, dim);
   assert( strcmp(childStr1, childStr2) == 0);
   assert((!strcmp(parentStr, childStr1)) == 0);

/* Test bruteForce function - should save the first 4 children of initial bcArr3 shelf arrangment
   Compare the outcome by stringifying the arrays and comparing to expected outcomes */
   initBookcase(bcArr3, 1000, dim);
   bcArr3[0].svs[0][0] = YELLOW;
   bcArr3[0].svs[1][0] = BLACK;
   bcArr3[0].svs[1][1] = BLACK;
   bcArr3[0].svs[1][2] = YELLOW;
   bcArr3[0].svs[2][0] = YELLOW;
   bcArr3[0].svs[2][1] = BLACK;
   bruteForce(bcArr3, 0, 0, dim);
   stringify(bcArr3[0].svs, bruteForceP,  dim);
   stringify(bcArr3[1].svs, bruteForceC1, dim);
   stringify(bcArr3[2].svs, bruteForceC2, dim);
   stringify(bcArr3[3].svs, bruteForceC3, dim);
   stringify(bcArr3[4].svs, bruteForceC4, dim);
   assert(strcmp(bruteForceP,  testShelvesP)  == 0);
   assert(strcmp(bruteForceC1, testShelvesC1) == 0);
   assert(strcmp(bruteForceC2, testShelvesC2) == 0);
   assert(strcmp(bruteForceC3, testShelvesC3) == 0);
   assert(strcmp(bruteForceC4, testShelvesC4) == 0);

/* Reset bcArr3 and test the runHappy function - will find the solution for the below initial shelf
   Compare outcome of runHappy with the expected happy outcome of initial arrangment */
   initBookcase(bcArr3, 1000, dim);
   bcArr3[0].svs[0][0] = YELLOW;
   bcArr3[0].svs[1][0] = BLACK;
   bcArr3[0].svs[1][1] = BLACK;
   bcArr3[0].svs[1][2] = YELLOW;
   bcArr3[0].svs[2][0] = YELLOW;
   bcArr3[0].svs[2][1] = BLACK;
   runHappy(bcArr3, route, dim);
   stringify(bcArr3[route[0]].svs, happyEndComp, dim);
   assert(strcmp(happyEndComp, happyEndShelf) == 0);

/* Test dupeShelves function and clearShelves. Set up bcArr3[1] as 1 move different
   vs bcArr[0] and then setup bcArr[2] the same as bcArr3[1]. Should fail.
   Then strcmp cleared bcArr[2] with empty string to show clear works */
   bcArr3[1].svs[0][0] = EMPTY;
   bcArr3[1].svs[0][1] = EMPTY;
   bcArr3[1].svs[0][2] = EMPTY;
   bcArr3[1].svs[1][0] = BLACK;
   bcArr3[1].svs[1][1] = BLACK;
   bcArr3[1].svs[1][2] = YELLOW;
   bcArr3[1].svs[2][0] = YELLOW;
   bcArr3[1].svs[2][1] = BLACK;
   bcArr3[1].svs[2][2] = BLACK;
   bcArr3[2].svs[0][0] = EMPTY;
   bcArr3[2].svs[0][1] = EMPTY;
   bcArr3[2].svs[0][2] = EMPTY;
   bcArr3[2].svs[1][0] = BLACK;
   bcArr3[2].svs[1][1] = BLACK;
   bcArr3[2].svs[1][2] = YELLOW;
   bcArr3[2].svs[2][0] = YELLOW;
   bcArr3[2].svs[2][1] = BLACK;
   bcArr3[2].svs[2][2] = BLACK;
   assert(!dupeShelves(bcArr3, 1, dim));
   assert( dupeShelves(bcArr3, 2, dim));
   stringify(bcArr3[2].svs, clearedBc, dim);
   assert(strcmp(clearedBc, ".........") == 0);

/* Test isValid function - shouldnt pass for non pre-defined inputs */
   assert( isValid(isValidStr, 0));
   assert( isValid(isValidStr, 3));
   assert(!isValid(isValidStr, 6));
   assert(!isValid(isValidStr, 7));
   assert(!isValid(isValidStr, 8));

/* Test the function that checks the starting board format's width matches
   the pre-set width at the top of files. Can't test false as EXIT_FAILURE */
   assert(bcWidthCheck(1, dim) == 0);
   assert(bcWidthCheck(2, dim) == 0);
   assert(bcWidthCheck(3, dim) == 0);

/* Test testInit function by checking that bcArr3 passes and isn't classified
   as having a full starting bookcase, and therefore can be made happy. */
   assert(testInit(bcArr3, dim) == 0);

   free(bcArr1);
   free(bcArr2);
   free(bcArr3);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* 4 Letters and 1 '\0' */
#define MAXLEN 5
#define IDENT 0
#define MAXPERM 24
#define LISTLEN 5000

typedef enum bool {false, true} bool;

struct llist {
   char word[MAXLEN];
   struct llist *next;
};

void permute(char wordChange[MAXLEN], char endWord[MAXLEN], char *wordList[LISTLEN], int len);
void lowerCase(char firstWord[MAXLEN], char secondWord[MAXLEN]);
bool wordCheck(char wordChange[MAXLEN], char *wordList[LISTLEN], int len);
int editDist(char firstWord[MAXLEN], char secondWord[MAXLEN]);
int linkList(const char *filename, char *wordList[MAXLEN]);
/*void swapLetters(char startWord[MAXLEN], char endWord[MAXLEN], int i); */

int main(int argc, char *argv[MAXLEN])
{
   int i, len;
   char inWord1[MAXLEN];
   char inWord2[MAXLEN];
   char *wordList[LISTLEN] = {NULL};

   strcpy(inWord1, argv[2]);
   strcpy(inWord2, argv[3]);
   printf("Start: %s\nEnd: %s\n", inWord1, inWord2);

   if (argc == 4) {
      len  = linkList(argv[1], wordList);
      permute(inWord1, inWord2, wordList, len);
   }
   else {
      printf("Incorrect number of inputs.\n");
   }

   for (i = 0; i < LISTLEN; i++) {
      free(wordList[i]);
   }
   return 0;
}

void permute(char wordChange[MAXLEN], char endWord[MAXLEN], char *wordList[LISTLEN], int len)
{
   int i, iter;
   char tmp;

   lowerCase(wordChange, endWord);
   iter = 0;
   while (editDist(wordChange, endWord) != IDENT && iter < MAXPERM) {
      for (i = 0; i < 4; i++) {
         if (wordChange[i] != endWord[i]) {
            tmp = wordChange[i];
            wordChange[i] = endWord[i];
            if (wordCheck(wordChange, wordList, len)) {
               printf("Pos[%d] Changed - %s\n", i, wordChange);
            }
            else {
               wordChange[i] = tmp;
            }
         }
      }
      iter++;
   }
   if (iter == MAXPERM) {
      printf("Cannot make word ladder.\n");
   }
   else {
      printf("Word ladder finished\n");
   }
}

void lowerCase(char firstWord[MAXLEN], char secondWord[MAXLEN])
{
   int i;

   for (i = 0; i < MAXLEN - 1; i++) {
      tolower(firstWord[i]);
      tolower(secondWord[i]);
   }
}

int editDist(char firstWord[MAXLEN], char secondWord[MAXLEN])
{
   int i, dist;

   dist = 0;
   for (i = 0; i < MAXLEN; i++) {
      if (tolower(firstWord[i]) != tolower(secondWord[i])) {
         dist++;
         /*         printf("Dist[%d]: %d\n", i, dist);*/
      }
   }
   if (dist >= 0 && dist <= MAXLEN) {
      return dist;
   }
   else {
      printf("Distance calc failed.\n");
      return 0;
   }
}

bool wordCheck(char wordChange[MAXLEN], char *wordList[LISTLEN], int len)
{
   int i = 0;

   for (i = 0; i < len; i++) {
      if (strcmp(wordList[i], wordChange) == 0) {
         return true;
      }
   }
   return false;
}

int linkList(const char *filename, char *wordList[MAXLEN])
{
   int i;
   FILE *file;
   struct llist *head, *cp;

   if ((file = fopen(filename, "r")) == NULL) {
      fprintf(stderr, "Failed to load file.\n");
      exit(EXIT_FAILURE);
   }

   cp = head = calloc(1, sizeof(struct llist));
   if (head == NULL) {
      printf("Failed to alloc memory.\n");
      exit(EXIT_FAILURE);
   }
   while (fscanf(file, "%s", cp->word) != EOF) {
      cp->next = calloc(1, sizeof(struct llist));
      cp = cp->next;
   }

   i = 0;
   cp = head;
   while (cp->next != NULL) {
      wordList[i] = cp->word;
      cp = cp->next;
      i++;
   }
   fclose(file);
   return i;
}

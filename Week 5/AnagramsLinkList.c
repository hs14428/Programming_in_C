#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLEN 50
#define ALPHANUM 26
#define DICTLEN 400000

typedef enum bool {false, true} bool;

struct llist {
   char word[MAXLEN];
   struct llist *next;
};

void makeHist(char inWord[MAXLEN], int outHist[ALPHANUM], int histSize);
bool histSame(int userHist[ALPHANUM], int dictHist[ALPHANUM], int histSize);
long linkList(const char *filename, char* llWord[MAXLEN]);

int main(int argc, char* argv[MAXLEN])
{
   int   i;
   long len;
   int   userHist[ALPHANUM] = {0};
   int   dictHist[ALPHANUM] = {0};
   char* wordList[DICTLEN] = {NULL};

   makeHist(argv[1], userHist, ALPHANUM);
   len = linkList(argv[2], wordList);
   printf("length of dict: %ld\n", len);

   printf("Anagrams of %s: \n", argv[1]);
   for (i = 0; i < len; i++) {
      makeHist(wordList[i], dictHist, ALPHANUM);
      if (histSame(userHist, dictHist, ALPHANUM)) {
         printf("%s\n", wordList[i]);
      }
   }

   for (i = 0; i < DICTLEN; i++) {
        free(wordList[i]);
   }
   return 0;
}

void makeHist(char inWord[MAXLEN], int outHist[ALPHANUM], int histSize)
{
   int i = 0;
/*
   printf("Size inWord %ld\n", sizeof(inWord));
   printf("Size outHist %ld\n", sizeof(outHist));
*/
   for (i = 0; i < histSize; i++) {
      outHist[i] = 0;
   }

   i = 0;
   while (inWord[i] != '\0') {
      outHist[inWord[i] - 'a']++;
      i++;
   }
}

bool histSame(int userHist[MAXLEN], int dictHist[MAXLEN], int histSize)
{
   int i, cnt = 0;

   for (i = 0; i < histSize; i++) {
      if (userHist[i] == dictHist[i]) {
         cnt++;
      }
   }
   if (cnt == histSize) {
      return true;
   }
   else {
      return false;
   }
}

long linkList(const char *filename, char* llWord[MAXLEN])
{
   long i;
   FILE *file;
   struct llist *head, *cp;

   if ((file = fopen(filename, "r")) == NULL) {
      fprintf(stderr, "Failed to load file.\n");
      exit(EXIT_FAILURE);
   }

   cp = head = calloc(1, sizeof(struct llist));
   if (head == NULL) {
      fprintf(stderr, "Failed to alloc memory.\n");
      exit(EXIT_FAILURE);
   }
   while (fscanf(file, "%s", cp->word) != EOF) {
      cp->next = calloc(1, sizeof(struct llist));
      cp = cp->next;
   }

   i = 0;
   cp = head;
   while (cp->next != NULL) {
      llWord[i] = cp->word;
/*      printf("llWord: %s\n", llWord[i]);*/
      cp = cp->next;
      i++;
   /*   printf("cnt: %d\n", i);*/
   }

   fclose(file);
   return i;
}

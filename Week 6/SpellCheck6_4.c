#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define MAXLEN 50
#define DICTLEN 400000

typedef enum bool {false, true} bool;

struct llist {
   char word[MAXLEN];
   struct llist *next;
};

int binaryIter(char inWord[MAXLEN], char *words[MAXLEN], int l, int r);
void swapStr(char *words[MAXLEN], int i, int j);
void sortList(char *words[MAXLEN], int len);
int getWords(const char *filename, char* words[MAXLEN]);

int main(int argc, char* argv[MAXLEN])
{
   int i, len;
   char inWord[MAXLEN];
   char* words[DICTLEN] = {NULL};

   if (argc != 3) {
      fprintf(stderr, "Not enough command line arguments.\n");
      exit(EXIT_FAILURE);
   }

   strcpy(inWord, argv[2]);
   len = getWords(argv[1], words);
   sortList(words, len);
   binaryIter(inWord, words, 0, len-1);

   for (i = 0; i < DICTLEN; i++) {
        free(words[i]);
   }

   return 0;
}

int binaryIter(char inWord[MAXLEN], char *words[MAXLEN], int l, int r)
{
   int mid;
   printf("inword: %s\n", inWord);

   while (l <= r) {
      mid = (l + r)/2;
      if (strcmp(inWord, words[mid]) == 0) {
         printf("Your word is in the list at pos: %d\n", mid);
         return 0;
      }
      else {
         if (strcmp(inWord, words[mid]) > 0) {
            l = mid + 1;
         }
         else {
            r = mid - 1;
         }
      }
   }
   return -1;
}

void sortList(char *words[MAXLEN], int len)
{
   int i, j;

   for (i = 0; i <= len; i++) {
      for (j = i + 1; j < len; j++) {
         if (strcmp(words[i], words[j]) > 0) {
            swapStr(words, i, j);
         }
      }
   }

   printf("length of dict: %d\n", len);

   printf("Sorted Dictionary:\n");
   for (i = 0; i < len; i++) {
      printf("%d: %s\n", i+1, words[i]);
   }
}

void swapStr(char *words[MAXLEN], int i, int j)
{
   char tmp[MAXLEN];

   strcpy(tmp, words[i]);
   strcpy(words[i], words[j]);
   strcpy(words[j], tmp);
}

int getWords(const char *filename, char* words[MAXLEN])
{
   FILE *file;
   int i;
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
      words[i] = cp->word;
      cp = cp->next;
      i++;
   }
   free(cp);
   fclose(file);
   return i;
}

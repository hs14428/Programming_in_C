#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define MAXLEN 50
#define DICTLEN 400000

typedef enum bool {false, true} bool;

int binaryIter(char inWord[MAXLEN], char words[DICTLEN][MAXLEN], int l, int r);
void swapStr(char words[DICTLEN][MAXLEN], int i, int j);
void sortList(char words[DICTLEN][MAXLEN], int len);
int getWords(const char *filename, char words[DICTLEN][MAXLEN]);

int main(int argc, char* argv[MAXLEN])
{
   int len;
   char inWord[MAXLEN];
   static char words[DICTLEN][MAXLEN];

   if (argc != 3) {
      fprintf(stderr, "Not enough command line arguments.\n");
      exit(EXIT_FAILURE);
   }

   strcpy(inWord, argv[2]);
   len = getWords(argv[1], words);
   sortList(words, len);
   binaryIter(inWord, words, 0, len - 1);

   return 0;
}

int binaryIter(char inWord[MAXLEN], char words[DICTLEN][MAXLEN], int l, int r)
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

void sortList(char words[DICTLEN][MAXLEN], int len)
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

void swapStr(char words[DICTLEN][MAXLEN], int i, int j)
{
   char tmp[MAXLEN];

   strcpy(tmp, words[i]);
   strcpy(words[i], words[j]);
   strcpy(words[j], tmp);
}

int getWords(const char *filename, char words[DICTLEN][MAXLEN])
{
   FILE *file;
   int i;
   char str[MAXLEN];

   if ((file = fopen(filename, "r")) == NULL) {
      fprintf(stderr, "Failed to load file.\n");
      exit(EXIT_FAILURE);
   }

   i=0;
   while (!feof(file)) {
      fscanf(file, "%s", str);
      strcpy(words[i], str);
      i++;
   }
   fclose(file);
   return i;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLEN 50
#define DICTLEN 400000

typedef enum bool {false, true} bool;

void swapStr(char *str, int i, int j);
int getWords(const char *filename, char *words[DICTLEN]);

int main(int argc, char* argv[MAXLEN])
{
   int   i, j;
   long len;
   static char tmp[MAXLEN];
   char* words[DICTLEN] = {NULL};

   if (argc != 2) {
      return 0;
   }

   len = getWords(argv[1], words);
   printf("length of dict: %ld\n", len);

   for (i = 0; i <= len; i++) {
      for (j = i + 1; j < len; j++) {
         if (strcmp(words[i], words[j]) > 0) {
         /*   printf("words[%d] = %s <-> %s words[%d]\n", i, words[i], words[j], j);*/
            strcpy(tmp, words[i]);
            strcpy(words[i], words[j]);
            strcpy(words[j], tmp);
         }
      }
   }
   printf("Sorted Dictionary:\n");
   for (i = 0; i < len; i++) {
      printf("%d: %s\n", i+1, words[i]);
       free(words[i]);
   }
   return 0;
}

int getWords(const char *filename, char *words[DICTLEN])
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
      words[i] = (char*) malloc(MAXLEN);
      strcpy(words[i], str);
      i++;
   }
   fclose(file);
   return i;
}

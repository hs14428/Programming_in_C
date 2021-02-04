#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define ALPHANUM 26
#define MAXLEN 30
#define LISTLEN 1500

int isogram(char *s);
int longIso(const char *filename);

int main(int argc, char *argv[MAXLEN])
{
   assert(isogram("programming") == 0);
   assert(isogram("housewarmings") == 0);
   assert(isogram("abductions") == 1);
   assert(isogram("housewarming") == 1);
   assert(isogram("hydromagnetics") == 1);
   assert(isogram("uncopyrightable") == 1);

   longIso(argv[1]);

   return 0;
}

int isogram(char *s)
{
   int i, j;
   int len;
   int hist[ALPHANUM] = {0};

   len = strlen(s);
   for (i = 0; i < len; i++) {
      for (j = 'a'; j <= 'z'; j++) {
         if (s[i] == j) {
            hist[j-'a']++;
         }
      }
   }
   for (i = 0; i < ALPHANUM; i++) {
      if (hist[i] > 1) {
         return 0;
      }
   }
   return 1;
}

int longIso(const char *filename)
{
   FILE *file;
   int i, n, j, max, maxidx;
   char str[MAXLEN];
   char wordList[LISTLEN][MAXLEN];

   file = fopen(filename, "r");

   i = 0;
   while (fscanf(file, "%s", str) != EOF) {
      strcpy(wordList[i], str);
      i++;
   }
   fclose(file);

   max = strlen(wordList[0]);
   maxidx = 0;
   for (j = 0; j < i; j++) {
      if (isogram(wordList[j])) {
         n = strlen(wordList[i]);
         if (n > max) {
            max = n;
            maxidx = j;
         }
      }
   }
   printf("%s (%d)\n", wordList[maxidx], max);
   return 0;
}

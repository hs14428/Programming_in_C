#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define MAXLEN 50
#define DICTLEN 400000

typedef enum bool {false, true} bool;

typedef struct entry {
   char word[MAXLEN];
   struct entry* next;
} Entry;

typedef struct dictionary {
   Entry* start;
   int length;
} Dictionary;

void printArray(Dictionary* dictionary);
void sortWord   (Dictionary* dictionary, char* str);
void getWords   (const char *filename, Dictionary* dictionary);
void freeSpace(Dictionary* dictionary);

int binaryIter(char inWord[MAXLEN], Dictionary* dictionary, int l, int r);

int main(int argc, char* argv[MAXLEN])
{
/*   char inWord[MAXLEN];*/
   Dictionary* dictionary = (Dictionary*) calloc(1, sizeof(Dictionary));

   if (dictionary == NULL) {
      fprintf(stderr, "Unable to allocate space.\n");
      exit(EXIT_FAILURE);
   }

   if (argc <= 2) {
      fprintf(stderr, "Not enough command line arguments.\n");
      exit(EXIT_FAILURE);
   }

/*   strcpy(inWord, argv[2]);*/
   getWords(argv[1], dictionary);
   printArray(dictionary);
   freeSpace(dictionary);

/* binaryIter(inWord, words, 0, len-1); */
   return 0;
}

void sortWord(Dictionary* dictionary, char* str)
{
   Entry* current, *tmp;
   current = dictionary->start;

/* If the word is at the beginning (str alphabetically before current->word): */
   if (strcmp(current->word, str) > 0) {
      tmp = (Entry*) calloc(1, sizeof(Entry));
      strcpy(tmp->word, str);
      tmp->next = current;
      dictionary->start = tmp;
      /* Optional */
      dictionary->length++;
      return;
   }

/* If the word is in the middle: */
   while (current->next != NULL) {
      if (strcmp(current->next->word, str) > 0) {
         tmp = (Entry*) calloc(1, sizeof(Entry));
         strcpy(tmp->word, str);
         tmp->next = current->next;
         current->next = tmp;
         /* Optional */
         dictionary->length++;
         return;
      }
/* Linked list equivalent of i++ */
      current = current->next;
   }

/* If the word is at the end: */
   tmp = (Entry*) calloc(1, sizeof(Entry));
   strcpy(tmp->word, str);
   tmp->next = NULL;
   current->next = tmp;
   /* Optional */
   dictionary->length++;
}

void getWords(const char *filename, Dictionary* dictionary)
{
   char str[MAXLEN];
   FILE *file;
   Entry* current;

   if ((file = fopen(filename, "r")) == NULL) {
      fprintf(stderr, "Failed to load file.\n");
      exit(EXIT_FAILURE);
   }

   fgets(str, MAXLEN, file);
   str[strlen(str) - 1] = '\0';
   current = (Entry*) calloc(1, sizeof(Entry));
   strcpy(current->word, str);
   dictionary->start = current;
   dictionary->length = 1;

   while (fgets(str, MAXLEN, file)) {
      str[strlen(str) - 1] = '\0';
      sortWord(dictionary, str);
   }
   fclose(file);
}

void printArray(Dictionary* dictionary)
{
   Entry* current;
   current = dictionary->start;

   printf("\n");
   while (current != NULL) {
      printf("%s\n", current->word);
      current = current->next;
   }
}

void freeSpace(Dictionary* dictionary)
{
   Entry *current, *tmp;
   current = dictionary->start;
   while(current != NULL) {
      tmp = current;
      current = current->next;
      free(tmp);
   }
   free(dictionary);
}
/*
int binaryIter(char inWord[MAXLEN], Dictionary* dictionary, int l, int r)
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
*/

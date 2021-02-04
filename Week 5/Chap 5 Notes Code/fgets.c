#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLEN 400000
#define SIZE 1000

int main(void)
{
   FILE *file;
   int i, n;
   char cell;
   char* words[400000] = {NULL};
   char str[SIZE];

   if ((file = fopen("eng_370k_shuffle.txt", "r")) == NULL) {
      fprintf(stderr, "Failed to load file.\n");
      exit(EXIT_FAILURE);
   }
   do {
      fgets(str, SIZE, file);
      n = strlen(str);
      words[i] = (char*)malloc(n + 1);
      strcpy(words[i], str);
      i++;
      printf("test");
   } while (fgets(str, SIZE, file) != NULL);

   for (i = 0; i < 400000; i++) {
      printf("%s", words[i]);
   }
   return 0;
}


/*i = 0;
while ((cell = fgetc(file)) != EOF) {
   if (cell != '\n') {
      buffer[i] = cell;
      i++;
   }
   /*printf("%d\n", cell);
}
buffer[i] = '\0';*/

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "common.h"

#define MAXLEN 50

char* strnodupes(const char* s)
{
   int i, j, k;
   int n = strlen(s);
   char *buffer = (char*)malloc((n+1)*sizeof(char*));

   printf("n:%d\n", n);
   for (i = 0; i < n; i++) {
      buffer[i] = s[i];
      printf("%c", buffer[i]);
   }
   buffer[i] = '\0';

   printf("\n");
   for (i = 0; i < n; i++) {
      if (buffer[i] != '*') {
         for (j = i+1; j < n; j++) {
            if (buffer[i] == buffer[j]) {
               buffer[j] = '*';
            }
         }
      }
   }
   k = 0;
   for (i = 0; i < n; i++) {
      buffer[i] = buffer[i + k];
      if (buffer[i] == '*') {
         k++;
         i--;
      }
   }
   printf("%s\n",buffer);
   free(buffer);
   return buffer;
}

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "common.h"

bool struses(const char* s1, const char* s2)
{
   int i, j, cnt;
   int len1, len2;

   len1 = strlen(s1);
   len2 = strlen(s2);

   cnt = 0;
   for (i = 0; i < len1; i++) {
      for (j = 0; j < len2; j++) {
         if (tolower(s1[i]) == tolower(s2[j])) {
            cnt++;
         }
      }
   }
   if (cnt == len1) {
      return true;
   }
   else {
      return false;
   }
}

#include "common.h"

void strflagvowel(const char* s1, char* s2)
{
   int i, j;
   int len;
   char tmp;

   len = strlen(s1);

   j = 0;
   for (i = 0; i < len; i++) {
      if (tolower(s1[i]) == 'a' || tolower(s1[i]) == 'e' || tolower(s1[i]) == 'i' ||
          tolower(s1[i]) == 'o' || tolower(s1[i]) == 'u') {
         tmp = s1[i];
         s2[j] = '*';
         j++;
         s2[j] = tmp;
         j++;
         s2[j] = '*';
         j++;
      }
      else {
         s2[j] = s1[i];
         j++;
      }
   }
   s2[j] = '\0';

}

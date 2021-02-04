#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LOWERCASEA 97
#define LOWERCASEZ 122

enum bool {false, true};
typedef enum bool bool;

bool vowel_check(char c);

int main(void)
{
   int cnt = 1;
   char i, j, k;

   for (i = LOWERCASEA; i <= LOWERCASEZ; i++) {
      for (j = LOWERCASEA; j <= LOWERCASEZ; j++) {
         for (k = LOWERCASEA; k <= LOWERCASEZ; k++) {
            if ((vowel_check(i) == false) && (vowel_check(j) == true) && (vowel_check(k) == false)) {
               printf("%d\t%c%c%c\n", cnt, i, j, k);
               cnt++;
            }
         }
      }
   }
   return 0;
}

bool vowel_check(char c)
{

   if (c == 'a' || c == 'e' || c == 'o' ||  c == 'i' || c == 'u') {
      return true;
   }
   else {
      return false;
   }
}

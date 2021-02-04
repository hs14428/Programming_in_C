/*
#include <stdio.h>
#include <ctype.h>

int isvowel();

int main(void)
{
   int c;

   while ((c = getchar()) != EOF) {
      isvowel(c);
   }
}

int isvowel(int v)
{
   v = getchar();

   if (v == 'a' || v == 'e' || v == 'o' || v== 'i' || v == 'u') {
      putchar(toupper(v));
   }
   else {
      putchar(v);
   }
   return 0;
}*/

#include <stdio.h>
#include <ctype.h>

char isvowel(char v);

int main(void)
{
   char c;

   while ((c = getchar()) != EOF) {
      isvowel(c);
   }
}

char isvowel(char v)
{

   if (v == 'a' || v == 'e' || v == 'o' || v== 'i' || v == 'u') {
      putchar(toupper(v));
   }
   else {
      putchar(v);
   }
   return 0;
}

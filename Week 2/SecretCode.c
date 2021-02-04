#include <stdio.h>
#include <ctype.h>

#define ADIFFERENCEZ 25
#define LOWERCASEA 97
#define LOWERCASEZ 122

/* int scode(int a); */
char scode2(char c);

int main(void)
{
   char c;

   while ((c = getchar()) != EOF) {
      printf("%c", scode((int) c));
   }
}

/*int scode(int a)
{
   char i;

   if (islower(a)) {
      return ((a + ADIFFERENCEZ) - (2*(a - (int) 'a')));
   }
   else {
      return ((a - ADIFFERENCEZ) - (2*(a - (int) 'A')));
   }
} */

char scode2(char c)
{
   if (islower(c)) {
      return 'z' - c + 'a';
   }
   if (isupper(c)) {
      return 'Z' - c + 'A';
   }
   return c;
}

void test(void)
{
   assert(scode2('a') == 'z');
   assert(scode2('z') == 'a');
   assert(scode2('A') == 'Z');
   assert(scode2('Z') == 'A');

   for(i =- 0; i < 100; i++) {
      c = rand()%26 +'a';
      assert(scode2(scode2()));
   }
}

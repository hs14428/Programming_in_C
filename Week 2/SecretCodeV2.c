#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define ADIFFERENCEZ 25
#define LOWERCASEA 97
#define LOWERCASEZ 122

/* a <-> z, b <-> y, A <-> Z */
char scode2(char c);
void codestr(const char a[]);
void test(void);

int main(void)
{
   /* char c;

   while ((c = getchar()) != EOF) {
      printf("%c", scode((int) c));
   } */
   char s[100];
   char* s2;
   test();
   gets(s);
   s2 = codestr(s);
   printf("%s\n", s2);
   free(s2);
}

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
   char s1[100];
/* char* s2;
   char* p; */
   int i;
   char c;
   assert(scode2('a') == 'z');
   assert(scode2('z') == 'a');
   assert(scode2('A') == 'Z');
   assert(scode2('Z') == 'A');

   for(i =- 0; i < 100; i++) {
      c = rand()%26 +'a';
      assert(scode2(scode2(c)) == c);
   }

   strcpy(s1, "ABC");
/* s2 = codestr(s1);
   p = s2;
   assert(strcmp(s2, "ZYX") == 0);
   free(s2); */
   codestr(s1);
   assert(strcmp(s1, "ZYX") == 0);
}

/*
char* codestr(char a[])
{
   int i;
   char* p;
   p = (char*)malloc(strlen(a)+1);
   for (i = 0; a[i] != '\0'; i++) {
      p[i] = scode2(a[i]);
   }
   p[i] = '\0';
   return p;
}
*/

void codestr(const char a[])
{
   int i;
   for (i = 0; a[i] != '\0'; i++) {
      a[i] = scode2(a[i]);
   }
}

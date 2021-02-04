#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#define ARRAYSIZE 50

typedef enum bool {false, true} bool;

void test(void);
int getString(char s[]);
int arrayLength(char s[]);
bool palindromeCheck(char s[], int cnt);

int main(void)
{
   int i, len;
   /*test();*/

   char phrase[ARRAYSIZE];
   printf("Please enter a word or phrase to test.\n");

   getString(phrase);
   len = arrayLength(phrase);

   if (palindromeCheck(phrase, len) == true) {
      printf("Yes, that is a palindrome.\n\n");
   }
   else {
      printf("Sorry, thats not a palindrome.\n\n");
   }

   return 0;
}

int getString(char s[])
{
   char c, j = 0;

   while ((c = getchar()) != EOF && c != '\n') {
      if (isalpha(c)) {
         s[j++] = tolower(c);
      }
   }
   s[j] = '\0';
}

int arrayLength(char s[])
{
   int cnt = 0;
   while (s[cnt]) {
      cnt++;
   }
   /*minus 1 to make 1=0 for referencing*/
   return cnt - 1;
}

bool palindromeCheck(char s[], int cnt)
{
   int i;

   for (i = 0; i <= cnt; i++) {
      if (s[i] != s[cnt - i]) {
         return false;
      }
   }
   return true;
}

void test(void)
{
   char s1[6] = {'K','a','y','a','k','\0'};
   int i, out, c1 = 4;

   (assert(palindromeCheck(s1, c1) == true));
}

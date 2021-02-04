/*
   Toggle the case (uppercase <-> lowercase ) of every
   occurrence of the character c in the in string s.
   A lowercase character becomes uppercase & vice-versa.
   c is invariant to case ('a' is the same as 'A').
   If c is not a letter (a-Z,A-Z), nothing is changed.
*/

#include "common.h"

void strtogglealpha(char* s, const char c);

int main(void)
{

   char str[1000];

   strcpy(str, "abc"); strtogglealpha(str, 'a');
   assert(strcmp(str,"Abc")==0);

   strcpy(str, "abc"); strtogglealpha(str, 'A');
   assert(strcmp(str,"Abc")==0);

   strcpy(str, "Here be dragons"); strtogglealpha(str, 'e');
   assert(strcmp(str, "HErE bE dragons")==0);

   strcpy(str, "Oops"); strtogglealpha(str, 'O');
   assert(strcmp(str, "oOps")==0);

   strcpy(str, "Hello World!"); strtogglealpha(str, '!');
   assert(strcmp(str,"Hello World!")==0);

   strcpy(str, ""); strtogglealpha("", 'F');
   assert(strcmp(str,"")==0);

   return 0;
}

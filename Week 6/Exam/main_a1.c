/*
   Removes every occurrence of the character c
   from the string s1, returning the new string s2.
*/

#include "common.h"

void strrem(const char* s1, const char c, char* s2);

int main(void)
{

   char str[1000];

   strrem("abc", 'b', str); assert(strcmp(str,"ac")==0);
   strrem("ABC", 'a', str); assert(strcmp(str,"ABC")==0);
   strrem("Hello World!", '!', str); assert(strcmp(str,"Hello World")==0);
   strrem("", ' ', str); assert(strcmp(str,"")==0);

   return 0;
}

#include <stdio.h>
#include <stdlib.h>
#define STRLN 100

int main(void)
{
   char i;
   char str[STRLN];

   fgets(str, STRLN, stdin);
   while (sscanf(str, "%s", &i) != 1) {
      printf("Cannot scan string!\n");
      fgets(str, STRLN, stdin);
   }
   printf("The string was %s", str);

   return 0;
}

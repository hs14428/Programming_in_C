#include <stdio.h>

int main(void)
{
   int i, step = 0, hash = 0;
   int prime = 31;
   int size = 11;
   int len = strlen("selodnaraf");
   char str[100] = "selodnaraf";

   for (i = 0; i < len; i++) {
      hash = ((prime*hash) + str[i]) % size;
   }
   printf("hash: %d\n", hash);

   for (i = 0; i < len; i++) {
      step = ((17*step) + str[i]) % size;
   }
   printf("step: %d\n", step);

   return 0;
}

#include <stdio.h>
#include <ctype.h>

#define TWENTYNOTE 20

int twentyCheck(int i);

typedef int money;

int main(void)
{
   char c;
   money m;

   printf("How much money would you like?\n");
   do {
      scanf("%d", &m);
      twentyCheck(m);
   } while (m % TWENTYNOTE !=0);
}

int twentyCheck(int i)
{
   money cnt1 = i, cnt2 = i;

   if(i % TWENTYNOTE != 0){
      while (cnt1 % TWENTYNOTE != 0) {
         cnt1++;
      }
      while (cnt2 % TWENTYNOTE != 0) {
         cnt2--;
      }
      return printf("I can give you %d or %d, try again\n", cnt2, cnt1);
   }
   return printf("Here's your money, %d", i);
}

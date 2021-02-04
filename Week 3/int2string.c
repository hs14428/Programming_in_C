#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>

char int2string(int i, char s[]);
int countInt(int num);
void test();

int main (void)
{

   int i;
   char s[256];

   test();

   scanf("%d", &i);

   int2string(i, s);

   printf("%s\n", s);

   return 1;
}

char int2string(int i, char s[])
{
   int j = 0, divisor, remainder;
   char strnum;

   divisor = countInt(i);
   remainder = i;

   do {
      strnum = remainder / divisor;
      s[j] = strnum + '0';
      remainder = remainder % divisor;
      divisor = countInt(remainder);
      j++;
   } while (remainder != 0);
   s[j] = '\0';

   return *s;
}

int countInt(int num)
{
   int cnt = 0, divisor;

   do {
      cnt++;
      num /= 10;
   } while (num != 0);

   if (cnt != 1) {
      divisor = pow(10, cnt - 1) + 0.5;
   }
   else {
      divisor = cnt;
   }
   return divisor;
}


void test()
{
   assert(countInt(2)   == 1);
   assert(countInt(24)  == 10);
   assert(countInt(241) == 100);

   int str1 = 197;
   char array1[256];
   int2string(str1, array1);
   char array2[256] = {'1','9','7','\0'};

   assert(strcmp(array1, array2) == 0);
}

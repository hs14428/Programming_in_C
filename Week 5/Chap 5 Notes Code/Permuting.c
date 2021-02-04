#include <stdio.h>
#include <string.h>

#define MAXLEN 5

void swap(char* x, char* y);

int main()
{
   char str[MAXLEN] = "ABC";
   int n = strlen(str);

   permute(str, 0, n-1);
   return 0;
}

void permute(char str[MAXLEN], int lf, int rt)
{
   int i;
   int n = strlen(str);
   static int cnt = 0;
   cnt++;
   printf("%d:\n", cnt);

   if (lf == rt) {
      printf("lf == rt: %s\n", str);
   }
   else {
      for (i = lf; i <= rt; i++) {
         printf("swap(&str[lf=%d], &str[i=%d]) = %c <-> %c\n", lf, i, str[lf], str[i]);
         swap(&str[lf], &str[i]);
         printf("permute(str=%s, lf+1=%d, rt(%d))\n", str, lf+1, rt);
         permute(str, lf+1, rt);
         printf("swap(&str[lf=%d], &str[i=%d]) = %c <-> %c\n", lf, i, str[lf], str[i]);
         swap(&str[lf], &str[i]);
      }
   }
}

void swap(char* x, char* y)
{
   char tmp;

   tmp = *x;
   *x = *y;
   *y = tmp;
}

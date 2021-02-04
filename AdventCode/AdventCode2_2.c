#include <stdio.h>
#include <string.h>

int main(void)
{
   FILE *file;
   int i, cnt1, cnt2, cnt3;
   int len = 0;
   int min = 0, max = 0;
   char pw[1000] = {0};
   char str[100] = {0};
   char let;

   file = fopen("password.txt", "r");
   cnt2 = 0;
   cnt3 = 0;
   while (cnt3 != 1000) {
      cnt1 = 0;
      fgets(pw, 1000, file);
      printf("%s", pw);
      sscanf(pw, "%d-%d %c: %s", &min, &max, &let, str);
      len = strlen(str);
      if ((str[min-1] == let) && (str[max-1] == let)) {

      }
      else if ((str[min-1] == let) || (str[max-1] == let)) {
         cnt2++;
      }
      cnt3++;
   }
   printf("Total valid %d\n", cnt2);
}

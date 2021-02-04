#include <assert.h>
#include <stdio.h>

/* Lowercase a = 97 and z = 122 in the ASCII table */
#define LOWERCASEA 97
#define LOWERCASEZ 122
/* To make a = 1, subtract number difference from a; 97 - 96 = 1 */
#define NUMBERDIFFERENCE 96
#define NOTAVOWEL 98
#define ISAVOWEL 97
#define NOTAPRIME 6
#define ISAPRIME 3

enum bool {false, true};
typedef enum bool bool;

bool vowel_check(char c);
bool namecriteria_check(char i, char j, char k);
bool primecheck(int n);
void test(void);

int main(void)
{
   test();

   int cnt = 1;
   char i, j, k;
   int p;

   for (i = LOWERCASEA; i <= LOWERCASEZ; i++) {
      for (j = LOWERCASEA; j <= LOWERCASEZ; j++) {
         for (k = LOWERCASEA; k <= LOWERCASEZ; k++) {
            if (namecriteria_check(i, j, k) == true) {
               if (i == k) {
                  p = (i - NUMBERDIFFERENCE) + (j - NUMBERDIFFERENCE) + (k - NUMBERDIFFERENCE);
                  if (primecheck(p) == true) {
                     printf("%d\t%c%c%c\n", cnt, i, j, k);
                     cnt++;
                  }
               }
            }
         }
      }
   }
   return 0;
}

bool vowel_check(char c)
{
   if (c == 'a' || c == 'e' || c == 'o' ||  c == 'i' || c == 'u') {
      return true;
   }
   else {
      return false;
   }
}

bool namecriteria_check(char i, char j, char k)
{
   if (((vowel_check(i) == false) && (vowel_check(j) == true) && (vowel_check(k) == false)) ||
      ((vowel_check(i) == true) && (vowel_check(j) == false) && (vowel_check(k) == true))) {
      return true;
   }
   else {
      return false;
   }
}

bool primecheck(int p)
{
   int x;

   for (x = 2; x <= p/2; x++) {
      if (p % x == 0) {
         return false;
      }
   }
   return true;
}

void test(void)
{
  assert(vowel_check(NOTAVOWEL) == false);
  assert(vowel_check(ISAVOWEL ) == true );
  assert(primecheck(NOTAPRIME ) == false);
  assert(primecheck(ISAPRIME  ) == true );
}

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define ALPHANUM 26

bool is_pangram(const char *str_in);

int main(void)
{
   assert(is_pangram("The quick, brown fox jumps over the lazy dog!"));
   assert(!is_pangram("The quick, brown fox jumped over the lazy dog!"));
   return 0;
}

bool is_pangram(const char *str_in) {

  int i, j, n;
  int alpha[ALPHANUM] = {0};

  n = strlen(str_in);
  for (i = 0; i < n; i++) {
    for (j = 'a'; j <= 'z'; j++) {
      if (tolower(str_in[i]) == j) {
        alpha[j-'a']++;
      }
    }
  }
  for (i = 0; i < ALPHANUM; i++) {
    if (alpha[i] < 1) {
      return false;
    }
  }
  return true;
}

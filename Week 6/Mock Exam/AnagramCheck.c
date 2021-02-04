#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define ALPHANUM 26

int anagram(char s1[], char s2[]);
int derange(char s1[], char s2[]);

int main(void)
{
   assert(anagram("elvis", "lives") == 1);
   assert(anagram("dreads", "sadder") == 1);
   assert(anagram("replays", "parsley") == 1);
   assert(anagram("listen", "silent") == 1);
   assert(anagram("orchestra", "carthorse") == 1);
   /* Two identical words are not anagrams*/
   assert(anagram("elvis", "elvis") == 0);
   assert(anagram("neill", "neil") == 0);
   assert(anagram("neil", "neill") == 0);
   assert(anagram("horse", "short") == 0);

   assert(derange("elvis", "lives") == 0);
   assert(derange("dreads", "sadder") == 1);
   assert(derange("replays", "parsley") == 1);
   assert(derange("listen", "silent") == 0);
   assert(derange("orchestra", "carthorse") == 1);

   return 0;
}

int anagram(char s1[], char s2[])
{
   int i, j, len, cnt;
   int hist1[ALPHANUM] = {0};
   int hist2[ALPHANUM] = {0};

   if (strlen(s1) > strlen(s2)) {
      len = strlen(s1);
   }
   else {
      len = strlen(s2);
   }

   for (i = 0; i < len; i++) {
      for (j = 'a'; j <= 'z'; j++) {
         if (s1[i] == j) {
            hist1[j-'a']++;
         }
         if (s2[i] == j) {
            hist2[j-'a']++;
         }
      }
   }

   cnt = 0;
   for (i = 0; i < ALPHANUM; i++) {
      if (hist1[i] == hist2[i]) {
         cnt++;
      }
   }
   if (cnt == ALPHANUM) {
      if (strcmp(s1, s2) == 0) {
         return 0;
      }
      else {
         return 1;
      }
   }
   else {
      return 0;
   }
}

int derange(char s1[], char s2[])
{
   int i, j, len, cnt;
   int hist1[ALPHANUM] = {0};
   int hist2[ALPHANUM] = {0};

   if (strlen(s1) > strlen(s2)) {
      len = strlen(s1);
   }
   else {
      len = strlen(s2);
   }

   for (i = 0; i < len; i++) {
      for (j = 'a'; j <= 'z'; j++) {
         if (s1[i] == j) {
            hist1[j-'a']++;
         }
         if (s2[i] == j) {
            hist2[j-'a']++;
         }
      }
   }
   cnt = 0;
   for (i = 0; i < ALPHANUM; i++) {
      if (hist1[i] == hist2[i]) {
         cnt++;
      }
   }
   if (cnt == ALPHANUM) {
      for (i = 0; i < len; i++) {
         if (s1[i] == s2[i]) {
            return 0;
         }
      }
      return 1;
   }
   else {
      return 0;
   }
}

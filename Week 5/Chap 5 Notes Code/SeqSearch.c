#include <stdio.h>
#include <string.h>

#define NOTFOUND -1
#define NUMPEOPLE 6

struct person {
   char *name;
   int age;
};

typedef struct person Person;

int findAge(char *name, Person *p, int n);

int main(void)
{
/* sorted list */
   Person ppl[NUMPEOPLE] = {{"Ackerby", 21}, {"Bloggs", 25},
                            {"Chumley", 26}, {"Dalton", 25},
                            {"Eggson",  22}, {"Fulton", 41}};

   printf("%d\n", findAge("Eggson", ppl, NUMPEOPLE));
   printf("%d\n", findAge("Campbell", ppl, NUMPEOPLE));

   return 0;
}
/* Sorted findAge - will go quicker as stops once it finds name
only if sorted list though*/
int findAge(char *name, Person *p, int n)
{
   int m, j;

   for (j = 0; j < n; j++) {
      m = strcmp(name, p[j].name);
      if (m == 0) {
         return p[j].age;
      }
      if (m < 0) {
         return NOTFOUND;
      }
   }
   return NOTFOUND;
}

/*not sorted findAge */
/*
int findAge(char *name, Person *p, int n)
{
   int j;

   for (j = 0; j < n; j++) {
      if(strcmp(name, p[j].name) == 0) {
         return p[j].age;
      }
   }
   return NOTFOUND;
}
*/

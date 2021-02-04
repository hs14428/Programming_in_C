/* Note:
    Allocate memory yourself!
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

char *dna_strand(const char *dna);

int main(void)
{
   dna_strand("ATTGC");
   dna_strand("GTAT");
   return 0;
}

char *dna_strand(const char *dna)
{
   int i;
   int n = strlen(dna);
   char *comp = (char*)malloc((n+1)*sizeof(char*));
   printf("n: %d\n", n);

   for (i = 0; i < n; i++) {
      if (dna[i] == 'A') {
         comp[i] = 'T';
      }
      if (dna[i] == 'T') {
         comp[i] = 'A';
      }
      if (dna[i] == 'C') {
         comp[i] = 'G';
      }
      if (dna[i] == 'G') {
         comp[i] = 'C';
      }
      printf("%c", comp[i]);
   }
   comp[i] = '\0';
   printf("\n");
   free(comp);
   return comp;
}

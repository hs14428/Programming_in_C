#include "extension_fn.h"

int main(void)
{
   char word[BUFFER] = "\0";

   /* test(); */
   welcome();
   if (scanf("%s", word) != 1) {
      SCANERROR("Scanf error");
   }
   if (yesFile(word)) {
      interactiveFile();
   }
   else if (noFile(word)) {
      interactiveType();
   }
   else {
      fprintf(stdout, "\nUnsure of file preference.\n\n");
      exit(EXIT_FAILURE);
   }

   return 0;
}

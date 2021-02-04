#include "parser_fn.h"

int main(int argc, char* argv[MAXFPLEN])
{
   Program prog;

   if (argc != 2) {
      fprintf(stderr, "Incorrect argument count\n");
      exit(EXIT_FAILURE);
   }
   else {
      test();
      fprintf(stderr, "Testing succesfully completed.\n\n");
      prog = ReadFile(argv[1]);
      if (Prog(&prog)) {
         fprintf(stderr, "Turtle file parsed OK.\n");
      }
      else {
         fprintf(stderr, "Parsing failed.\n");
      }
      return 0;
   }
}

#include "interp_fn.h"

int main(int argc, char* argv[MAXFPLEN])
{
   Program prog;

   if (argc != 2) {
      fprintf(stderr, "Incorrect argument count\n");
      exit(EXIT_FAILURE);
   }
   /* test(); */
   prog = ReadFile(argv[1]);
   SDLInit(&prog);
   if (!interpProg(&prog)) {
      fprintf(stderr, "Issue around word pos: %d = %s\n",\
      prog.ci, prog.instr[prog.ci]);
      exit(EXIT_FAILURE);
   }
   else {
      Loop_SDL_Events(&prog);
      return 0;
   }
}

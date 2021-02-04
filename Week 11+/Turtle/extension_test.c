#include "extension_fn.h"

void test()
{
   int i;
   FILE *efp;
   char *filename;
   Program ep;

   ep.ci = 0;
   ep.currVal = 0;
   for (i = 0; i < MAXINSTRS; i++) {
      ep.instr[i][0] = '\0';
   }

   filename = "testext.ttl";
   if (!(efp = nfopen(filename, "r"))) {
      fprintf(stderr, "Cannot open %s\n", filename);
      exit(2);
   }
   i = 0;
   while (fscanf(efp, "%s", ep.instr[i++]) == 1 && i < MAXINSTRS) {
      assert (i < MAXINSTRS);
   }
   fclose(efp);

/* Test lines 1 - 4 of testext.ttl */
   ep.ci = 0;
   assert(Jump(&ep));
   assert(ep.jump == 1);
   ep.ci = 2;
   assert(Jump(&ep));
   assert(!(ep.jump == 1));
   assert(ep.jump == 5);
   ep.ci = 4;
   ep.var['A' - 'A'] = 10;
   assert(Jump(&ep));
   assert(ep.jump == 10);
   ep.ci = 6;
   assert(!Jump(&ep));
   assert(!(ep.jump == 5));

/* Line 5 of testinerp.ttl file. DO A FROM 1 TO 10 { FD 30 } */
   ep.ci = 8;
   interpDo(&ep);
   assert(dblEquiv(ep.var['A'-'A'], 10));
/* Line 6 of testinerp.ttl file. DO A FROM 1 TO 10 JUMP 4 { FD 30 } */
   ep.ci = 18;
   interpDo(&ep);
   assert(!dblEquiv(ep.var['A'-'A'], 10));
   assert(dblEquiv(ep.var['A'-'A'], 9));
}

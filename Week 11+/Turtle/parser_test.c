#include "parser_fn.h"

void test()
{
   int i;
   FILE *tfp;
   char *filename;
   Program tp;

   tp.ci = 0;
   for (i = 0; i < MAXINSTRS; i++) {
      tp.instr[i][0] = '\0'; /* Could use calloc here */
   }

   filename = "testinstr.ttl";
   if (!(tfp = nfopen(filename, "r"))) {
      fprintf(stderr, "Cannot open %s\n", filename);
      exit(2);
   }
   i = 0;
   while (fscanf(tfp, "%s", tp.instr[i++]) == 1 && i < MAXINSTRS) {
      assert (i < MAXINSTRS);
   }
   fclose(tfp);

/* Test isValNumStart fn with lines 1-7 of testinstr.ttl file*/
   tp.ci = 0; /* Line 1 of test file: +300 */
   assert(isValNumStart(&tp));
   tp.ci = 1; /* Line 2 of test file: -300 */
   assert(isValNumStart(&tp));
   tp.ci = 2; /* Line 3 of test file: 300 */
   assert(isValNumStart(&tp));
   tp.ci = 5; /* Line 6 of test file: x30 */
   assert(!isValNumStart(&tp));
   tp.ci = 6; /* Line 7 of test file: .42 */
   assert(!isValNumStart(&tp));

/* Test initial cutdown grammar <VARNUM fn with lines 1-9 of testinstr.ttl file */
   tp.ci = 0; /* Line 1 of test file: +300 */
   assert(VarNum(&tp));
   tp.ci = 3; /* Line 4 of test file: 30.7 */
   assert(VarNum(&tp));
   tp.ci = 4; /* Line 5 of test file: 30.9.1 */
   assert(!VarNum(&tp));
   tp.ci = 5; /* Line 6 of test file: x30 */
   assert(!VarNum(&tp));
   tp.ci = 7; /* Line 8 of test file: hello300 */
   assert(!VarNum(&tp));
   tp.ci = 8; /* Line 9 of test file: --30 */
   assert(!VarNum(&tp));

/* Test <RT> grammar fn with lines 10-14 of testinstr.ttl file */
   tp.ci = 9; /* Line 10 of test file: RT 300 */
   assert(Rt(&tp));
   tp.ci = 11; /* Line 11 of test file: rT 30.7 */
   assert(!Rt(&tp));
   tp.ci = 13; /* Line 12 of test file: .RT 40 */
   assert(!Rt(&tp));
   tp.ci = 15; /* Line 13 of test file: RT 19.67 */
   assert(Rt(&tp));
   tp.ci = 17; /* Line 14 of test file: R_T 40 */
   assert(!Rt(&tp));

/* Test <LT> grammar fn with lines 15-18 of testinstr.ttl file */
   tp.ci = 19; /* Line 15 of test file: LT 21.9 */
   assert(Lt(&tp));
   tp.ci = 21; /* Line 16 of test file: Lt 300 */
   assert(!Lt(&tp));
   tp.ci = 23; /* Line 17 of test file: LT +42 */
   assert(Lt(&tp));
   tp.ci = 25; /* Line 18 of test file: RT -30 */
   assert(!Lt(&tp));

/* Test <FD> grammar fn with lines 19-22 of testinstr.ttl file */
   tp.ci = 27; /* Line 19 of test file: FD +32.3 */
   assert(Fd(&tp));
   tp.ci = 29; /* Line 20 of test file: F.D 19 */
   assert(!Fd(&tp));
   tp.ci = 31; /* Line 21 of test file: (space) FD -30.1 */
   assert(Fd(&tp));
   tp.ci = 33; /* Line 22 of test file: fd 2 */
   assert(!Fd(&tp));

/* Test initial cutdown grammar <INSTRUCTION> fn with lines 23-28 of testinstr.ttl file */
   tp.ci = 35; /* Line 23 of test file: RT 30 */
   assert(Instruction(&tp));
   tp.ci = 37; /* Line 24 of test file: FD_1.9 */
   assert(!Instruction(&tp));
   tp.ci = 38; /* Line 25 of test file: LT 11.9 */
   assert(Instruction(&tp));
   tp.ci = 40; /* Line 26 of test file: LT RT 40 */
   assert(!Instruction(&tp));
   tp.ci = 43; /* Line 27 of test file: FD RT LT 30 */
   assert(!Instruction(&tp));
   tp.ci = 47; /* Line 28 of test file: FD -15 */
   assert(Instruction(&tp));

/* Test <INSTRCTLST> grammar fn with lines 29-35 of testinstr.ttl file */
/* I also tested that the instruction will fail if missing an ending }, */
/* but because of the way I am testing, line by line, if i dont add the } */
/* at the end, the function will look to the next line for the next string */
   tp.ci = 49; /* Line 29 of test file: } */
   assert(InstrctLst(&tp));
   tp.ci = 50; /* Line 30 of test file: FD 20.1 } */
   assert(InstrctLst(&tp));
   tp.ci = 53; /* Line 31 of test file: FD 20.1 . */
   assert(!InstrctLst(&tp));
   tp.ci = 56; /* Line 32 of test file: FD 20.1} */
   assert(!InstrctLst(&tp));
   tp.ci = 58; /* Line 33 of test file: RT -81 } */
   assert(InstrctLst(&tp));
   tp.ci = 61; /* Line 34 of test file: L T 12 } */
   assert(!InstrctLst(&tp));
   tp.ci = 65; /* Line 35 of test file: LT RT 12 } */
   assert(!InstrctLst(&tp));

/* Test <MAIN> grammar fn with lines 36-39 of testinstr.ttl file. */
   tp.ci = 69; /* Line 36 of test file: { RT 30 } */
   assert(Prog(&tp));
   tp.ci = 73; /* Line 37 of test file: { FD 29.0 } */
   assert(Prog(&tp));
   tp.ci = 77; /* Line 38 of test file: { RT 30} */
   assert(!Prog(&tp));
   tp.ci = 80; /* Line 39 of test file: { LT RT 30 } */
   assert(!Prog(&tp));

/* All testing from here is post completion of cut-down grammar */
/* From this point on, I began to add the rest of the grammar   */

/* Test <OP> grammar fn with lines 40-45 of testinstr.ttl file */
   tp.ci = 85; /* Line 40 of test file: + */
   assert(Op(&tp));
   tp.ci = 86; /* Line 41 of test file: - */
   assert(Op(&tp));
   tp.ci = 87; /* Line 42 of test file: * */
   assert(Op(&tp));
   tp.ci = 88; /* Line 43 of test file: / */
   assert(Op(&tp));
   tp.ci = 89; /* Line 44 of test file: ++ */
   assert(!Op(&tp));
   tp.ci = 90; /* Line 45 of test file: *- */
   assert(!Op(&tp));

/* Test <VAR> grammar fn with lines 46-51 of testinstr.ttl file */
   tp.ci = 91; /* Line 46 of test file: A */
   assert(Var(&tp));
   tp.ci = 92; /* Line 47 of test file: Z */
   assert(Var(&tp));
   tp.ci = 93; /* Line 48 of test file: M */
   assert(Var(&tp));
   tp.ci = 94; /* Line 48 of test file: @ */
   assert(!Var(&tp));
   tp.ci = 95; /* Line 50 of test file: [ */
   assert(!Var(&tp));
   tp.ci = 96; /* Line 51 of test file: BB */
   assert(!Var(&tp));

/* Re-test <VARNUM> fn with full grammar with lines 52-56 of testinstr.ttl file */
   tp.ci = 97; /* Line 52 of test file: A */
   assert(VarNum(&tp));
   tp.ci = 98; /* Line 53 of test file: +8.1 */
   assert(VarNum(&tp));
   tp.ci = 99; /* Line 54 of test file: +8,1 */
   assert(!VarNum(&tp));
   tp.ci = 100; /* Line 55 of test file: # */
   assert(!VarNum(&tp));
   tp.ci = 101; /* Line 56 of test file: A8.1 */
   assert(!VarNum(&tp));

/* Test <POLISH> grammar fn with lines 57-62 of testinstr.ttl file   */
/* As with the <INSTRCTLST> fn, I also tested that the instr will    */
/* fail if missing an ending ;, but can't show here for same reasons */
   tp.ci = 102; /* Line 57 of test file: + + ; */
   assert(Polish(&tp));
   tp.ci = 105; /* Line 58 of test file: + +100 ; */
   assert(Polish(&tp));
   tp.ci = 108; /* Line 59 of test file: 100 250.5 + * ; */
   assert(Polish(&tp));
   tp.ci = 113; /* Line 60 of test file: 14 *+ 350 ; */
   assert(!Polish(&tp));
   tp.ci = 117; /* Line 61 of test file: -29 41; */
   assert(!Polish(&tp));
   tp.ci = 119; /* Line 62 of test file: 32 45 + +; */
   assert(!Polish(&tp));

/* Test <SET> grammar fn with lines 63-67 of testinstr.ttl file */
   tp.ci = 123; /* Line 63 of test file: SET A := +50 32 + ; */
   assert(Set(&tp));
   tp.ci = 130; /* Line 64 of test file: SET H := * 30 12 + / ; */
   assert(Set(&tp));
   tp.ci = 139; /* Line 65 of test file: SET B:= 4 12 + + ; */
   assert(!Set(&tp));
   tp.ci = 146; /* Line 66 of test file: SeT A := -32 + ; */
   assert(!Set(&tp));
   tp.ci = 152; /* Line 67 of test file: SET A = +50 32 - ; */
   assert(!Set(&tp));

/* Test <DO> grammar fn with lines 68-73 of testinstr.ttl file            */
/* As with the <INSTRCTLST> and <POLISH> fn, I also tested that the instr */
/* will fail if missing an ending }, but can't show here for same reasons */
   tp.ci = 159; /* Line 68 of test file: DO A FROM 1 TO 10 { } */
   assert(Do(&tp));
   tp.ci = 167; /* Line 69 of test file: DO B FROM 1 TO 20 { FD 30 } */
   assert(Do(&tp));
   tp.ci = 177; /* Line 70 of test file: DO C FROM 9.1 TO -12 { LT +21 } */
   assert(Do(&tp));
   tp.ci = 187; /* Line 71 of test file: do d from 1 to 3 { RT 21 } */
   assert(!Do(&tp));
   tp.ci = 197; /* Line 72 of test file: DO E FROM 1 TO 5 {LT 31} */
   assert(!Do(&tp));
   tp.ci = 205; /* Line 73 of test file: DO F FROM 9 TO 10 FD 10 } */
   assert(!Do(&tp));

/* Additional testing on <INSTRUCTION> grammar fn after adding */
/* <D0> and <SET>. Test with lines 63 and 69 of testinstr.ttl file */
   tp.ci = 123; /* Line 63 of test file: SET A := +50 32 + ; */
   assert(Instruction(&tp));
   tp.ci = 167; /* Line 69 of test file: DO B FROM 1 TO 20 { FD 30 } */
   assert(Instruction(&tp));
/* Test a nested <DO> after adding <DO> to <INSTRUCTION> fn */
   tp.ci = 214; /* Line 74 of test file: DO B FROM 1 TO 20 { DO A FROM 1 TO 10 { LT 30 } } */
   assert(Do(&tp));

/* Bring together all the functions and re-test the <MAIN> Prog fn */
/* Testing with the GitHub .ttl files pasted into my test .ttl file */
   tp.ci = 233; /* simp.ttl file on Line 76 of testinstr.ttl file */
   assert(Prog(&tp));
   tp.ci = 248; /* spiral1_c.ttl file on Line 84 of testinstr.ttl file */
   assert(Prog(&tp));
   tp.ci = 270; /* spiral2_c.ttl file on Line 93 of testinstr.ttl file */
   assert(Prog(&tp));
   tp.ci = 304; /* simp_c.ttl file on Line 106 of testinstr.ttl file */
   assert(Prog(&tp));
}

#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "general.h"

#define MAXINSTRS 1000
#define MAXINSTRLEN 15
#define MAXFPLEN 30
#define ADD "+"
#define SUB "-"
#define MUL "*"
#define DIV "/"
#define strsame(A, B) (strcmp(A, B) ==0)
#define ERROR(PHRASE) {fprintf(stderr, "Fatal Error. %s Occured in %s, line %d\n", PHRASE, __FILE__, __LINE__); }

typedef enum bool {false, true} bool;

typedef struct prog {
   char instr[MAXINSTRS][MAXINSTRLEN];
   int  ci; /* Current instruction */
} Program;

void test();
Program ReadFile(char *filename);
bool Prog(Program *p);
bool InstrctLst(Program *p);
bool Instruction(Program *p);
bool Fd(Program *p);
bool Lt(Program *p);
bool Rt(Program *p);
bool Do(Program *p);
bool Var(Program *p);
bool VarNum(Program *p);
bool isValNumStart(Program *p);
bool Set(Program *p);
bool Polish(Program *p);
bool Op(Program *p);

int main(int argc, char* argv[MAXFPLEN])
{
   Program prog;

   if (argc != 2) {
      fprintf(stderr, "Incorrect argument count\n");
      exit(2);
   }
   test();
   prog = ReadFile(argv[1]);

   return 0;
}

Program ReadFile(char *filename)
{
   int i;
   FILE *fp;
   Program p;

   p.ci = 0;
   for (i = 0; i < MAXINSTRS; i++) {
      p.instr[i][0] = '\0';
   }
   if (!(fp = nfopen(filename, "r"))) {
      fprintf(stderr, "Cannot open %s\n", filename);
      exit(2);
   }
   i = 0;
   while (fscanf(fp, "%s", p.instr[i++]) == 1 && i < MAXINSTRS) {
      assert (i < MAXINSTRS);
   }
   fclose(fp);
   return p;
}

bool Prog(Program *p)
{
   if (!strsame(p->instr[p->ci], "{")) {
      ERROR("No opening { in input?\n");
      return false;
   }
   p->ci = p->ci + 1;
   if (!InstrctLst(p)) {
      return false;
   }
   return true;
}

bool InstrctLst(Program *p)
{
   if (strsame(p->instr[p->ci], "}")) {
      return true;
   }
   if (!Instruction(p)) {
      return false;
   }
   p->ci = p->ci + 1;
   return(InstrctLst(p));
}

bool Instruction(Program *p)
{
   int si = p->ci;
   if (Fd(p)) {
      return true;
   }
   p->ci = si;
   if (Lt(p)) {
      return true;
   }
   p->ci = si;
   if (Rt(p)) {
      return true;
   }
   p->ci = si;
   if (Do(p)) {
      return true;
   }
   p->ci = si;
   if (Set(p)) {
      return true;
   }
   p->ci = si;
/* ERROR("Instruction expected FD/LT/RT/DO/SET."); */
   return false;
}

bool Fd(Program *p)
{
   if (!strsame(p->instr[p->ci++], "FD")) {
      return false;
   }
   /* p->ci = p->ci + 1; */
   if (!VarNum(p)) {
      return false;
   }
   return true;
}

bool Lt(Program *p)
{
   if (!strsame(p->instr[p->ci++], "LT")) {
      return false;
   }
   /* p->ci = p->ci + 1; */
   if (!VarNum(p)) {
      return false;
   }
   return true;
}

bool Rt(Program *p)
{
   if (!strsame(p->instr[p->ci++], "RT")) {
      return false;
   }
   /* p->ci = p->ci + 1; */
   if (!VarNum(p)) {
      return false;
   }
   return true;
}

bool Do(Program *p)
{
   if (!strsame(p->instr[p->ci++], "DO")) {
      return false;
   }
   /* p->ci = p->ci + 1; */
   if (!Var(p)) {
      return false;
   }
   p->ci = p->ci + 1;
   if (!strsame(p->instr[p->ci++], "FROM")) {
      return false;
   }
   /* p->ci = p->ci + 1; */
   if (!VarNum(p)) {
      return false;
   }
   p->ci = p->ci + 1;
   if (!strsame(p->instr[p->ci++], "TO")) {
      return false;
   }
   /* p->ci = p->ci + 1; */
   if (!VarNum(p)) {
      return false;
   }
   p->ci = p->ci + 1;
   if (!strsame(p->instr[p->ci++], "{")) {
      return false;
   }
   /* p->ci = p->ci + 1; */
   if (!InstrctLst(p)) {
      return false;
   }
   return true;
}

bool Var(Program *p)
{
   int len = strlen(p->instr[p->ci]);

   if (len == 1) {
      if (p->instr[p->ci][0] >= 'A' && p->instr[p->ci][0] <= 'Z') {
         return true;
      }
      return false;
   }
   return false;
}

bool VarNum(Program *p)
{
   int i;
   int dec;
   int len = strlen(p->instr[p->ci]);

   dec = 0;
   if (isValNumStart(p)) {
      for (i = 1; i < len; i++) {
         if (!isdigit(p->instr[p->ci][i])) {
            if (p->instr[p->ci][i] == '.' && dec == 0) {
               dec++;
            }
            else {
               return false;
            }
         }
      }
      return true;
   }
   else if (Var(p)) {
      return true;
   }
   return false;
}

/* Function to check whether the start of number string is valid */
/* Will allow number strings to be +ve or -ve */
bool isValNumStart(Program *p)
{
   if (isdigit(p->instr[p->ci][0])
      || p->instr[p->ci][0] == '-'
      || p->instr[p->ci][0] == '+') {
      return true;
   }
   else {
      return false;
   }
}

bool Set(Program *p)
{
   if (!strsame(p->instr[p->ci++], "SET")) {
      return false;
   }
   /* p->ci = p->ci + 1; */
   if (!Var(p)) {
      return false;
   }
   p->ci = p->ci + 1;
   if (!strsame(p->instr[p->ci++], ":=")) {
      return false;
   }
   /* p->ci = p->ci + 1; */
   if (!Polish(p)) {
      return false;
   }
   return true;
}

bool Polish(Program *p)
{
   if (strsame(p->instr[p->ci], ";")) {
      return true;
   }
   if (Op(p)) {
      p->ci = p->ci + 1;
      if (Polish(p)) {
         return true;
      }
   }
   if (VarNum(p)) {
      p->ci = p->ci + 1;
      if (Polish(p)) {
         return true;
      }
   }
   return false;
}

bool Op(Program *p)
{
   if (strsame(p->instr[p->ci], ADD)) {
      return true;
   }
   if (strsame(p->instr[p->ci], SUB)) {
      return true;
   }
   if (strsame(p->instr[p->ci], MUL)) {
      return true;
   }
   if (strsame(p->instr[p->ci], DIV)) {
      return true;
   }
   return false;
}

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

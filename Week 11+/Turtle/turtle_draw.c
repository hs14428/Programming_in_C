#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include "General/general.h"
#include "Stack/stack.h"
#include "neillsdl2.h"

#define MAXINSTRS 1000
#define MAXINSTRLEN 15
#define MAXFPLEN 30
#define ALPHANUM 26
#define EPSILON 0.00001
#define M_PI 3.14159265358979323846
#define RADSCONV M_PI/180
#define FULLROT 360
#define DELAY 500
#define ADD "+"
#define SUB "-"
#define MUL "*"
#define DIV "/"
#define strsame(A,B) (strcmp(A, B) ==0)
#define ERROR(PHRASE) {fprintf(stderr, "Fatal Error. %s Occured in %s, line %d\n", PHRASE, __FILE__, __LINE__); }

typedef struct coordinates {
   double x;
   double y;
   double theta;
} Coords;

typedef struct prog {
   char   instr[MAXINSTRS][MAXINSTRLEN];
   int    ci; /* Current instruction */
   int    numWords;
   double currVal;
   char   currVar;
   double i;
   double var[ALPHANUM];
   double from[ALPHANUM];
   double to[ALPHANUM];
   Coords old_xyt;
   Coords xyt;
   SDL_Simplewin sw;
} Program;

void test();
void SDLInit(Program *p);
Program ReadFile(char *filename);
bool interpProg(Program *p);
bool Prog(Program *p);
bool interpInstrctLst(Program *p);
bool InstrctLst(Program *p);
bool interpInstruction(Program *p);
bool Instruction(Program *p);
bool moveFd(Program *p);
double getVarNum(Program *p);
bool Fd(Program *p);
bool rotate(Program *p);
bool turnMod(Program *p);
bool Lt(Program *p);
bool Rt(Program *p);
bool interpDo(Program *p);
bool Do(Program *p);
bool Var(Program *p);
bool VarNum(Program *p);
bool isValNumStart(Program *p);
bool interpSet(Program *p, stack *s);
bool Set(Program *p);
bool Polish(Program *p);
bool interpPolish(Program *p, stack *s);
bool stackCheck(stack *s);
bool getVarNumStack(Program *p, stack *s);
bool calcPolish(Program *p, stack *s);
bool Op(Program *p);

int main(int argc, char* argv[MAXFPLEN])
{
   Program prog;

   if (argc != 2) {
      fprintf(stderr, "Incorrect argument count\n");
      exit(2);
   }
   /* test(); */
   prog = ReadFile(argv[1]);
   Neill_SDL_Init(&prog.sw);
   Neill_SDL_SetDrawColour(&prog.sw, 255, 255, 255);
   Neill_SDL_UpdateScreen(&prog.sw);

   SDL_RenderDrawLine(prog.sw.renderer, 500, 400, 600, 430);
   Neill_SDL_UpdateScreen(&prog.sw);

   interpProg(&prog);

   while (!prog.sw.finished) {
      Neill_SDL_Events(&prog.sw);
   }
   SDL_Quit();
   atexit(SDL_Quit);

   return 0;
}

void SDLInit(Program *p)
{
   Neill_SDL_Init(&p->sw);
   Neill_SDL_SetDrawColour(&p->sw, 255, 255, 255);
   Neill_SDL_UpdateScreen(&p->sw);
}

Program ReadFile(char *filename)
{
   int i;
   FILE *fp;
   Program p;

/* try with callocing */
   p.ci = 0;
   p.xyt.x = WWIDTH/2;
   p.xyt.y = WHEIGHT/2;
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
   p.numWords = i-1;
   fclose(fp);
   return p;
}

bool interpProg(Program *p)
{
   if (!strsame(p->instr[p->ci], "{")) {
      ERROR("No opening { in input?\n");
      return false;
   }
/*    SDL_RenderDrawLine(p->sw.renderer, 400, 300, 400, 330);
   Neill_SDL_UpdateScreen(&p->sw); */
   while (!p->sw.finished) {
      Neill_SDL_Events(&p->sw);
   }
   p->ci = p->ci + 1;
   if (!interpInstrctLst(p)) {
      return false;
   }
   return true;
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

bool interpInstrctLst(Program *p)
{
   if (strsame(p->instr[p->ci], "}")) {
      return true;
   }
   if (interpInstruction(p)) {
      p->ci = p->ci + 1;
      if (interpInstrctLst(p)) {
         return true;
      }
   }
   return false;
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

bool interpInstruction(Program *p)
{
   int si = p->ci;
   stack *s;
   s = stack_init();
   if (moveFd(p)) {
      SDL_RenderDrawLine(p->sw.renderer, p->i+30, (p->i+40), p->i+80, (p->i+10));
      p->i = p->i+50;
      printf("Hello, i = %f\n", p->i);
      return true;
   }
   /* p->ci = si; */
   if (rotate(p)) {
      return true;
   }
   /* p->ci = si; */
   if (interpDo(p)) {
      return true;
   }
   /* p->ci = si; */
   if (interpSet(p, s)) {
      return true;
   }
   /* p->ci = si; */
   stack_free(s);
   return false;
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

bool moveFd(Program *p)
{
   double rads, val;
   if (!strsame(p->instr[p->ci], "FD")) {
      return false;
   }
   p->ci = p->ci + 1;
   if (VarNum(p)) {
      val = getVarNum(p);
      rads = p->xyt.theta * RADSCONV;
      p->old_xyt.x = p->xyt.x;
      p->old_xyt.y = p->xyt.y;
      p->xyt.x = -(val)*sin(rads) + p->xyt.x;
      p->xyt.y =  (val)*cos(rads) + p->xyt.y;
      printf("old_x: %.2f -> new_x: %.2f | old_y: %.2f -> new_y: %.2f\n", p->old_xyt.x, p->xyt.x, p->old_xyt.y, p->xyt.y);

/* try putting blocks here */
      SDL_RenderDrawLine(p->sw.renderer, p->old_xyt.x, p->old_xyt.y, p->xyt.x, p->xyt.y);
      SDL_Delay(DELAY);
      Neill_SDL_UpdateScreen(&p->sw);
      Neill_SDL_Events(&p->sw);
      return true;
   }
   return false;
}

double getVarNum(Program *p)
{
   int index;
   double val;
   char* ptr;
   if (Var(p)) {
      index = p->instr[p->ci][0];
      val = p->var[index - 'A'];
      return val;
   }
   else {
      val = strtod(p->instr[p->ci], &ptr);
      return val;
   }
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

bool rotate(Program *p)
{
   p->old_xyt.theta = p->xyt.theta;
   if (strsame(p->instr[p->ci], "LT")) {
      p->ci = p->ci + 1;
      if (VarNum(p)) {
         p->xyt.theta += getVarNum(p);
         if (turnMod(p)) {
            return true;
         }
      }
   }
   if (strsame(p->instr[p->ci], "RT")) {
      p->ci = p->ci + 1;
      if (VarNum(p)) {
         p->xyt.theta -= getVarNum(p);
         if (turnMod(p)) {
            return true;
         }
      }
   }
   return false;
}

bool turnMod(Program *p)
{
   if (p->xyt.theta < 0) {
      p->xyt.theta += FULLROT;
      return true;
   }
   else {
      p->xyt.theta = fmod(p->xyt.theta, FULLROT);
      return true;
   }
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

bool interpDo(Program *p)
{
   int wi;
   double from, to;
   double saveVar = p->currVar;
   if (!strsame(p->instr[p->ci], "DO")) {
      return false;
   }
   p->ci = p->ci + 1;
   if (Var(p)) {
      p->currVar = p->instr[p->ci++][0];
      if (strsame(p->instr[p->ci++], "FROM")) {
         if (VarNum(p)) {
            from = getVarNum(p);
            p->ci = p->ci + 1;
            if (strsame(p->instr[p->ci++], "TO")) {
               if (VarNum(p)) {
                  to = getVarNum(p);
                  p->ci = p->ci + 1;
                  if (strsame(p->instr[p->ci++], "{")) {
                     wi = p->ci;
                     p->var[p->currVar-'A'] = from;
                     while (p->var[p->currVar-'A'] <= to) {
                        p->ci = wi;
                        if (!interpInstrctLst(p)) {
                           return false;
                        }
                        p->var[p->currVar-'A']++;
                     }
                     p->var[p->currVar-'A']--;
                     p->currVar = saveVar;
                     return true;
                  }
               }
            }
         }
      }
   }
   return false;
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

bool interpSet(Program *p, stack *s)
{
   char var;
   if (!strsame(p->instr[p->ci++], "SET")) {
      return false;
   }
   /* p->ci = p->ci + 1; */
   if (Var(p)) {
      var = p->instr[p->ci++][0];
      /* p->ci = p->ci + 1; */
      if (strsame(p->instr[p->ci++], ":=")) {
         /* p->ci = p->ci + 1; */
         interpPolish(p, s);
         p->var[var-'A'] = p->currVal;
         return true;
      }
   }
   return false;
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

bool interpPolish(Program *p, stack *s)
{
   double result;

   if (strsame(p->instr[p->ci], ";")) {
      stack_pop(s, &result);
      stackCheck(s);
      p->currVal = result;
      return true;
   }
   else if (Op(p)) {
      calcPolish(p, s);
      p->ci = p->ci + 1;
      if (interpPolish(p, s)) {
         return true;
      }
   }
   else if (VarNum(p)) {
      getVarNumStack(p, s);
      p->ci = p->ci + 1;
      if (interpPolish(p, s)) {
         return true;
      }
   }
   return false;
}

bool stackCheck(stack *s)
{
   double d;
   if (stack_peek(s, &d) == true) {
      fprintf(stderr, "Stack still had items on it. Check instructions are correct.\n");
      return false;
   }
   stack_free(s);
   return true;
}

bool getVarNumStack(Program *p, stack *s)
{
   int index;
   double val;
   char* ptr;
   if (Var(p)) {
      index = p->instr[p->ci][0];
      val = p->var[index - 'A'];
      stack_push(s, val);
      return true;
   }
   else {
      val = strtod(p->instr[p->ci], &ptr);
      stack_push(s, val);
      return true;
   }
}

bool calcPolish(Program *p, stack* s)
{
   double d, p1, p2, result;
   if (stack_peek(s, &d)) {
      stack_pop(s, &p1);
      if (stack_peek(s, &d)) {
      stack_pop(s, &p2);
      }
      else {
         /* fprintf(stderr, "No items on the stack. Cannot operate.\n"); */
         return false;
      }
   }
   else {
      /* fprintf(stderr, "No items on the stack. Cannot operate.\n"); */
      return false;
   }

   if (strsame(p->instr[p->ci], ADD)) {
      result = p2 + p1;
   }
   else if (strsame(p->instr[p->ci], SUB)) {
      result = p2 - p1;
   }
   else if (strsame(p->instr[p->ci], MUL)) {
      result = p2 * p1;
   }
   else if (strsame(p->instr[p->ci], DIV)) {
      result = p2 / p1;
   }
   stack_push(s, result);
   return true;
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
   Program tp, tp2;
   stack* ts;
   double result;

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

/* Testing from here is interpreter specific */
/* Import new test .ttl file for interpreter testing */

   tp2.ci = 0;
   tp2.currVal = 0;
   for (i = 0; i < MAXINSTRS; i++) {
      tp2.instr[i][0] = '\0'; /* Could use calloc here */
   }

   filename = "testinterp.ttl";
   if (!(tfp = nfopen(filename, "r"))) {
      fprintf(stderr, "Cannot open %s\n", filename);
      exit(2);
   }
   i = 0;
   while (fscanf(tfp, "%s", tp2.instr[i++]) == 1 && i < MAXINSTRS) {
      assert (i < MAXINSTRS);
   }
   fclose(tfp);

/* Test calcPolish fn by passing operators to fn that cause two values to be
   popped from stack and operated on based on the operator in .ttl file     */
   ts = stack_init();
/* Line 1 of testinterp.ttl file: Add + == 10 + 5 = 15 */
   tp2.ci = 0;
   stack_push(ts, 10); stack_push(ts, 5);
   calcPolish(&tp2, ts);
   assert(stack_pop(ts, &result));
   assert((result - 15) <  EPSILON);
   assert((result - 15) > -EPSILON);
/* Line 2 of testinterp.ttl file. calcPolish should sub - == 10 - 5 = 5 */
   tp2.ci = 1;
   stack_push(ts, 10); stack_push(ts, 5);
   calcPolish(&tp2, ts);
   assert(stack_pop(ts, &result));
   assert((result - 5) <  EPSILON);
   assert((result - 5) > -EPSILON);
/* Line 3 of testinterp.ttl file. calcPolish should multiply == 10 * 5 = 50 */
   tp2.ci = 2;
   stack_push(ts, 10); stack_push(ts, 5);
   calcPolish(&tp2, ts);
   assert(stack_pop(ts, &result));
   assert((result - 50) <  EPSILON);
   assert((result - 50) > -EPSILON);
/* Line 4 of testinterp.ttl file. calcPolish should divide == 10 / 5 = 2 */
   tp2.ci = 3;
   stack_push(ts, 10); stack_push(ts, 5);
   calcPolish(&tp2, ts);
   assert(stack_pop(ts, &result));
   assert((result - 2) <  EPSILON);
   assert((result - 2) > -EPSILON);

/* Test getVarNumStack fn. Fn should convert a string number to double, and
   convert variable into its saved double value from a Set instruction */
/* Line 5 of testinterp.ttl file. Convert string 50 to double and push to stack */
   tp2.ci = 4;
   getVarNumStack(&tp2, ts);
   stack_pop(ts, &result);
   assert((result - 50) <  EPSILON);
   assert((result - 50) > -EPSILON);
/* Line 6 of testinterp.ttl file. Convert string -50 to double and push to stack */
   tp2.ci = 5;
   getVarNumStack(&tp2, ts);
   stack_pop(ts, &result);
   assert((result + 50) <  EPSILON);
   assert((result + 50) > -EPSILON);
/* Line 7 of testinterp.ttl file. Convert var A to double and push to stack */
   tp2.ci = 6;
/* Set variable A == to 30 */
   tp2.var[0] = 30;
   getVarNumStack(&tp2, ts);
   stack_pop(ts, &result);
   assert((result - 30) <  EPSILON);
   assert((result - 30) > -EPSILON);
/* Line 8 of testinterp.ttl file. Convert var U to double and push to stack */
   tp2.ci = 7;
/* Set variable A == to 30 */
   tp2.var['U'-'A'] = 99;
   getVarNumStack(&tp2, ts);
   stack_pop(ts, &result);
   assert((result - 99) <  EPSILON);
   assert((result - 99) > -EPSILON);

/* Made some changes to calcPolish - check calc fails when not enough vals on stack */
   stack_push(ts, 5);
   assert(!calcPolish(&tp2, ts));
   stack_free(ts);

/* test interpPolish fn. First reset stack  */
   ts = stack_init();
/* Line 9 of testinterp.ttl file. Interp 10 5 + ; and push result to tp2.currVal */
   tp2.ci = 8;
   interpPolish(&tp2, ts);
   assert((tp2.currVal - 15) <  EPSILON);
   assert((tp2.currVal - 15) > -EPSILON);
/* Line 10 of testinterp.ttl file. Interp 1000 5 10 10 * + 5 - / ; and push result to tp2.currVal */
   ts = stack_init();
   tp2.ci = 12;
   interpPolish(&tp2, ts);
   assert((tp2.currVal - 10) <  EPSILON);
   assert((tp2.currVal - 10) > -EPSILON);
/* Line 11 of testinterp.ttl file. Interp A 5 + U - ; and push result to tp2.currVal */
/* A was previoisly set to 30, and U to 99. Therefore 30+5-99 = -64 */
   ts = stack_init();
   tp2.ci = 22;
   interpPolish(&tp2, ts);
   assert((tp2.currVal + 64) <  EPSILON);
   assert((tp2.currVal + 64) > -EPSILON);
/* Line 12 of testinterp.ttl file. Interp A 5 + U -; and push result to tp2.currVal */
/* Reset currVal to 0 as interpPolish should fail due to syntax error */
   ts = stack_init();
   tp2.ci = 28;
   tp2.currVal = 0;
   assert(!interpPolish(&tp2, ts));
   assert(!((tp2.currVal + 64) <  EPSILON));
   assert(!((tp2.currVal - 64) > -EPSILON));

/* test interpPolish fn. First reset stack  */
   ts = stack_init();
/* Line 13 of testinterp.ttl file. Interp SET A := 30 ; and save result to tp2.var[] */
   tp2.ci = 33;
   interpSet(&tp2, ts);
   assert((tp2.var['A'-'A'] - 30) <  EPSILON);
   assert((tp2.var['A'-'A'] - 30) > -EPSILON);
/* Line 14 of testinterp.ttl file. Interp SET B := A 5 + ; and save result to tp2.var[] */
   ts = stack_init();
   tp2.ci = 38;
   interpSet(&tp2, ts);
   assert((tp2.var['B'-'A'] - 35) <  EPSILON);
   assert((tp2.var['B'-'A'] - 35) > -EPSILON);
/* Line 15 of testinterp.ttl file. Interp SET b := A 5 + ; and save result to tp2.var[] */
/* Reset tp2.var[1] to 0 as should fail due to lowercase b */
   ts = stack_init();
   tp2.ci = 45;
   tp2.var['B'-'A'] = 0;
   assert(!interpSet(&tp2, ts));
   assert(!((tp2.var['B'-'A'] + 35) <  EPSILON));
   assert(!((tp2.var['B'-'A'] - 35) > -EPSILON));

/* Test moveFd fn */
/* Line 16 of testinerp.ttl file. FD 30 should just add 30 to y when xyt all = 0 */
   tp2.ci = 52;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   assert(moveFd(&tp2));
   assert((tp2.xyt.y - 30) <  EPSILON);
   assert((tp2.xyt.y - 30) > -EPSILON);
   assert((tp2.xyt.x -  0) <  EPSILON);
   assert((tp2.xyt.x -  0) > -EPSILON);
   assert((tp2.xyt.theta - 0) <  EPSILON);
   assert((tp2.xyt.theta - 0) > -EPSILON);
/* Line 16 of testinerp.ttl file. FD 30 should just add 30 to y. y here is at 10 so = 40 */
   tp2.ci = 52;
   tp2.xyt.x = 0; tp2.xyt.y = 10; tp2.xyt.theta = 0.0;
   moveFd(&tp2);
   assert((tp2.xyt.y - 40) <  EPSILON);
   assert((tp2.xyt.y - 40) > -EPSILON);
/* Line 16 of testinerp.ttl file. FD 30 but theta starts at 90, so x' == -30 */
   tp2.ci = 52;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 90;
   moveFd(&tp2);
   assert((tp2.xyt.x + 30) <  EPSILON);
   assert((tp2.xyt.x + 30) > -EPSILON);
/* Line 17 of testinerp.ttl file. FD A . Set A=30 so should just add 30 to y when xyt all = 0 */
   tp2.ci = 54;
   tp2.var['A'-'A'] = 30;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   assert(moveFd(&tp2));
   assert((tp2.xyt.y - 30) <  EPSILON);
   assert((tp2.xyt.y - 30) > -EPSILON);

/* Test the turnMod function used in the rotate fn */
/* Simulate a RT of 90 when theta = 350 and x&y = 0 by subbing 30 from theta. Should give 150 */
   tp2.xyt.theta = 180 - 30;
   assert(turnMod(&tp2));
   assert((tp2.xyt.theta - 150) <  EPSILON);
   assert((tp2.xyt.theta - 150) > -EPSILON);
/* Simulate a RT of 90 when theta = 350 and x&y = 0 by subbing 90 from theta. Should give 260 */
   tp2.xyt.theta = 350 - 90;
   assert(turnMod(&tp2));
   assert((tp2.xyt.theta - 260) <  EPSILON);
   assert((tp2.xyt.theta - 260) > -EPSILON);
/* Simulate a RT of 90 when theta = 0 and x&y = 0 by subbing 90 from theta. Should give 315 */
   tp2.xyt.theta = 45 - 90;
   assert(turnMod(&tp2));
   assert((tp2.xyt.theta - 315) <  EPSILON);
   assert((tp2.xyt.theta - 315) > -EPSILON);
/* Simulate a LT of 45 when theta = 0 and x&y = 0 by adding 45 to theta. Should give 35 */
   tp2.xyt.theta = 350 + 45;
   assert(turnMod(&tp2));
   assert((tp2.xyt.theta - 35) <  EPSILON);
   assert((tp2.xyt.theta - 35) > -EPSILON);
/* Simulate a LT of 45 when theta = 0 and x&y = 0 by adding 45 to theta. Should give 45 */
   tp2.xyt.theta = 0 + 45;
   assert(turnMod(&tp2));
   assert((tp2.xyt.theta - 45) <  EPSILON);
   assert((tp2.xyt.theta - 45) > -EPSILON);

/* Test the rotate function */
/* Line 18 of testinerp.ttl file. LT 30. Rotate 30 degrees anticlockwise */
   tp2.ci = 56;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   assert(rotate(&tp2));
   assert((tp2.xyt.theta - 30) <  EPSILON);
   assert((tp2.xyt.theta - 30) > -EPSILON);
/* Line 19 of testinerp.ttl file. RT 45. Rotate 45 degrees clockwise from 40 == theta=355 */
   tp2.ci = 58;
   tp2.xyt.theta = 40;
   assert(rotate(&tp2));
   assert((tp2.xyt.theta - 355) <  EPSILON);
   assert((tp2.xyt.theta - 355) > -EPSILON);
/* Line 20 of testinerp.ttl file. RT B (=RT 180). Rotate 180 degrees clockwise from 210 == theta=30 */
   tp2.ci = 60;
   tp2.var['B'-'A'] = 180; tp2.xyt.theta = 210;
   assert(rotate(&tp2));
   assert((tp2.xyt.theta - 30) <  EPSILON);
   assert((tp2.xyt.theta - 30) > -EPSILON);

/* Test modified interpInstruction fn. Same as parser, just calls interp version fns instead */
/* No need to test the values as tested individual FD/LT/RT/SET fns separately */
/* Line 16 of testinerp.ttl file. FD 30 */
   tp2.ci = 52;
   assert(interpInstruction(&tp2));
/* Line 17 of testinerp.ttl file. FD A */
   tp2.ci = 54;
   assert(interpInstruction(&tp2));
/* Line 18 of testinerp.ttl file. LT 30 */
   tp2.ci = 56;
   assert(interpInstruction(&tp2));
/* Line 19 of testinerp.ttl file. RT 45 */
   tp2.ci = 58;
   assert(interpInstruction(&tp2));
/* Line 20 of testinerp.ttl file. RT B */
   tp2.ci = 60;
   assert(interpInstruction(&tp2));
/* Line 21 of testinerp.ttl file. rt 10 - fail */
   tp2.ci = 62;
   assert(!interpInstruction(&tp2));

/* Test modified interpInstrctlst fn. Same as parser, just calls interp fns instead */
/* Line 22 of testinerp.ttl file. FD 30 }. Only y should change as single FD move */
   tp2.ci = 64;
   tp2.xyt.x = 10; tp2.xyt.y = 10; tp2.xyt.theta = 0.0;
   interpInstrctLst(&tp2);
   assert((tp2.xyt.y - 40) <  EPSILON);
   assert((tp2.xyt.y - 40) > -EPSILON);
   assert((tp2.xyt.x - 10) <  EPSILON);
   assert((tp2.xyt.x - 10) > -EPSILON);
   assert((tp2.xyt.theta - 0) <  EPSILON);
   assert((tp2.xyt.theta - 0) > -EPSILON);
/* Line 23 of testinerp.ttl file. FD 30 RT 30 }. y should +30 and theta rotate clockwise 30 */
   tp2.ci = 67;
   tp2.xyt.x = 10; tp2.xyt.y = 10; tp2.xyt.theta = 0.0;
   interpInstrctLst(&tp2);
   assert((tp2.xyt.y - 40) <  EPSILON);
   assert((tp2.xyt.y - 40) > -EPSILON);
   assert((tp2.xyt.x - 10) <  EPSILON);
   assert((tp2.xyt.x - 10) > -EPSILON);
   assert((tp2.xyt.theta - 330) <  EPSILON);
   assert((tp2.xyt.theta - 330) > -EPSILON);
/* Line 24 of testinerp.ttl file. SET A := 50 30 + ; } */
   tp2.ci = 72;
   interpInstrctLst(&tp2);
   assert((tp2.var['A'-'A'] - 80) <  EPSILON);
   assert((tp2.var['A'-'A'] - 80) > -EPSILON);

/* Test modified interpDo function */
/* Line 25 of testinerp.ttl file. DO A FROM 1 TO 10 { } */
   tp2.ci = 80;
   interpDo(&tp2);
   assert((tp2.var['A'-'A'] - 10) <  EPSILON);
   assert((tp2.var['A'-'A'] - 10) > -EPSILON);
/* Line 26 of testinerp.ttl file. DO A FROM 1 TO 10 { FD 30 }*/
   tp2.ci = 88;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   interpDo(&tp2);
   assert((tp2.xyt.y - 300) <  EPSILON);
   assert((tp2.xyt.y - 300) > -EPSILON);
/* Line 30 (simp.ttl) of testinerp.ttl file. DO A FROM 1 TO 10 { FD 30 LT 45 } */
/* As it draws a octagon, x, y and theta should all end up back at the original values */
   tp2.ci = 100;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   interpDo(&tp2);
   assert((tp2.xyt.y - 0) <  EPSILON);
   assert((tp2.xyt.y - 0) > -EPSILON);
   assert((tp2.xyt.x - 0) <  EPSILON);
   assert((tp2.xyt.x - 0) > -EPSILON);
   assert((tp2.xyt.theta - 0) <  EPSILON);
   assert((tp2.xyt.theta - 0) > -EPSILON);

/* Test modified interpProg fn */
/* Using the provided to .ttl files from github, pasted into testinterp.ttl */
/* simp.ttl - starting on line 29 to include exterior { } around instructions */
   tp2.ci = 99;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   interpProg(&tp2);
   assert((tp2.xyt.y - 0) <  EPSILON);
   assert((tp2.xyt.y - 0) > -EPSILON);
   assert((tp2.xyt.x - 0) <  EPSILON);
   assert((tp2.xyt.x - 0) > -EPSILON);
   assert((tp2.xyt.theta - 0) <  EPSILON);
   assert((tp2.xyt.theta - 0) > -EPSILON);
   assert((tp2.var['A'-'A'] - 8) <  EPSILON);
   assert((tp2.var['A'-'A'] - 8) > -EPSILON);

/* spiral1_c.ttl - line 37 */
   tp2.ci = 114;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   interpProg(&tp2);
   printf("x: %f, y: %f, t: %f\n", tp2.xyt.x, tp2.xyt.y, tp2.xyt.theta);
   assert((tp2.var['A'-'A'] - 100) <  EPSILON);
   assert((tp2.var['A'-'A'] - 100) > -EPSILON);
   assert((tp2.xyt.y - 110.58184) <  EPSILON);
   assert((tp2.xyt.y - 110.58184) > -EPSILON);
   assert((tp2.xyt.x - -93.85045) <  EPSILON);
   assert((tp2.xyt.x - -93.85045) > -EPSILON);
   assert((tp2.xyt.theta - 280) <  EPSILON);
   assert((tp2.xyt.theta - 280) > -EPSILON);

/* spiral2_c.ttl - line 46 */
   tp2.ci = 136;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   interpProg(&tp2);
   printf("x: %f, y: %f, t: %f\n", tp2.xyt.x, tp2.xyt.y, tp2.xyt.theta);
   printf("var[A] = %f var[B] = %f var[C] = %f\n", tp2.var['A'-'A'], tp2.var['B'-'A'], tp2.var['C'-'A']);
   assert((tp2.var['A'-'A'] - 50) <  EPSILON);
   assert((tp2.var['A'-'A'] - 50) > -EPSILON);
   assert((tp2.var['B'-'A'] -  8) <  EPSILON);
   assert((tp2.var['B'-'A'] -  8) > -EPSILON);
   assert((tp2.var['C'-'A'] - 10) <  EPSILON);
   assert((tp2.var['C'-'A'] - 10) > -EPSILON);
   assert((tp2.xyt.y - 68.301270) <  EPSILON);
   assert((tp2.xyt.y - 68.301270) > -EPSILON);
   assert((tp2.xyt.x - -64.56921) <  EPSILON);
   assert((tp2.xyt.x - -64.56921) > -EPSILON);
   assert((tp2.xyt.theta - 300) <  EPSILON);
   assert((tp2.xyt.theta - 300) > -EPSILON);
   printf("\n%d = %s\n\n", tp2.ci, tp2.instr[tp2.ci]);

/* simp_c.ttl - line 59 */
   tp2.ci = 170;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   interpProg(&tp2);
   assert((tp2.xyt.y - 0) <  EPSILON);
   assert((tp2.xyt.y - 0) > -EPSILON);
   assert((tp2.xyt.x - 0) <  EPSILON);
   assert((tp2.xyt.x - 0) > -EPSILON);
   assert((tp2.xyt.theta - 0) <  EPSILON);
   assert((tp2.xyt.theta - 0) > -EPSILON);
   printf("\n%d = %s\n\n", tp2.ci, tp2.instr[tp2.ci]);
}

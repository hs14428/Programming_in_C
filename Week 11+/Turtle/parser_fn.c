#include "parser_fn.h"

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
   if (!strsame(p->instr[p->ci], OPEN)) {
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
   if (strsame(p->instr[p->ci], CLOSE)) {
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
   if (!strsame(p->instr[p->ci++], FD)) {
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
   if (!strsame(p->instr[p->ci++], LT)) {
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
   if (!strsame(p->instr[p->ci++], RT)) {
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
   if (!strsame(p->instr[p->ci++], DO)) {
      return false;
   }
   /* p->ci = p->ci + 1; */
   if (!Var(p)) {
      return false;
   }
   p->ci = p->ci + 1;
   if (!strsame(p->instr[p->ci++], FROM)) {
      return false;
   }
   /* p->ci = p->ci + 1; */
   if (!VarNum(p)) {
      return false;
   }
   p->ci = p->ci + 1;
   if (!strsame(p->instr[p->ci++], TO)) {
      return false;
   }
   /* p->ci = p->ci + 1; */
   if (!VarNum(p)) {
      return false;
   }
   p->ci = p->ci + 1;
   if (!strsame(p->instr[p->ci++], OPEN)) {
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
      if (p->instr[p->ci][0] >= A && p->instr[p->ci][0] <= Z) {
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
   if (!strsame(p->instr[p->ci++], SET)) {
      return false;
   }
   /* p->ci = p->ci + 1; */
   if (!Var(p)) {
      return false;
   }
   p->ci = p->ci + 1;
   if (!strsame(p->instr[p->ci++], EQUAL)) {
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
   if (strsame(p->instr[p->ci], ENDPOLISH)) {
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

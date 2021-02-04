#include "interp_fn.h"

void SDLInit(Program *p)
{
   Neill_SDL_Init(&p->sw);
   Neill_SDL_SetDrawColour(&p->sw, WHITE, WHITE, WHITE);
   Neill_SDL_UpdateScreen(&p->sw);
}

void Loop_SDL_Events(Program *p)
{
   while (!p->sw.finished) {
      Neill_SDL_Events(&p->sw);
   }
   SDL_Quit();
   atexit(SDL_Quit);
}

Program ReadFile(char *filename)
{
   int i;
   FILE *fp;
   Program p;
   p.ci = 0;
   p.xyt.x = WWIDTH/2;
   p.xyt.y = WHEIGHT/2;
   for (i = 0; i < MAXINSTRS; i++) {
      p.instr[i][0] = '\0';
   }
   if (!(fp = nfopen(filename, "r"))) {
      fprintf(stderr, "Cannot open %s\n", filename);
      exit(EXIT_FAILURE);
   }
   i = 0;
   while (fscanf(fp, "%s", p.instr[i++]) == 1 && i < MAXINSTRS) {
      assert (i < MAXINSTRS);
   }
   fclose(fp);
   return p;
}

bool interpProg(Program *p)
{
   if (!strsame(p->instr[p->ci], OPEN)) {
      ERROR("No opening { in input?\n");
      return false;
   }
   p->ci = p->ci + 1;
   if (!interpInstrctLst(p)) {
      return false;
   }
   return true;
}

bool interpInstrctLst(Program *p)
{
   if (strsame(p->instr[p->ci], CLOSE)) {
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

bool interpInstruction(Program *p)
{
   stack *s;
   if (moveFd(p)) {
      return true;
   }
   if (rotate(p)) {
      return true;
   }
   if (interpDo(p)) {
      return true;
   }
   s = stack_init();
   if (interpSet(p, s)) {
      return true;
   }
   stack_free(s);
   return false;
}

bool moveFd(Program *p)
{
   double rads, val;
   if (!strsame(p->instr[p->ci], FD)) {
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
      drawLine(p);
      return true;
   }
   return false;
}

void drawLine(Program *p)
{
   SDL_Delay(DELAY);
   SDL_RenderDrawLine(p->sw.renderer, p->old_xyt.x, \
                      p->old_xyt.y, p->xyt.x, p->xyt.y);
   Neill_SDL_UpdateScreen(&p->sw);
   Neill_SDL_Events(&p->sw);
}

double getVarNum(Program *p)
{
   int index;
   double val;
   char* ptr;

   if (Var(p)) {
      index = p->instr[p->ci][0];
      val = p->var[index - A];
      return val;
   }
   else {
      val = strtod(p->instr[p->ci], &ptr);
      return val;
   }
}

bool rotate(Program *p)
{
   if (strsame(p->instr[p->ci], LT)) {
      p->ci = p->ci + 1;
      if (VarNum(p)) {
         p->xyt.theta += getVarNum(p);
         if (turnMod(p)) {
            return true;
         }
      }
   }
   else if (strsame(p->instr[p->ci], RT)) {
      p->ci = p->ci + 1;
      if (VarNum(p)) {
         p->xyt.theta -= getVarNum(p);
         if (turnMod(p)) {
            return true;
         }
      }
   }
   else {
      return false;
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

bool interpDo(Program *p)
{
   double from, to;
   double saveVar = p->currVar;
   if (!strsame(p->instr[p->ci], DO)) {
      return false;
   }
   p->ci = p->ci + 1;
   if (Var(p)) {
      p->currVar = p->instr[p->ci++][0];
      if (strsame(p->instr[p->ci++], FROM)) {
         if (VarNum(p)) {
            from = getVarNum(p);
            p->ci = p->ci + 1;
            if (strsame(p->instr[p->ci++], TO)) {
               if (VarNum(p)) {
                  to = getVarNum(p);
                  if (to < from) {
                     ERROR("End of DO loop < start.\n");
                  }
                  p->ci = p->ci + 1;
                  if (strsame(p->instr[p->ci++], OPEN)) {
                     if (doLoop(p, saveVar, from, to)) {
                        return true;
                     }
                  }
               }
            }
         }
      }
   }
   return false;
}

bool doLoop(Program *p, double var, double from, double to)
{
   int wi;
   wi = p->ci;
   p->var[p->currVar-A] = from;
   while (p->var[p->currVar-A] <= to) {
      p->ci = wi;
      if (!interpInstrctLst(p)) {
         return false;
      }
      p->var[p->currVar-A]++;
   }
   p->var[p->currVar-A]--;
   p->currVar = var;
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
   int i, len, dec;
   dec = 0;
   len = strlen(p->instr[p->ci]);
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

bool interpSet(Program *p, stack *s)
{
   char var;
   if (!strsame(p->instr[p->ci++], SET)) {
      return false;
   }
   if (Var(p)) {
      var = p->instr[p->ci++][0];
      if (strsame(p->instr[p->ci++], EQUAL)) {
         interpPolish(p, s);
         p->var[var-A] = p->currVal;
         return true;
      }
   }
   return false;
}

bool interpPolish(Program *p, stack *s)
{
   double result;
   if (strsame(p->instr[p->ci], ENDPOLISH)) {
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
   else {
      return false;
   }
   return false;
}

bool stackCheck(stack *s)
{
   double d;
   if (stack_peek(s, &d) == true) {
      fprintf(stderr, "Stack still had items on it. Check \
                       instructions are correct.\n");
      stack_free(s);
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
      val = p->var[index - A];
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
   double d, p1, p2;
   if (stack_peek(s, &d)) {
      stack_pop(s, &p1);
      if (stack_peek(s, &d)) {
      stack_pop(s, &p2);
      }
      else {
         ERROR("No items on the stack. Cannot operate.\n");
         return false;
      }
   }
   else {
      ERROR("No items on the stack. Cannot operate.\n");
      return false;
   }
   if (polishMaths(p, s, p1, p2)) {
      return true;
   }
   return false;
}

bool polishMaths(Program *p, stack *s, double p1, double p2)
{
   if (strsame(p->instr[p->ci], ADD)) {
      stack_push(s, p2 + p1);
      return true;
   }
   if (strsame(p->instr[p->ci], SUB)) {
      stack_push(s, p2 - p1);
      return true;
   }
   if (strsame(p->instr[p->ci], MUL)) {
      stack_push(s, p2 * p1);
      return true;
   }
   if (strsame(p->instr[p->ci], DIV)) {
      if (dblEquiv(p1, 0)) {
         ERROR("Cannot divide by zero.\n");
         return false;
      }
      stack_push(s, p2 / p1);
      return true;
   }
   ERROR("Polish calculations failed.\n");
   return false;
}

bool dblEquiv(double num, double equiv)
{
   if ((num - equiv) <  EPSILON) {
      if ((num - equiv) >  -EPSILON) {
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

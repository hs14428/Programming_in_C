#include "extension_fn.h"

/* New functions for extension.c */

bool yesFile(char* word)
{
   if ((strsame(word, "yes")) || (strsame(word, "Yes")) ||
       (strsame(word, "YES")) || (strsame(word, "Y"))   ||
       (strsame(word, "y"))) {
      return true;
   }
   else {
      return false;
   }
}

bool noFile(char* word)
{
   if ((strsame(word, "no")) || (strsame(word, "No")) ||
       (strsame(word, "NO")) || (strsame(word, "N"))  ||
       (strsame(word, "n"))) {
      return true;
   }
   else {
      return false;
   }
}

bool quitFile(char* word)
{
   if ((strsame(word, "quit")) || (strsame(word, "QUIT")) ||
       (strsame(word, "Quit")) || (strsame(word, "Q"))  ||
       (strsame(word, "q"))) {
      return true;
   }
   else {
      return false;
   }
}

void welcome()
{
   fprintf(stdout, "Welcome.\n\n"
                   "Do you want to import a .ttl file "
                   "(Y/N)? If no, you can type in "
                   "the instructions instead.\n\n");
}

void interactiveFile()
{
   double x, y;
   char word[BUFFER] = "\0";
   Program prog;

   fprintf(stdout, "\nPlease enter the .ttl file name\n\n");
   if (scanf("%s", word) != 1) {
      SCANERROR("Scanf error");
   }
   fprintf(stdout, "\nPlease enter starting x and y coordinates\n\n");
   if (scanf("%le %le", &x, &y) != 2) {
      SCANERROR("Scanf error");
   }
   prog = ReadFile(word, x, y);
   fprintf(stdout, "\nWhat colour do you want the lines to be? "
                   "Please pick from RED/GREEN/BLUE/WHITE/RAND\n\n");
   if (scanf("%s", word) != 1) {
      SCANERROR("Scanf error");
   }
   runInterp(&prog, word);
}

void interactiveType()
{
   double x, y;
   char word[BUFFER] = "\0";
   Program prog;

   fprintf(stdout, "\nBefore entering the turtle instructions"
                   ", please enter the starting x and y "
                   "coordinates.\n\n");
   if (scanf("%le %le", &x, &y) != 2) {
      SCANERROR("Scanf error");
   }
   fprintf(stdout, "\nWhat colour do you want the lines to be? "
                   "Please pick from RED/GREEN/BLUE/WHITE/RAND\n\n");
   if (scanf("%s", word) != 1) {
      SCANERROR("Scanf error");
   }
   fprintf(stdout, "\nNow please type in the instructions, "
                   "following the standard grammar.\nTo "
                   "quit, please type QUIT.\n\n");
   prog = typeInFile(x, y);
   runInterp(&prog, word);
}

void runInterp(Program *prog, char *colour)
{
   SDLInit(prog, colour);
   if (!interpProg(prog)) {
      fprintf(stderr, "Issue around word pos: %d = %s\n",\
      prog->ci, prog->instr[prog->ci]);
      exit(EXIT_FAILURE);
   }
   else {
      Loop_SDL_Events(prog);
   }
}

Program typeInFile(double x, double y)
{
   int i;
   char word[BUFFER] = "\0";
   Program p;
   p.ci = 0;
   p.xyt.x = x;
   p.xyt.y = y;
   for (i = 0; i < MAXINSTRS; i++) {
      p.instr[i][0] = '\0';
   }

   i = 0;
   while (!(quitFile(word)) && i < MAXINSTRS) {
      assert(i < MAXINSTRS);
      if (scanf("%s", word) != 1) {
         SCANERROR("Scanf error");
      }
      strcpy(p.instr[i++], word);
   }
   return p;
}

/* Edited original interp.c functions */

/* Optional function to decide the loop increment amount */
/* e.g. DO A FROM 1 TO 10 JUMP 5 = increment by 5 each time */
bool Jump(Program *p)
{
   if (strsame(p->instr[p->ci], JUMP)) {
      p->ci = p->ci + 1;
      if (VarNum(p)) {
         p->jump = getVarNum(p);
         return true;
      }
   }
   return false;
}

/* Added jump check */
bool interpInstruction(Program *p)
{
   stack *s;
   if (moveFd(p)) {
      return true;
   }
   if (rotate(p)) {
      return true;
   }
   if (Jump(p)) {
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

/* Added jump functionality */
bool interpDo(Program *p)
{
   double from, to;
   int j = 1;
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
                  if (Jump(p)) {
                     j = p->jump;
                     p->ci = p->ci + 1;
                  }
                  if (strsame(p->instr[p->ci++], OPEN)) {
                     if (doLoop(p, saveVar, from, to, j)) {
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

/* Added jump functionality */
bool doLoop(Program *p, double var, double from, double to, int j)
{
   int wi;
   wi = p->ci;
   p->var[p->currVar-A] = from;
   while (p->var[p->currVar-A] <= to) {
      p->ci = wi;
      if (!interpInstrctLst(p)) {
         return false;
      }
      p->var[p->currVar-A] += j;
   }
   p->var[p->currVar-A] -= j;
   p->currVar = var;
   return true;
}

/* Added Mod */
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
   if (strsame(p->instr[p->ci], MOD)) {
      stack_push(s, fmod(p2, p1));
      return true;
   }
   ERROR("Polish calculations failed.\n");
   return false;
}

/* Added MOD */
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
   if (strsame(p->instr[p->ci], MOD)) {
      return true;
   }
   return false;
}

/* Added custom colour options */
void SDLInit(Program *p, char *colour)
{
   if (strsame(colour, WHITE)) {
      Neill_SDL_Init(&p->sw);
      Neill_SDL_SetDrawColour(&p->sw, WHITERGB, WHITERGB, WHITERGB);
      Neill_SDL_UpdateScreen(&p->sw);
   }
   else if (strsame(colour, RED)) {
      Neill_SDL_Init(&p->sw);
      Neill_SDL_SetDrawColour(&p->sw, REDR, REDG, REDB);
      Neill_SDL_UpdateScreen(&p->sw);
   }
   else if (strsame(colour, BLUE)) {
      Neill_SDL_Init(&p->sw);
      Neill_SDL_SetDrawColour(&p->sw, BLUER, BLUEG, BLUEB);
      Neill_SDL_UpdateScreen(&p->sw);
   }
   else if (strsame(colour, GREEN)) {
      Neill_SDL_Init(&p->sw);
      Neill_SDL_SetDrawColour(&p->sw, GREENR, GREENG, GREENB);
      Neill_SDL_UpdateScreen(&p->sw);
   }
   else if (strsame(colour, RAND)) {
      srand((unsigned)time(NULL));
      p->rand = 1;
      Neill_SDL_Init(&p->sw);
      Neill_SDL_SetDrawColour(&p->sw, RANDC, RANDC, RANDC);
      Neill_SDL_UpdateScreen(&p->sw);
   }
   else {
      fprintf(stdout, "\nIncorrect colour type.\n\n");
      exit(EXIT_FAILURE);
   }
}

/* Added update random colours for RAND colour option */
void drawLine(Program *p)
{
   SDL_Delay(DELAY);
   if (p->rand == 1) {
      srand((unsigned)time(NULL));
      Neill_SDL_SetDrawColour(&p->sw, RANDC, RANDC, RANDC);
   }
   SDL_RenderDrawLine(p->sw.renderer, p->old_xyt.x, \
                      p->old_xyt.y, p->xyt.x, p->xyt.y);
   Neill_SDL_UpdateScreen(&p->sw);
   Neill_SDL_Events(&p->sw);
}

/* All functions below here are same as interp_fn.c and are unchanged */

void Loop_SDL_Events(Program *p)
{
   while (!p->sw.finished) {
      Neill_SDL_Events(&p->sw);
   }
   SDL_Quit();
   atexit(SDL_Quit);
}

Program ReadFile(char *filename, double x, double y)
{
   int i;
   FILE *fp;
   Program p;
   p.ci = 0;
   p.xyt.x = x;
   p.xyt.y = y;
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

bool dblEquiv(double num, double equiv)
{
   if ((num - equiv) <  EPSILON) {
      if ((num - equiv) >  -EPSILON) {
         return true;
      }
   }
   return false;
}

#ifndef INTERP_FN_H
#define INTERP_FN_H

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
#define DELAY 30
#define WHITE 255
#define A 'A'
#define Z 'Z'
#define FD "FD"
#define RT "RT"
#define LT "LT"
#define DO "DO"
#define FROM "FROM"
#define TO "TO"
#define SET "SET"
#define EQUAL ":="
#define OPEN "{"
#define CLOSE "}"
#define ENDPOLISH ";"
#define ADD "+"
#define SUB "-"
#define MUL "*"
#define DIV "/"
#define strsame(A, B) (strcmp(A, B) ==0)
#define ERROR(PHRASE) {fprintf(stderr, "Fatal Error. %s Occured in %s, line %d\n", PHRASE, __FILE__, __LINE__); }

typedef struct coordinates {
   double x;
   double y;
   double theta;
} Coords;

typedef struct prog {
   char   instr[MAXINSTRS][MAXINSTRLEN];
   int    ci; /* Current instruction */
   double currVal;
   char   currVar;
   double var[ALPHANUM];
   Coords old_xyt;
   Coords xyt;
   SDL_Simplewin sw;
} Program;

void test();
void SDLInit(Program *p);
void Loop_SDL_Events(Program *p);
Program ReadFile(char *filename);
bool interpProg(Program *p);
bool interpInstrctLst(Program *p);
bool interpInstruction(Program *p);
bool moveFd(Program *p);
void drawLine(Program *p);
double getVarNum(Program *p);
bool rotate(Program *p);
bool turnMod(Program *p);
bool interpDo(Program *p);
bool doLoop(Program *p, double var, double from, double to);
bool Var(Program *p);
bool VarNum(Program *p);
bool isValNumStart(Program *p);
bool interpSet(Program *p, stack *s);
bool interpPolish(Program *p, stack *s);
bool stackCheck(stack *s);
bool getVarNumStack(Program *p, stack *s);
bool calcPolish(Program *p, stack *s);
bool polishMaths(Program *p, stack *s, double p1, double p2);
bool dblEquiv(double num, double equiv);
bool Op(Program *p);

#endif

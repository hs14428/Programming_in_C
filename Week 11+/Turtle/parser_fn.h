#ifndef PARSER_FN_H
#define PARSER_FN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "General/general.h"

#define MAXINSTRS 1000
#define MAXINSTRLEN 15
#define MAXFPLEN 30
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

#endif

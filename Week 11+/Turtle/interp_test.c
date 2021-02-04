#include "interp_fn.h"

void test()
{
   int i;
   FILE *tfp;
   char *filename;
   Program tp2;
   stack* ts;
   double result;

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

/* Test double equivalence fn */
   assert( dblEquiv(15, 15));
   assert(!dblEquiv(15, 15.1));
   assert( dblEquiv(15.00001, 15.00001));
   assert( dblEquiv(15.00001, 15.00002));
   assert(!dblEquiv(15.00001, 15.00003));
   assert( dblEquiv(15.000001, 15.000002));

/* Test calcPolish fn by passing operators to fn that cause two values to be
   popped from stack and operated on based on the operator in .ttl file     */
   ts = stack_init();
/* Line 1 of testinterp.ttl file: Add + == 10 + 5 = 15 */
   tp2.ci = 0;
   stack_push(ts, 10); stack_push(ts, 5);
   calcPolish(&tp2, ts);
   assert(stack_pop(ts, &result));
   assert(dblEquiv(result, 15));
/*    assert((result - 15) <  EPSILON);
   assert((result - 15) > -EPSILON); */
/* Line 2 of testinterp.ttl file. calcPolish should sub - == 10 - 5 = 5 */
   tp2.ci = 1;
   stack_push(ts, 10); stack_push(ts, 5);
   calcPolish(&tp2, ts);
   assert(stack_pop(ts, &result));
   assert(dblEquiv(result, 5));
/*    assert((result - 5) <  EPSILON);
   assert((result - 5) > -EPSILON); */
/* Line 3 of testinterp.ttl file. calcPolish should multiply == 10 * 5 = 50 */
   tp2.ci = 2;
   stack_push(ts, 10); stack_push(ts, 5);
   calcPolish(&tp2, ts);
   assert(stack_pop(ts, &result));
   assert(dblEquiv(result, 50));
/*    assert((result - 50) <  EPSILON);
   assert((result - 50) > -EPSILON); */
/* Line 4 of testinterp.ttl file. calcPolish should divide == 10 / 5 = 2 */
   tp2.ci = 3;
   stack_push(ts, 10); stack_push(ts, 5);
   calcPolish(&tp2, ts);
   assert(stack_pop(ts, &result));
   assert(dblEquiv(result, 2));
/*    assert((result - 2) <  EPSILON);
   assert((result - 2) > -EPSILON); */

/* Test getVarNumStack fn. Fn should convert a string number to double, and
   convert variable into its saved double value from a Set instruction */
/* Line 5 of testinterp.ttl file. Convert string 50 to double and push to stack */
   tp2.ci = 4;
   getVarNumStack(&tp2, ts);
   stack_pop(ts, &result);
   assert(dblEquiv(result, 50));
/*    assert((result - 50) <  EPSILON);
   assert((result - 50) > -EPSILON); */
/* Line 6 of testinterp.ttl file. Convert string -50 to double and push to stack */
   tp2.ci = 5;
   getVarNumStack(&tp2, ts);
   stack_pop(ts, &result);
   assert(dblEquiv(result, -50));
/*    assert((result + 50) <  EPSILON);
   assert((result + 50) > -EPSILON); */
/* Line 7 of testinterp.ttl file. Convert var A to double and push to stack */
   tp2.ci = 6;
/* Set variable A == to 30 */
   tp2.var[0] = 30;
   getVarNumStack(&tp2, ts);
   stack_pop(ts, &result);
   assert(dblEquiv(result, 30));
/*    assert((result - 30) <  EPSILON);
   assert((result - 30) > -EPSILON); */
/* Line 8 of testinterp.ttl file. Convert var U to double and push to stack */
   tp2.ci = 7;
/* Set variable A == to 30 */
   tp2.var['U'-'A'] = 99;
   getVarNumStack(&tp2, ts);
   stack_pop(ts, &result);
   assert(dblEquiv(result, 99));
/*    assert((result - 99) <  EPSILON);
   assert((result - 99) > -EPSILON); */

/* Made some changes to calcPolish - check calc fails when not enough vals on stack */
   stack_push(ts, 5);
   assert(!calcPolish(&tp2, ts));
   stack_free(ts);

/* test interpPolish fn. First reset stack  */
   ts = stack_init();
/* Line 9 of testinterp.ttl file. Interp 10 5 + ; and push result to tp2.currVal */
   tp2.ci = 8;
   interpPolish(&tp2, ts);
   assert(dblEquiv(tp2.currVal, 15));
/*    assert((tp2.currVal - 15) <  EPSILON);
   assert((tp2.currVal - 15) > -EPSILON); */
/* Line 10 of testinterp.ttl file. Interp 1000 5 10 10 * + 5 - / ; and push result to tp2.currVal */
   ts = stack_init();
   tp2.ci = 12;
   interpPolish(&tp2, ts);
   assert(dblEquiv(tp2.currVal, 10));
/*    assert((tp2.currVal - 10) <  EPSILON);
   assert((tp2.currVal - 10) > -EPSILON); */
/* Line 11 of testinterp.ttl file. Interp A 5 + U - ; and push result to tp2.currVal */
/* A was previoisly set to 30, and U to 99. Therefore 30+5-99 = -64 */
   ts = stack_init();
   tp2.ci = 22;
   interpPolish(&tp2, ts);
   assert(dblEquiv(tp2.currVal, -64));
/*    assert((tp2.currVal + 64) <  EPSILON);
   assert((tp2.currVal + 64) > -EPSILON); */
/* Line 12 of testinterp.ttl file. Interp A 5 + U -; and push result to tp2.currVal */
/* Reset currVal to 0 as interpPolish should fail due to syntax error */
   ts = stack_init();
   tp2.ci = 28;
   tp2.currVal = 0;
   assert(!interpPolish(&tp2, ts));
   assert(!dblEquiv(tp2.currVal, -64));
/*    assert(!((tp2.currVal + 64) <  EPSILON));
   assert(!((tp2.currVal - 64) > -EPSILON)); */
   stack_free(ts);

/* test interpPolish fn. First reset stack  */
   ts = stack_init();
/* Line 13 of testinterp.ttl file. Interp SET A := 30 ; and save result to tp2.var[] */
   tp2.ci = 33;
   interpSet(&tp2, ts);
   assert(dblEquiv(tp2.var['A'-'A'], 30));
/*    assert((tp2.var['A'-'A'] - 30) <  EPSILON);
   assert((tp2.var['A'-'A'] - 30) > -EPSILON); */
/* Line 14 of testinterp.ttl file. Interp SET B := A 5 + ; and save result to tp2.var[] */
   ts = stack_init();
   tp2.ci = 38;
   interpSet(&tp2, ts);
   assert(dblEquiv(tp2.var['B'-'A'], 35));
/*    assert((tp2.var['B'-'A'] - 35) <  EPSILON);
   assert((tp2.var['B'-'A'] - 35) > -EPSILON); */
/* Line 15 of testinterp.ttl file. Interp SET b := A 5 + ; and save result to tp2.var[] */
/* Reset tp2.var[1] to 0 as should fail due to lowercase b */
   ts = stack_init();
   tp2.ci = 45;
   tp2.var['B'-'A'] = 0;
   assert(!interpSet(&tp2, ts));
   assert(!dblEquiv(tp2.var['B'-'A'], 35));
/*    assert(!((tp2.var['B'-'A'] + 35) <  EPSILON));
   assert(!((tp2.var['B'-'A'] - 35) > -EPSILON)); */
   stack_free(ts);

/* Test moveFd fn */
/* Line 16 of testinerp.ttl file. FD 30 should just add 30 to y when xyt all = 0 */
   tp2.ci = 52;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   assert(moveFd(&tp2));
   assert(dblEquiv(tp2.xyt.y, 30));
   assert(dblEquiv(tp2.xyt.y, 30));
   assert(dblEquiv(tp2.xyt.theta, 0));
/*    assert((tp2.xyt.y - 30) <  EPSILON);
   assert((tp2.xyt.y - 30) > -EPSILON);
   assert((tp2.xyt.x -  0) <  EPSILON);
   assert((tp2.xyt.x -  0) > -EPSILON);
   assert((tp2.xyt.theta - 0) <  EPSILON);
   assert((tp2.xyt.theta - 0) > -EPSILON); */
/* Line 16 of testinerp.ttl file. FD 30 should just add 30 to y. y here is at 10 so = 40 */
   tp2.ci = 52;
   tp2.xyt.x = 0; tp2.xyt.y = 10; tp2.xyt.theta = 0.0;
   moveFd(&tp2);
   assert(dblEquiv(tp2.xyt.y, 40));
/*    assert((tp2.xyt.y - 40) <  EPSILON);
   assert((tp2.xyt.y - 40) > -EPSILON); */
/* Line 16 of testinerp.ttl file. FD 30 but theta starts at 90, so x' == -30 */
   tp2.ci = 52;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 90;
   moveFd(&tp2);
   assert(dblEquiv(tp2.xyt.x, -30));
/*    assert((tp2.xyt.x + 30) <  EPSILON);
   assert((tp2.xyt.x + 30) > -EPSILON); */
/* Line 17 of testinerp.ttl file. FD A . Set A=30 so should just add 30 to y when xyt all = 0 */
   tp2.ci = 54;
   tp2.var['A'-'A'] = 30;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   assert(moveFd(&tp2));
   assert(dblEquiv(tp2.xyt.y, 30));
/*    assert((tp2.xyt.y - 30) <  EPSILON);
   assert((tp2.xyt.y - 30) > -EPSILON); */

/* Test the turnMod function used in the rotate fn */
/* Simulate a RT of 90 when theta = 350 and x&y = 0 by subbing 30 from theta. Should give 150 */
   tp2.xyt.theta = 180 - 30;
   assert(turnMod(&tp2));
   assert(dblEquiv(tp2.xyt.theta, 150));
/*    assert((tp2.xyt.theta - 150) <  EPSILON);
   assert((tp2.xyt.theta - 150) > -EPSILON); */
/* Simulate a RT of 90 when theta = 350 and x&y = 0 by subbing 90 from theta. Should give 260 */
   tp2.xyt.theta = 350 - 90;
   assert(turnMod(&tp2));
   assert(dblEquiv(tp2.xyt.theta, 260));
/*    assert((tp2.xyt.theta - 260) <  EPSILON);
   assert((tp2.xyt.theta - 260) > -EPSILON); */
/* Simulate a RT of 90 when theta = 0 and x&y = 0 by subbing 90 from theta. Should give 315 */
   tp2.xyt.theta = 45 - 90;
   assert(turnMod(&tp2));
   assert(dblEquiv(tp2.xyt.theta, 315));
/*    assert((tp2.xyt.theta - 315) <  EPSILON);
   assert((tp2.xyt.theta - 315) > -EPSILON); */
/* Simulate a LT of 45 when theta = 0 and x&y = 0 by adding 45 to theta. Should give 35 */
   tp2.xyt.theta = 350 + 45;
   assert(turnMod(&tp2));
   assert(dblEquiv(tp2.xyt.theta, 35));
/*    assert((tp2.xyt.theta - 35) <  EPSILON);
   assert((tp2.xyt.theta - 35) > -EPSILON); */
/* Simulate a LT of 45 when theta = 0 and x&y = 0 by adding 45 to theta. Should give 45 */
   tp2.xyt.theta = 0 + 45;
   assert(turnMod(&tp2));
   assert(dblEquiv(tp2.xyt.theta, 45));
/*    assert((tp2.xyt.theta - 45) <  EPSILON);
   assert((tp2.xyt.theta - 45) > -EPSILON); */

/* Test the rotate function */
/* Line 18 of testinerp.ttl file. LT 30. Rotate 30 degrees anticlockwise */
   tp2.ci = 56;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   assert(rotate(&tp2));
   assert(dblEquiv(tp2.xyt.theta, 30));
/*    assert((tp2.xyt.theta - 30) <  EPSILON);
   assert((tp2.xyt.theta - 30) > -EPSILON); */
/* Line 19 of testinerp.ttl file. RT 45. Rotate 45 degrees clockwise from 40 == theta=355 */
   tp2.ci = 58;
   tp2.xyt.theta = 40;
   assert(rotate(&tp2));
   assert(dblEquiv(tp2.xyt.theta, 355));
/*    assert((tp2.xyt.theta - 355) <  EPSILON);
   assert((tp2.xyt.theta - 355) > -EPSILON); */
/* Line 20 of testinerp.ttl file. RT B (=RT 180). Rotate 180 degrees clockwise from 210 == theta=30 */
   tp2.ci = 60;
   tp2.var['B'-'A'] = 180; tp2.xyt.theta = 210;
   assert(rotate(&tp2));
   assert(dblEquiv(tp2.xyt.theta, 30));
/*    assert((tp2.xyt.theta - 30) <  EPSILON);
   assert((tp2.xyt.theta - 30) > -EPSILON); */

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
   assert(dblEquiv(tp2.xyt.y, 40));
   assert(dblEquiv(tp2.xyt.x, 10));
   assert(dblEquiv(tp2.xyt.theta, 0));
/*    assert((tp2.xyt.y - 40) <  EPSILON);
   assert((tp2.xyt.y - 40) > -EPSILON);
   assert((tp2.xyt.x - 10) <  EPSILON);
   assert((tp2.xyt.x - 10) > -EPSILON);
   assert((tp2.xyt.theta - 0) <  EPSILON);
   assert((tp2.xyt.theta - 0) > -EPSILON); */
/* Line 23 of testinerp.ttl file. FD 30 RT 30 }. y should +30 and theta rotate clockwise 30 */
   tp2.ci = 67;
   tp2.xyt.x = 10; tp2.xyt.y = 10; tp2.xyt.theta = 0.0;
   interpInstrctLst(&tp2);
   assert(dblEquiv(tp2.xyt.y, 40));
   assert(dblEquiv(tp2.xyt.x, 10));
   assert(dblEquiv(tp2.xyt.theta, 330));
/*    assert((tp2.xyt.y - 40) <  EPSILON);
   assert((tp2.xyt.y - 40) > -EPSILON);
   assert((tp2.xyt.x - 10) <  EPSILON);
   assert((tp2.xyt.x - 10) > -EPSILON);
   assert((tp2.xyt.theta - 330) <  EPSILON);
   assert((tp2.xyt.theta - 330) > -EPSILON); */
/* Line 24 of testinerp.ttl file. SET A := 50 30 + ; } */
   tp2.ci = 72;
   interpInstrctLst(&tp2);
   assert(dblEquiv(tp2.var['A'-'A'], 80));
/*    assert((tp2.var['A'-'A'] - 80) <  EPSILON);
   assert((tp2.var['A'-'A'] - 80) > -EPSILON); */

/* Test modified interpDo function */
/* Line 25 of testinerp.ttl file. DO A FROM 1 TO 10 { } */
   tp2.ci = 80;
   interpDo(&tp2);
   assert(dblEquiv(tp2.var['A'-'A'], 10));
   /* assert((tp2.var['A'-'A'] - 10) <  EPSILON);
   assert((tp2.var['A'-'A'] - 10) > -EPSILON); */
/* Line 26 of testinerp.ttl file. DO A FROM 1 TO 10 { FD 30 }*/
   tp2.ci = 88;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   interpDo(&tp2);
   assert(dblEquiv(tp2.xyt.y, 300));
/*    assert((tp2.xyt.y - 300) <  EPSILON);
   assert((tp2.xyt.y - 300) > -EPSILON); */
/* Line 30 (simp.ttl) of testinerp.ttl file. DO A FROM 1 TO 10 { FD 30 LT 45 } */
/* As it draws a octagon, x, y and theta should all end up back at the original values */
   tp2.ci = 100;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   interpDo(&tp2);
   assert(dblEquiv(tp2.xyt.y, 0));
   assert(dblEquiv(tp2.xyt.x, 0));
   assert(dblEquiv(tp2.xyt.theta, 0));
   /* assert((tp2.xyt.y - 0) <  EPSILON);
   assert((tp2.xyt.y - 0) > -EPSILON);
   assert((tp2.xyt.x - 0) <  EPSILON);
   assert((tp2.xyt.x - 0) > -EPSILON);
   assert((tp2.xyt.theta - 0) <  EPSILON);
   assert((tp2.xyt.theta - 0) > -EPSILON); */

/* Test modified interpProg fn */
/* Using the provided to .ttl files from github, pasted into testinterp.ttl */
/* simp.ttl - starting on line 29 to include exterior { } around instructions */
   tp2.ci = 99;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   interpProg(&tp2);
   assert(dblEquiv(tp2.xyt.y, 0));
   assert(dblEquiv(tp2.xyt.x, 0));
   assert(dblEquiv(tp2.xyt.theta, 0));
   assert(dblEquiv(tp2.var['A'-'A'], 8));
   /* assert((tp2.xyt.y - 0) <  EPSILON);
   assert((tp2.xyt.y - 0) > -EPSILON);
   assert((tp2.xyt.x - 0) <  EPSILON);
   assert((tp2.xyt.x - 0) > -EPSILON);
   assert((tp2.xyt.theta - 0) <  EPSILON);
   assert((tp2.xyt.theta - 0) > -EPSILON);
   assert((tp2.var['A'-'A'] - 8) <  EPSILON);
   assert((tp2.var['A'-'A'] - 8) > -EPSILON); */

/* spiral1_c.ttl - line 37 */
   tp2.ci = 114;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   interpProg(&tp2);
   assert(dblEquiv(tp2.xyt.y, 110.58184));
   assert(dblEquiv(tp2.xyt.x, -93.85045));
   assert(dblEquiv(tp2.xyt.theta, 280));
   assert(dblEquiv(tp2.var['A'-'A'], 100));
   /* assert((tp2.var['A'-'A'] - 100) <  EPSILON);
   assert((tp2.var['A'-'A'] - 100) > -EPSILON);
   assert((tp2.xyt.y - 110.58184) <  EPSILON);
   assert((tp2.xyt.y - 110.58184) > -EPSILON);
   assert((tp2.xyt.x - -93.85045) <  EPSILON);
   assert((tp2.xyt.x - -93.85045) > -EPSILON);
   assert((tp2.xyt.theta - 280) <  EPSILON);
   assert((tp2.xyt.theta - 280) > -EPSILON); */

/* spiral2_c.ttl - line 46 */
   tp2.ci = 136;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   interpProg(&tp2);
   assert(dblEquiv(tp2.xyt.y, 68.301270));
   assert(dblEquiv(tp2.xyt.x, -64.56921));
   assert(dblEquiv(tp2.xyt.theta, 300));
   assert(dblEquiv(tp2.var['A'-'A'], 50));
   assert(dblEquiv(tp2.var['B'-'A'], 8));
   assert(dblEquiv(tp2.var['C'-'A'], 10));
   /* assert((tp2.var['A'-'A'] - 50) <  EPSILON);
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
   assert((tp2.xyt.theta - 300) > -EPSILON); */

/* simp_c.ttl - line 59 */
   tp2.ci = 170;
   tp2.xyt.x = 0; tp2.xyt.y = 0; tp2.xyt.theta = 0;
   interpProg(&tp2);
   assert(dblEquiv(tp2.xyt.y, 0));
   assert(dblEquiv(tp2.xyt.x, 0));
   assert(dblEquiv(tp2.xyt.theta, 0));
   /* assert((tp2.xyt.y - 0) <  EPSILON);
   assert((tp2.xyt.y - 0) > -EPSILON);
   assert((tp2.xyt.x - 0) <  EPSILON);
   assert((tp2.xyt.x - 0) > -EPSILON);
   assert((tp2.xyt.theta - 0) <  EPSILON);
   assert((tp2.xyt.theta - 0) > -EPSILON); */

}

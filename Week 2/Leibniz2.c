#include <stdio.h>
#include <math.h>
#include <assert.h>

#define PI9 3.141592653
#define EPSILON 0.00000001
#define NUMERATOR 4.0
#define DENOMINATOR 3.0

int piEquation(void);
double piFraction(int i);

double main(void)
{
   int p;
   p = piEquation();
   printf("It takes %d iterations to approx pi to 9 dp", p);
}

int piEquation(void)
{
   double pi, d;
   int i = 0, n = 1;
   int inv = pow(-1.0, n);
   /*printf("Fraction: %.9f\n", piFraction(i));*/
   pi = NUMERATOR + (inv * piFraction((double) i));
   n++;
   i++;
   /*printf("Pi: %.9f\nPI9: %.9f\n", pi, PI9);*/
   /*printf("Difference: %.9f\nEps: %.9f\n", d, EPSILON);*/

   while (fabs(pi - PI9) > EPSILON) {
      /*printf("Difference: %.9f\nEps: %.9f\n", d, EPSILON);*/
      pi = pi + (inv * piFraction((double) i));
      printf("Fraction: %.9f\n", piFraction(i));
      printf("Cnt: %d Pi: %.9f\n", i, pi);
      i++;
      n++;
   }
   return i;
}

double piFraction(int i)
{
   double fraction;

   fraction = (NUMERATOR/(DENOMINATOR + (2.0*i)));
   assert(fraction != 0.0);
   return fraction;
}

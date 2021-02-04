#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#define EPSILON 0.01

typedef float length;
typedef enum {false, true} bool;

void tri_angle(length a, length b, length c);
float maxtwo(float a, float b);
float maxthree(float a, float b,float c);
void tri_type(length a, length b, length c);
bool compare_floats_equality(float a, float b);

int main(void)
{
    length a;
    length b;
    length c;
    printf("enter three numbers:\n");
    scanf("%f", &a);
    scanf("%f", &b);
    scanf("%f", &c);
    /*assert(a>0 &&"positive integers only");
    assert(b>0 && "positive integers only");
    assert(c>0 && "positive integers only");*/
    while(a>0)
    {

    tri_angle(a,b,c);
    tri_type(a,b,c);
    printf("more nums\n");
    scanf("%f", &a);
    scanf("%f", &b);
    scanf("%f", &c);
    }
    printf("negative number found- loop end\n");
}

float maxtwo(float a, float b)
{
    return (a>b) ? a:b;
}

float maxthree(float a, float b,float c)
{
    return maxtwo(maxtwo(a,b),c);
}

bool compare_floats_equality(float a, float b)
{
   if (fabs(a-b)<EPSILON)
   {
      return true;
   }
   else
   {
      return false;
   }
}

void tri_angle(length a, length b, length c)
{
    length max,x,y;
    max = maxthree(a,b,c);
    /*x =(a!=max) ? a : (b!=max) ? b: c;*/
    x =(!compare_floats_equality(a,max)) ? a : (!compare_floats_equality(b,max)) ? b: c;
    /*y = (b!=max) ? b: (c!=max) ? c : a;*/
    y = (!compare_floats_equality(b,max)) ? b: (!compare_floats_equality(c,max)) ? c : a;
    printf("max :%f  others :%f %f\n", max , x ,y);
    if(max==sqrt(x*x +y*y))
    {
        printf("it is right angle\n");
    }
    else
    {
      printf("not right angle\n");
   }
}


void tri_type(length a, length b, length c)
{
    if(compare_floats_equality(a,b) && compare_floats_equality(c,b))
    {
        printf("equilateral tri\n");
    }
    else
    {
        if(compare_floats_equality(a,b)|| compare_floats_equality(b,c)||compare_floats_equality(c,a))
        {
            printf("isosceles tri\n");
        }
        else
        {
            printf("scalene tri \n");
        }
    }
}

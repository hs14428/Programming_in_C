#include <stdio.h>
#include <assert.h>

#define FEVER_THRESHOLD 37.5

enum scale {Celsius, Farenheit};
typedef enum scale scale;
enum bool {false, true};
typedef enum bool bool;
typedef double temperature;

int fvr(temperature t, scale s);
temperature Farenheit2Celsius(temperature t);

int main(void)
{
   assert(fvr(37.5, Celsius  ) == true );
   assert(fvr(36.5, Celsius  ) == false);
   assert(fvr(96.5, Farenheit) == true );
   assert(fvr(99.5, Farenheit) == false);

   return 0;
}

temperature Farenheit2Celsius(temperature t)
{
   return ((t-32.0)*5.0/9.0);
}

bool fvr(temperature t, scale s)
{

   if(s == 1) {
      t = Farenheit2Celsius(t);
   }
   if(t >= FEVER_THRESHOLD) {
      return true;
   }
   return false;
}

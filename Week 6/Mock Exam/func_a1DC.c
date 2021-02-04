#include "common.h"
/*
   Returns a Boolean, depending upon whether the
   string s1 is composed entirely from characters that
   occur in s2 or not.
*/



bool struses(const char* s1, const char* s2)
{
   int size_s1,size_s2,i,j;
   bool state;
   size_s1=strlen(s1);
   size_s2=strlen(s2);
   for(i=0;i<size_s1;i++)
   {
      state=false;
      for(j=0;j<size_s2;j++)
      {
         if(s1[i]==s2[j])
         {
            state=true;
         }
      }
      if(state==false)
      {
         return false;
      }
   }
   return true;
}

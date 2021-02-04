#include "common.h"

/*
   Return the count of numbers in the array l (of size n)
   which are unique (i.e. duplicates are not counted)
*/




int numdiff(const int l[], const int n)
{
   int i,j,current;
   bool state;
   int* arr;
   int size;
   arr=malloc(sizeof(int)*n);
   if(l==NULL)
   {
      free(arr);
      return 0;
   }

   arr[0]=l[0];
   size=1;

   for(i=0;i<n;i++)
   {
      current=l[i];

      state=true;
      for(j=0;j<size+1;j++)
      {
         if(current==arr[j])
         {
            state=false;
         }
      }
      if(state==true)
      {
         arr[size]=current;
         size+=1;
      }

   }
   free(arr);
   return size;
}

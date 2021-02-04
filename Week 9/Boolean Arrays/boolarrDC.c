#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "general.h"
#include "boolarr.h"

#define BITSINBYTE 8

#define INITCAP 30
#define SCALEFACTOR 2

void printasbool(unsigned char i);
unsigned char _bits_to_byte(int position,const char* str);
unsigned int _ones_in_single(const unsigned char a);
bool _add_empty(boolarr* ba);
void _shuffle(boolarr* ba);
unsigned int _n_bit_change(unsigned char ba,unsigned char bit_on,const bool b);
unsigned int _n_bit_false(unsigned char ba,unsigned char bit_on);
bool _boolarr_issame_helper(const boolarr* bigger, const boolarr* smaller);
bool _check_blanks(const boolarr* b1,const boolarr* b2);

int _byte_to_str(unsigned char i,int start,char* str);
void test(void);



/*int main(void)
{

   test();
   return 0;
}

void test(void)
{
   unsigned int i;
   boolarr* test_bool, *test_bool2,*test_bool3;
   bool b;
   char str[1000];

   test_bool=boolarr_init();
   boolarr_set(test_bool, 5, true);
   boolarr_set(test_bool, 0, true);
   boolarr_tostring(test_bool,str);
   printf("%s\n",str);
   assert(boolarr_set(test_bool, 127, true));
   assert(boolarr_set(test_bool, 128, true));
   assert(boolarr_set(test_bool, 191, true));
   assert(boolarr_set(test_bool, 194, true));
   boolarr_tostring(test_bool,str);
   printf("%s\n",str);

   test_bool=boolarr_initstr("11100001100000");
   for(i=0;i<test_bool->size;i++)
   {
      printasbool(test_bool->arr[i]);
   }

   printf("\n");
   printf("size in bits= %d\n",boolarr_size(test_bool));
   printf("num of 1s= %d\n",boolarr_count1s(test_bool));


   test_bool2=boolarr_clone(test_bool);
   for(i=0;i<test_bool2->size;i++)
   {
      printasbool(test_bool2->arr[i]);
   }
   printf("\n");

   printf("\n");
   boolarr_set(test_bool,4,true);

   printf("\n");
   boolarr_set(test_bool,1,true);

   boolarr_set(test_bool,1,false);

   printf("\n");
   boolarr_get(test_bool,1,&b);
   printf("bool=%d\n",b);
   boolarr_get(test_bool,4,&b);
   printf("bool=%d\n",b);

   for(i=0;i<test_bool->size;i++)
   {
      printasbool(test_bool->arr[i]);
   }
   printf("\n");
   test_bool2=boolarr_clone(test_bool);
   boolarr_set(test_bool2,30,false);
   for(i=0;i<test_bool2->size;i++)
   {
      printasbool(test_bool2->arr[i]);
   }
   printf("\n");
   printf("true=%d\n",boolarr_issame(test_bool,test_bool2));
   boolarr_set(test_bool2,30,true);
   printf("false=%d\n",boolarr_issame(test_bool,test_bool2));
   boolarr_set(test_bool2,30,false);
   boolarr_set(test_bool2,1,true);
   printf("false=%d\n",boolarr_issame(test_bool,test_bool2));
   test_bool2=boolarr_clone(test_bool);
   printf("true=%d\n",boolarr_issame(test_bool,test_bool2));
   boolarr_set(test_bool2,1,true);
   printf("false=%d\n",boolarr_issame(test_bool,test_bool2));
   for(i=0;i<test_bool2->size;i++)
   {
      printasbool(test_bool2->arr[i]);
   }
   boolarr_tostring(test_bool2,str);
   printf("\n%s\n",str);
   boolarr_print(test_bool2);

   test_bool=boolarr_initstr("1100");
   test_bool2=boolarr_initstr("1011");
   test_bool3=boolarr_bitwise(test_bool,test_bool2,or);
   boolarr_print(test_bool3);
   test_bool3=boolarr_bitwise(test_bool,test_bool2,and);
   boolarr_print(test_bool3);
   test_bool3=boolarr_bitwise(test_bool,test_bool2,xor);
   boolarr_print(test_bool3);
   boolarr_free(test_bool3);
}*/

boolarr* boolarr_init(void)
{
   boolarr* n_bool;
   n_bool=ncalloc(1, sizeof(boolarr));
   n_bool->arr=ncalloc(INITCAP, sizeof(byte));
   n_bool->size=0;
   n_bool->capacity=INITCAP;
   n_bool->parent_size=0;
   n_bool->bitwise=false;

   return n_bool;
}

/* Return a deep copy */
boolarr* boolarr_clone(const boolarr* ba)
{
   if(ba)
   {
      boolarr* n_bool;
      n_bool = boolarr_init();
      n_bool->size =ba->size;
      n_bool->capacity=ba->capacity;
      n_bool->arr= ncalloc(n_bool->capacity,sizeof(byte));
      memcpy(n_bool->arr, ba->arr,sizeof(byte)*ba->capacity);
      n_bool->bitwise=ba->bitwise;
      n_bool->parent_size=ba->parent_size;
      return n_bool;
   }
   return NULL;
}


/* Create boolarr based on string e.g. "1100001"
                           same as     "0110 0001"*/
                           /*"00000001 11000011"*/
boolarr* boolarr_initstr(const char* str)
{
   boolarr* n_bool;
   int start_point;
   int i;
   start_point = (strlen(str)%BITSINBYTE)-1;
   n_bool=boolarr_init();
   n_bool->parent_size= (unsigned int) strlen(str);
   i=0;
   for(;start_point<=(int)strlen(str);start_point+=BITSINBYTE)
   {
      n_bool->arr[i]=_bits_to_byte(start_point,str);
      i++;
      n_bool->size++;
   }

   return n_bool;
}

   /*100000000*/
unsigned char _bits_to_byte(int position,const char* str)
{
   int i;
   int converter;
   int num;
   converter=1;
   num=0;
   for(i=position;(i>position-BITSINBYTE)&&(i>=0) ;i--)
   {
      num+= (str[i]-48)*converter;
      converter=converter*2;
   }
   return (unsigned char) num;
}

/* Get number of bits in array */
unsigned int boolarr_size(const boolarr* ba)
{
   if(ba)
   {
      return ba->size*BITSINBYTE;
   }
   return 0;
}

/* Return number of bits that are set true */
unsigned int boolarr_count1s(const boolarr* ba)
{
   unsigned int count;
   unsigned int i;
   count=0;
   if(ba)
   {
      for(i=0;i<ba->size;i++)
      {
         count+=_ones_in_single(ba->arr[i]);
      }
      return count;
   }
   return 0;
}

unsigned int _ones_in_single(const unsigned char a)
{
   unsigned int count;
   unsigned char mask;
   mask=128;
   count=0;
   while(mask)
   {
      if(a& mask)
      {
         count++;
      }
      mask=mask>>1;
   }
   return count;
}


void printasbool(unsigned char i)
{
   unsigned char mask;
   mask=128;
   while(mask)
   {
      if(i& mask)
      {
         printf("1");
      }
      else
      {
         printf("0");
      }
      /*take all bits and shuffle on right*/
      mask=mask>>1;
   }

}




bool boolarr_set(boolarr* ba, const unsigned int n, const bool b)
{
   int byte_pos;
   unsigned char bit_on;
   int remainder;
   int i,zeros_to_add;

   if(ba)
   {
      zeros_to_add= ((n/BITSINBYTE)-ba->size)+1;
      for(i=0;i<zeros_to_add;i++)
      {

         _add_empty(ba);

      }

      byte_pos = (ba->size - (n/BITSINBYTE))-1 ;
      remainder= n%BITSINBYTE;
      bit_on=1;
      for(i=0;i<remainder;i++)
      {
         bit_on=bit_on*2;
      }
      ba->arr[byte_pos]=_n_bit_change(ba->arr[byte_pos],bit_on,b);
      return true;
   }
   return false;
}


bool boolarr_get(const boolarr* ba, const unsigned int n, bool* b)
{
   int byte_pos;
   unsigned char bit_on;
   int remainder;
   int i;
   unsigned char comparison;
   if(ba)
   {
      byte_pos = (ba->size-(n/BITSINBYTE))-1;
      if(byte_pos<0)
      {
         *b =false;
         return true;
      }
      remainder= n%BITSINBYTE;
      bit_on=1;
      for(i=0;i<remainder;i++)
      {
         bit_on=bit_on*2;
      }
      comparison=_n_bit_false(ba->arr[byte_pos],bit_on);
      *b= (!(ba->arr[byte_pos]==comparison));
      return true;
   }
   return false;
}



bool _add_empty(boolarr* ba)
{
   if(ba)
   {
      _shuffle(ba);
      ba->arr[0]=(unsigned char)0;

      ba->size++;
      if(ba->size==ba->capacity)
      {
         /*void* nrecalloc(void* p, int oldbytes, int newbytes);*/
         ba=nrecalloc(ba,ba->capacity*sizeof(byte),\
         ba->capacity*sizeof(byte)*SCALEFACTOR);

         ba->capacity= ba->capacity*SCALEFACTOR;
      }
      return true;
   }
   return false;
}

void _shuffle(boolarr* ba)
{
   int i;

   for(i=ba->size;i>0;i--)
   {
      ba->arr[i]=ba->arr[i-1];
   }

}

unsigned int _n_bit_change(unsigned char ba,unsigned char bit_on,const bool b)
{
   if(b)
   {
      return ba|bit_on;
   }
   else
   {
      return _n_bit_false(ba,bit_on);
   }
}


unsigned int _n_bit_false(unsigned char ba,unsigned char bit_on)
{
   unsigned char all_on;

   all_on= 255;
   all_on=all_on - bit_on;
   return ba & all_on;
}


/* Return if two arrays are the same (bitwise) */
bool boolarr_issame(const boolarr* b1, const boolarr* b2)
{
   if(b1&&b2)
   {
      if(b1->size>b2->size)
      {
         return _boolarr_issame_helper(b1,b2);
      }
      else
      {
         /*if same size shouldnt matter*/
         return _boolarr_issame_helper(b2,b1);
      }
   }
   if(b1==NULL&&b2==NULL)
   {
      return true;
   }
   return false;
}

bool _boolarr_issame_helper(const boolarr* bigger, const boolarr* smaller)
{
   unsigned int i,j;
   if(!_check_blanks(bigger,smaller))
   {
      return false;
   }
   i=bigger->size-smaller->size;


   for(j=0;j<smaller->size;j++)
   {
      if(bigger->arr[i]!=smaller->arr[j])
      {
         return false;
      }
      i++;
   }
   return true;
}


bool _check_blanks(const boolarr* bigger,const boolarr* smaller)
{
   int i;
   int pos;

   pos=bigger->size-smaller->size;
   for(i=0;i<pos;i++)
   {
      if(bigger->arr[i]!=(unsigned char) 0)
      {
         return false;
      }
   }
   return true;
}


bool boolarr_tostring(const boolarr* ba, char* str)
{
   unsigned int j,i;
   j=0;
   if(ba)

   {
      if(str)
      {
         for(i=0;i<ba->size;i++)
         {

            j=_byte_to_str(ba->arr[i],j,str);
         }

         str[j]='\0';
         if(!ba->bitwise)
         {
            while(str[0]=='0')
            {

               memmove(str, str+1, strlen(str));
            }
         }
         else
         {
            while((unsigned int)strlen(str)>ba->parent_size)
            {
               memmove(str, str+1, strlen(str));
            }
         }

         return true;
      }

   }
   return false;
}


int _byte_to_str(unsigned char i,int start,char* str)
{
   unsigned char mask;

   int j;
   j=start;
   mask=128;

   while(mask)
   {
      if(i& mask)
      {
         str[j]='1';
      }
      else
      {
         str[j]='0';
      }

      mask=mask>>1;
      j++;
   }
   return j;
}



bool boolarr_print(const boolarr* ba)
{
   char str[1000];
   if(ba)
   {
      boolarr_tostring(ba,str);
      printf("%s\n",str);
      printf("size=%d\ncapacity=%d\n",ba->size,ba->capacity);
      return true;
   }
   return false;
}

/* Flip all bits */
bool boolarr_negate(boolarr* ba)
{
   unsigned int i;
   if(ba)
   {
      for(i=0;i<ba->size;i++)
      {
         ba->arr[i]= ~ba->arr[i];
      }
      return true;
   }
   return false;
}

/* Functions dealing with 2 bitwise-arrays */
/* Must be the same length */
boolarr* boolarr_bitwise(const boolarr* ba1, const boolarr* ba2, const logicalop l)
{
   boolarr* bit_boolarr;
   unsigned int i;
   bit_boolarr=boolarr_init();
   bit_boolarr->bitwise=true;
   bit_boolarr->parent_size=ba1->parent_size;
   while(bit_boolarr->size < ba1->size)
   {
      _add_empty(bit_boolarr);
   }
   if(l==xor)
   {
      for(i=0;i<ba1->size;i++)
      {
         bit_boolarr->arr[i]=ba1->arr[i]^ba2->arr[i];
      }
   }
   if(l==and)
   {
      for(i=0;i<ba1->size;i++)
      {
         bit_boolarr->arr[i]=ba1->arr[i]&ba2->arr[i];
      }
   }
   if(l==or)
   {
      for(i=0;i<ba1->size;i++)
      {
         bit_boolarr->arr[i]= ba1->arr[i] | ba2->arr[i];
      }
   }
   return bit_boolarr;
}


/* Clears all space */
bool boolarr_free(boolarr* p)
{
   free(p->arr);
   free(p);
   return true;
}

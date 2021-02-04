#include <stdio.h>
#include<stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define UPPER 35
#define LOWER 0
typedef int bet;
typedef int money;
typedef enum preference {odds, number_bet} preference;

int random_gen(void);
money better(int choice, int answer,int random);

int main(void)
{
   int pref,ans,rand,s,ch;
   money total
/*
   assert(random_gen()<36);
   assert(random_gen()>-1);
   assert(better(0,1,1)==3);
   assert(better(0,1,4)==0);
   assert(better(1,1,1)==36);
   assert(better(1,2,3)==0);
   printf("asserts done\n");
*/
   total=10;

   while(total>0)
   {

   total+=better(pref,ans,rand)-1;
   printf("total = %d\n", total);
   //the second while removes white space from input
   while((ch = getchar()) != '\n'|| !=EOF);
   }
   return 0;
}

int random_gen(void)
{
   srand(time(NULL));
   return rand() % UPPER + LOWER;
}

money better(int choice, int answer,int random)
{
   int rand;
   preference pref;
   bet ans;
   printf("enter 0 for odds bet or 1 for number bet-\n");
   scanf("%d\n", &pref);
   printf("enter your number for betting\n");
   scanf("%d\n", &ans);
   rand= random_gen();
   switch(pref)
   {
      case odds:

      if((ans%2==0 && rand %2==0)||(ans%2!=0 && rand%2!=0))
      {
         //2 to 1 bets and 1 pound bets
         printf("you won\n");
         return 3;
      }
      else
      {
         printf("you lost\n");
         return 0;
      }
      break;
      case number_bet:
      if(ans==rand)
      {
         printf("you won\n");
         return 36;
      }
      else
      {
         printf("you lost\n");
         return 0;
      }
      break;
      default:
      printf("error\n");
      exit(1);
   }
}

/*Roulette machine */
/*09/10/20        */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define MAXNUM 35
#define MINNUM 0
#define INITCASH 50

typedef int money;
enum btype {odd_even, number};
typedef enum btype bettype;

int whichbet(int b);
int spinwheel(void);
int stakeamount(int s);
int stakecheck(int s, int p);
int input(int n);
void test(void);

int main(void)
{
   /*test();*/

   bettype bet;
   money pot = INITCASH;
   money stake;
   money returns;
   int playerinput, result;

   printf("Welcome to the Casino!\n\n");

   while (pot > 0) {

      int b, n;
      money s, stake;
      bet = whichbet(b);
      stake = stakeamount(s);
      stakecheck(stake, pot);

      playerinput = input(n);
      result = spinwheel();

      switch(bet) {
         case odd_even:
            if ((playerinput%2 == 0) && (result%2 == 0) || (playerinput%2 !=0) && (result%2 != 0)) {
               printf("The ball landed on: %d\n\n", result);
               printf("Congrats you won. Spin again.\n\n");
               returns = (2 * stake);
               printf("Your winnings are: %d.\n\n", returns);
               pot = (pot - stake + returns);
            }
            else {
               printf("The ball landed on: %d\n\n", result);
               printf("Sorry, you lost this time. Spin again if you have money.\n\n");
               pot = (pot - stake);
            }
            break;
         case number:
            if (playerinput == result) {
               printf("The ball landed on: %d\n\n", result);
               printf("Congrats you won. Spin again.\n\n");
               returns = (35 * stake);
               printf("Your winnings are: %d.\n\n", returns);
               pot = (pot - stake + returns);
            }
            else {
               printf("The ball landed on: %d\n\n", result);
               printf("Sorry, you lost this time. Spin again if you have money.\n\n");
               pot = (pot - stake);
            }
            break;
         default:
            printf("Game is broken.\n\n");
            exit(1);
      }
      printf("Your balance is: %d\n\n", pot);
   }

   printf("Game over, you lose!\n\n");
   return 0;
}

int stakeamount(int s)
{
   printf("Please enter your stake amount.\n\n");
   if (scanf("%d", &s) != 1) {
      printf("Error: Please enter only 1 number.\n\n");
   }

   return s;
}

int stakecheck(int s, int p)
{
   money stake, pot;

   if (stake > pot) {
      printf("You don't have enough cash. Please enter a lower number.\n\n");
      stake = stakeamount(s);
   }
   else {
      return 0;
   }
}

int whichbet(int b)
{

   printf("Please choose your bet type - Enter 0 for an Odd or Even bet, and 1 for a number bet.\n\n");

   if (scanf("%d", &b) != 1) {
      printf("Error: Please enter only 1 number.\n");
   }

   assert(b <= 1 && b >=0);

   if (b == 0) {
      return odd_even;
   }
   else {
      return number;
   }
}

int input(int n)
{
   printf("Please enter your number for betting.\n\n");
   scanf("%d", &n);
   return n;
}

int spinwheel(void)
{
   return rand()%(MAXNUM + 1);
}

void test(void)
{
   int i, n;
   for(i = 0; i <= 1000000; i++) {
      n = spinwheel();
      assert(n >= MINNUM && n <= MAXNUM);
   }
}

/*int oddoreven(int o)
{
   if (o=2||o=4||o=6||o=8||o=10||o=12||o=14||o=16||o=18||o=20||o=22||o=24||o=26||o=28||o=30||o=32||o=34||) {
      return

}*/

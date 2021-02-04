#include <stdio.h>
#include <math.h>
#include <assert.h>

#define GAP 'a'-1
#define NOTAVOWEL 20
#define ISAVOWEL 1
#define NOTAPRIME 6
#define ISAPRIME 3

typedef enum vowel{a=1,e=5,i=9,o=15,u=21} vowel;
typedef enum boolean{False,True} boolean;

int isPrime(int n);
int isVowel(int n);
void test(void);

int main(void)
{

  int i,j,cnt;

  test();

  cnt = 0;
  for(i=1;i<=26;i++){
    for(j=1;j<=26;j++){
      if((isPrime(2*i+j)&&isVowel(i)&&!isVowel(j))
      ||(isPrime(2*i+j)&&!isVowel(i)&&isVowel(j))){
        cnt++;
        printf("%2d  %c%c%c\n",cnt,i+GAP,j+GAP,i+GAP);
      }
    }
  }
  return 0;
}

int isPrime(int n)
{
  int i;
  if(n==1)
  return False;
  for(i=2;i<=sqrt(n);i++){
    if(n%i==0){
    return False;
    }
  }
  return True;
}

int isVowel(int n)
{
  if(n==a||n==e||n==i||n==o||n==u){
    return True;
  }
  else
  {
  return False;
  }
}

void test(void)
{
  assert(isVowel(NOTAVOWEL) == False);
  assert(isVowel(ISAVOWEL) == True);
  assert(isPrime(NOTAPRIME) == False);
  assert(isPrime(ISAPRIME) == True);
}

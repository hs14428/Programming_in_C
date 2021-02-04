#include <stdio.h>
#include <assert.h>

typedef enum bool {false, true} bool;
typedef char letter;

int letterValue(letter c);
int nameSum(letter c1, letter c2);
bool isVowel(letter c);
bool isPrime(int n);
bool isValid(letter c1, letter c2);
void test();

int main(void){
  letter ch1, ch2;
  int counter = 0;
  for (ch1 = 'a'; ch1 <= 'z'; ++ch1){
    for (ch2 = 'a'; ch2 <= 'z'; ++ch2){
      if (isValid(ch1, ch2) == true){
        counter++;
        printf("%2d %c%c%c\n", counter, ch1, ch2, ch1);
      }
    }
  }
  test();

  return 0;
}

/*Runs asserts on functions for testing*/
void test(){
  assert(isVowel('e') == true);
  assert(isVowel('f') == false);
  assert(isPrime(5) == true);
  assert(isPrime(10) == false);
  assert(isPrime(1) == false);
  assert(isPrime(2) == true);
  assert(isValid('a', 'b') == false);
  assert(isValid('b', 'o') == true);
}

/*Returns the number equivalent of input character*/
int letterValue(letter c){
  return (c - 'a') + 1;
}

/*Returns numerical total of a name*/
int nameSum(letter c1, letter c2){
  return (letterValue(c1) * 2) + letterValue(c2);
}

/*Checks if person with name can live on planet Bob*/
bool isValid(letter c1, letter c2){
  int letterTotal;
  if (isVowel(c1) == isVowel(c2)){
    return false;
  }
  letterTotal = nameSum(c1, c2);
  if (isPrime(letterTotal) == false){
    return false;
  }
  return true;
}

/*Checks if a character is a vowel*/
bool isVowel(letter c){
  if ((c == 'a') || (c == 'e') || (c == 'i') || (c == 'o') || (c == 'u')){
  return true;
  }
  return false;
}

/*Checks if an integer is a prime number*/
bool isPrime(int n){
  int i;
  /*By normal definition of prime, the number
    must be greater than 1.*/
  if (n <= 1) {
    return false;
  }

  for (i = 2; i <= n / 2; ++i){
   if (n % i == 0){
    return false;
    }
  }
  return true;
}

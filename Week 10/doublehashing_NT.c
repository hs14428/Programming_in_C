#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#define INITSIZE 11
#define MAXWORD 10

struct input{
  char word[MAXWORD];
  int index;
  int used;
};

typedef struct input input;

int hash_loc(char *word);
void test(void);
void print_list(input *list);
void insert_word(input* list, char* word);
int step(char *word);
int search_word(input* list, char *word, int index);
int equal_str(char *w1, char *w2);


int main(void)
{
  test();
  input *list = calloc(1, sizeof(input)*INITSIZE);
  FILE *fp;
  char str[MAXWORD];
  char testw[MAXWORD] = "PHL";
  fp = fopen("words.txt","r");
  int searchcnt = 0, totwords = 0;

  while(fgets(str, MAXWORD, fp)!=0){
    insert_word(list, str);
  }
  print_list(list);
  fp = fopen("wordstosearch.txt","r");
  while(fgets(str, MAXWORD, fp)!=0){
    searchcnt += search_word(list, str, hash_loc(str));
    totwords += 1;
  }
  printf("\\\ Search count = %d \n",searchcnt);
  printf("\\\ Number of words = %d \n",totwords);
  printf("\\\ Average Search = %d \n\n",searchcnt/totwords);
}

int search_word(input* list, char *word, int index)
{
   int cnt = 1;

   if(equal_str(list[index].word,word)){
      return cnt;
   }
   else{
      while(equal_str(list[index].word,word)==0){
            if(index>0){
              index = (index-step(word)+INITSIZE)%INITSIZE;
            }
            else{
              index = INITSIZE - 1;
            }
            cnt++;
            if(cnt>10){
               break;
            }
      }
   }
   return cnt;
}

int equal_str(char *w1, char *w2)
{
   int i, flag = 0;
   for(i = 0; i < 3; i++){
      if(w1[i]!=w2[i]){
         flag = 1;
      }
   }
   if(flag==1){
      return 0;
   }
   return 1;
}

void print_list(input *list)
{
  int i;
  for(i = 0; i < INITSIZE; i++){
    printf("%d: %s\n",i+1, list[i].word);
  }
}

void insert_word(input* list, char* word)
{
  int index, st = 0, s = 0;
  index = hash_loc(word);
  while(list[index].used!=0){
    st = step(word);
    if(index>0) {
      index = (index-st+INITSIZE)%INITSIZE;
    }
    else {
      index = INITSIZE - 1;
    }
  }
  strcpy(list[index].word, word);
  list[index].used = 1;
}

int hash_loc(char *word)
{
   int size = (int)strlen(word);
   int i, j = 2, stepsize = 0, wordnum = 0;
   char c;

   for(i = 0; i < size; i++){
      c = tolower(word[i]);
      wordnum = (int) c - 97;
      stepsize += wordnum*pow(26, j);
      j--;
   }
   stepsize = stepsize%INITSIZE;
   return stepsize;
}

int step(char *word)
{
  int size = (int)strlen(word);
  int i, j = 2, stepsize = 0, wordnum = 0;
  char c;

  for(i = 0; i < size; i++){
    c = tolower(word[i]);
    wordnum = (int) c - 97;
    stepsize += wordnum*pow(26, j);
    j--;
  }
  stepsize = stepsize/INITSIZE;
  stepsize = stepsize%INITSIZE;
  if(stepsize<1){
    stepsize = 1;
  }
  return stepsize;
}

void test(void)
{
  char testw1[MAXWORD] = "DCA";
  char testw2[MAXWORD] = "HKA";
  char testw3[MAXWORD] = "PHL";
  char testw4[MAXWORD] = "HKG";

  assert(hash_loc(testw1)==1);
  assert(hash_loc(testw2)==9);
  assert(hash_loc(testw3)==4);

  assert(step(testw3)==4);
  assert(step(testw4)==3);
}

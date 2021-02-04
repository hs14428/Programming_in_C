#include <stdio.h>

int _hash_num(void* key);
int _hash_num_step(void* key);

int main(void)
{
   int step, hash;
   void* key;
   key = (void*)1918;

   hash = _hash_num(&key);
   printf("hash: %d\n", hash);
   step = _hash_num_step(&key);
   printf("step: %d\n", step);
}

int _hash_num(void* key)
{
   int c;
   unsigned long hash = 5381;
   int size = 11;
   char* str;
   str = (char*)key;

   while ((c = (*str++))) {
      hash = 33*hash^c;
   }
   return (int) (hash%size);
}

int _hash_num_step(void* key)
{
   int c;
   unsigned long hash = 5381;
   int size = 11;
   char* str;
   str = (char*)key;

   while ((c = (*str++))) {
      hash = 33*hash^c;
   }
   hash = ((hash/size)%size);
   if (hash < 1) {
      return 1;
   }
   else {
      return (int) hash;
   }
}

#include "specific.h"
#include "../assoc.h"


bool _resize_check(assoc* a);
void _resize_arr(assoc* a);
void _point_hash_pairs(assoc* a, void* key, void* data, int hash);
int _hash_str(assoc* a, char *str);
int _hash_str_step(assoc* a, char *str);
int _hash_str_probe(assoc* a, char *str, int hash);
int _hash_int(assoc* a, int n);
int _hash_int_step(assoc* a, int n);
int _hash_int_probe(assoc* a, int n, int hash);
int _wrap_around(assoc* a, int hash, int step);
int findPrime(assoc* a, int n);
void test();

/*
   Initialise the Associative array
   keysize : number of bytes (or 0 => string)
   This is important when comparing keys since
   we'll need to use either memcmp() or strcmp()
*/
assoc* assoc_init(int keysize)
{
   assoc* a;
   a = (assoc*) ncalloc(1, sizeof(assoc));
   a->size = 0;
   a->keysize = keysize;
   a->capacity = INITSIZE;
   a->arr = (hash*) ncalloc(a->capacity, sizeof(hash));
   return a;
}

/* WIP - Need to test */
/* Resize the hash table if it gets close to capacity */
/* will need to resize to a new prime number */
void _resize_arr(assoc* a)
{
   int filled;
   int newCap;
   if (a) {
      /* Might need to change the scaleup so that it scales new capacity to a new prime etc*/
      a->capacity = a->capacity*SCALEFACTOR;
      newCap = findPrime(a, a->capacity);
      a->arr = (hash*) nrecalloc(a->arr, a->capacity*sizeof(hash), newCap*sizeof(hash));
      a->capacity = newCap;
   }
}

/* Need to test */
/* Returns true if array needs to be resized */
bool _resize_check(assoc* a)
{
   int filled;
/* Add one at the start to check if when you insert new key it
   will cause the assoc to need to be resized. Undo this later
   so that when actually adding the key you add 1 to size  */
   if (a) {
      a->size = a->size + 1;
      filled = ((a->size/a->capacity)+0.5)*100;
      if (filled > CAPCHECK) {
         a->size = a->size - 1;
         return true;
      }
      a->size = a->size - 1;
      return false;
   }
   fprintf(stderr, "Associative Array is NULL.\n");
   return false;
}

/* Finds the the first prime less than the associative arrays
   Capacity*Scalefactor for resizing array when array overflows.
   Based on the Sieve of Eratosthenes code from geekforgeeks */
int findPrime(assoc* a, int n)
{
   int i, p;
   bool *prime = malloc(a->capacity*sizeof(bool)+1);

   for (i = 0; i <= n; i++) {
      prime[i] = true;
   }
   for (p = 2; p*p <= n; p++) {
      if (prime[p] == true) {
         for (i = p*p; i<=n; i +=p) {
            prime[i] = false;
         }
      }
   }
   for (p = n; p >= 0; p--) {
      if (prime[p]) {
         return p;
      }
   }
   return 0;
}

/* Need to test */
/*
   Insert key/data pair
   - may cause resize, therefore 'a' might
   be changed due to a realloc() etc.
*/
void assoc_insert(assoc** a, void* key, void* data)
{
   int hash, step, newHash;
   char* str;

   assoc* aptr = *a;
/*
   if (_resize_check(a)) {
      _resize_arr(a);
      /* if resize is required, rehash also required */
/*   }*/

   if (aptr->keysize == 0) {
      /* Hash for strings - split into _insert_str?? */
      str = key;
      hash = _hash_str(aptr, str);
      if (aptr->arr[hash].key == NULL) {
         _point_hash_pairs(aptr, key, data, hash);
      }
      else {
         newHash = _hash_str_probe(aptr, str, hash);
         _point_hash_pairs(aptr, key, data, newHash);
      }
   }
   else {
      /* Hash for ints */

      hash = _hash_int(aptr, (long)key);
      if (aptr->arr[hash].key == NULL) {
         _point_hash_pairs(aptr, key, data, hash);
      /*   printf("in insert: %ld\n", *(long*)aptr->arr[hash].key);*/
      }
      else {
         newHash = _hash_int_probe(aptr, (long)key, hash);
         _point_hash_pairs(aptr, key, data, newHash);
      /*   printf("in insert: %ld\n", *(long*)aptr->arr[hash].key);*/
      }
   }
}

void _point_hash_pairs(assoc* a, void* key, void* data, int hash)
{
   a->arr[hash].key = key;
   a->arr[hash].data = data;
   a->size += 1;
/* bring the function out slowly through the other functions where this fn is called */
/*   printf("in point: %ld\n", *(long*)a->arr[hash].key);*/

}

/* String hash function from Princeton */
/* from https://algs4.cs.princeton.edu/34hash/#:~:text=The%20most%20commonly%20used%20method,between%200%20and%20M%2D1 */
int _hash_str(assoc* a, char *str)
{
   int i;
   int hash = 0;
   int len = strlen(str);

   for (i = 0; i < len; i++) {
      hash = ((PRIME*hash) + str[i]) % a->capacity;
   }
   return hash;
}

/* Double hashing to find probe decrement for str hashing */
/* Same as first hash but with smaller prime */
int _hash_str_step(assoc* a, char *str)
{
   int i;
   int step = 0;
   int len = strlen(str);

   for (i = 0; i < len; i++) {
      step = ((PRIME2*step) + str[i]) % a->capacity;
   }
   if (step == 0) {
      step = 1;
   }
   return step;
}

/* Loop through the hash table until a free space is found */
int _hash_str_probe(assoc* a, char *str, int hash)
{
   int step;

   step = _hash_str_step(a, str);
   while (a->arr[hash].key != NULL) {
      if (hash-step < 0) {
         hash = _wrap_around(a, hash, step);
      }
      else {
         hash = hash - step;
      }
   }
   return hash;
}

/* Modular hashing method for positive integers */
/* from https://algs4.cs.princeton.edu/34hash/#:~:text=The%20most%20commonly%20used%20method,between%200%20and%20M%2D1 */
int _hash_int(assoc* a, int n)
{
   int hash;
   int size = a->capacity;
   hash = n%size;
   return hash;
}

/* Double hashing to find the probe decrement for int hashing */
/* Hash function taken from https://www.cs.utexas.edu/~mitra/csSpring2017/cs313/lectures/hash.html */
int _hash_int_step(assoc* a, int n)
{
   int prime, step;
   int size = a->capacity;

   prime = findPrime(a, size/2);
   step = prime - (n%prime);
   return step;
}

/* Loop through the hash table until a free space is found */
int _hash_int_probe(assoc* a, int n, int hash)
{
   int step;

   step = _hash_int_step(a, n);
   while (a->arr[hash].key != NULL) {
      if (hash-step < 0) {
         hash = _wrap_around(a, hash, step);
      }
      else {
         hash = hash - step;
      }
   }
   return hash;
}

int _wrap_around(assoc* a, int hash, int step)
{
   int pos;
   pos = hash - step + a->capacity;
   pos %= a->capacity;
   return pos;
}

/*
   Returns the number of key/data pairs
   currently stored in the table
*/
unsigned int assoc_count(assoc* a)
{
   if (a == NULL) {
      return 0;
   }
   return a->size;
}

/*
   Returns a pointer to the data, given a key
   NULL => not found
*/
void* assoc_lookup(assoc* a, void* key)
{
   char* str;
   if (a == NULL) {
      return NULL;
   }
   if (a->keysize == 0) {
      str = key;
      hash = _hash_str(aptr, str);

   }
   else {

   }
   return NULL;
}

void assoc_todot(assoc* a)
{


}

/* Free up all allocated space from 'a' */
void assoc_free(assoc* a)
{
   if (a == NULL) {
      return;
   }
   free(a->arr);
   free(a);
}

/* Test functions private to this realloc.c file and further specific.h fn testing */
void test()
{
   int keysize;
   assoc* aa;

   aa = assoc_init(0);
   assert(aa->size     ==  0);
   assert(aa->keysize  ==  0);
   assert(aa->capacity == 17);
   assoc_free(aa);
   assert(aa->arr == NULL);

   keysize = sizeof(int);
   aa = assoc_init(keysize);
   assert(aa->size     ==  0);
   assert(aa->keysize  == keysize);
   assert(aa->capacity == 17);

   assert(assoc_count(aa) == 0);
   aa->size = 10;
   assert(assoc_count(aa) == 10);
   aa->size = 15;
   assert(assoc_count(aa)  == 15);
   assert(!(assoc_count(aa) == 10));
   assoc_free(aa);
   assert(!(assoc_count(aa) == 15));

   aa = assoc_init(0);
   aa->capacity = 17;
   aa->capacity *= SCALEFACTOR;
   assert(findPrime(aa, aa->capacity) == 31);
   assert(findPrime(aa, 30) == 29);
   assert(findPrime(aa, 20) == 19);

   aa->capacity = 11;
   assert(_hash_str(aa, "1234")   == 0);
   assert(_hash_str(aa, "12345")  == 9);
   assert(_hash_str(aa, "hello")  == 6);
   assert(_hash_str(aa, "world!") == 5);
   assert(_hash_str(aa, "Hope you're well Neill") == 6);

/* aa->capacity is 11 at the moment */
   assert(_hash_str_step(aa, "1234")   == 4);
   assert(_hash_str_step(aa, "12345")  == 1);
   assert(_hash_str_step(aa, "hello")  == 9);
   assert(_hash_str_step(aa, "world!") == 6);
   assert(_hash_str_step(aa, "Hope you're well Neill") == 9);

   assert(_hash_int(aa, 7)  == 7);
   assert(_hash_int(aa, 14) == 3);
   assert(_hash_int(aa, 0)  == 0);

   assert(_hash_int_step(aa,  7) == 3);
   assert(_hash_int_step(aa, 56) == 4);
   assert(_hash_int_step(aa, 11) != 0);

   assert(_wrap_around(aa,  6,  9) == 8);
   assert(_wrap_around(aa, 10,  1) == 9);
   assert(_wrap_around(aa,  1, 10) == 2);
   assert(_wrap_around(aa,  0,  5) == 6);

/* No collisions so should just return normal hash index */
   assert(_hash_str_probe(aa, "1234",  0) == 0);
   assert(_hash_str_probe(aa, "12345", 9) == 9);
/* Now there should be a clash so probe by stepping back */
   aa->arr[9].key = "1234";
   assert(_hash_str_probe(aa, "12345", 9) != 9);
   assert(_hash_str_probe(aa, "12345", 9) == 8);
/* Probe twice as there will be two collisions */
   aa->arr[6].key = "world!";
   aa->arr[8].key = "world!";
   assert(_hash_str_probe(aa, "hello", 6) != 8);
   assert(_hash_str_probe(aa, "hello", 6) == 10);

   _point_hash_pairs(aa, "hello", (void*)1, 8);
   assert(strcmp(aa->arr[8].key, "world!") != 0);
   assert(strcmp(aa->arr[8].key, "hello" ) == 0);
   assert((long)aa->arr[8].data != 0);
   assert((long)aa->arr[8].data == 1);

/* No collisions so should just return normal hash index */
   assert(_hash_int_probe(aa,  7, 7) == 7);
   assert(_hash_int_probe(aa, 14, 3) == 3);
/* Now there should be a clash so probe by stepping back */
   aa->arr[7].key = (void*)8;
   assert(_hash_int_probe(aa,  7, 7) != 7);
   assert(_hash_int_probe(aa,  7, 7) == 4);
/* Probe twice as there will be two collisions */
   aa->arr[4].key = (void*)8;
   assert(_hash_int_probe(aa,  7, 7) != 4);
   assert(_hash_int_probe(aa,  7, 7) == 1);

   assoc_free(aa);
}

/* Harrys Functions to be made
----------------------------------                         */

/* hash() - for initial hashing function - use lecture one */
/* doubleHash() - for collisions */
/* reSize() - to resize te hash table if full */
/* reHash() - to re-hash the table if resize required, call within resize? */

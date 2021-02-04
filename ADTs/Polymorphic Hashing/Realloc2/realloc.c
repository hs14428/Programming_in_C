#include "specific.h"
#include "../assoc.h"

assoc* _resize_arr(assoc* a);
assoc* _rehash(assoc* a, assoc* new_a);
bool _resize_check(assoc* a);
void _hash(assoc* a, void* key, void* data);
void _point_hash_pairs(assoc* a, void* key, void* data, int hash);
int _hash_str(assoc* a, char *str);
int _hash_str_step(assoc* a, char *str);
int _hash_str_run(assoc* a, char *str);
int _hash_num(assoc* a, void* key);
int _hash_num_step(assoc* a, void* key);
int _hash_num_run(assoc* a, void* key);
int _probe(assoc* a, int hash, int step);
int _lookup_str_probe(assoc* a, void* key);
int _lookup_num_probe(assoc* a, void* key);
int _wrap_around(assoc* a, int hash, int step);
int findPrime(int n);
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
   static bool tested = false;
   if (tested == false) {
      tested = true;
      test();
   }
   a = (assoc*) ncalloc(1, sizeof(assoc));
   a->size = 0;
   a->keysize = keysize;
   a->capacity = INITSIZE;
   a->arr = (hash*) ncalloc(a->capacity, sizeof(hash));
   return a;
}

/* Resize the hash table if it gets close to capacity */
assoc* _resize_arr(assoc* a)
{
   int newCap;
   assoc* new_a;

   new_a = (assoc*) ncalloc(1, sizeof(assoc));
   new_a->keysize = a->keysize;
   new_a->capacity = a->capacity*SCALEFACTOR;
   newCap = findPrime(new_a->capacity);
   new_a->capacity = newCap;
   new_a->arr = (hash*) ncalloc(new_a->capacity, sizeof(hash));
   new_a = _rehash(a, new_a);
   assoc_free(a);

   return new_a;
}

assoc* _rehash(assoc* a, assoc* new_a)
{
   int i;
   if (a && new_a) {
      for (i = 0; i < a->capacity; i++) {
         if (a->arr[i].key) {
            _hash(new_a, a->arr[i].key, a->arr[i].data);
         }
      }
      return new_a;
   }
   fprintf(stderr, "a or new_a are NULL");
   return NULL;
}

/* Returns true if array needs to be resized. Resize at >60% full */
bool _resize_check(assoc* a)
{
   int filled;
/* Add one at the start to check if when you insert new key it
   will cause the assoc to need to be resized. Undo this later
   so that when actually adding the key you add 1 to size  */
   if (a) {
      a->size = a->size + 1;
   /* round up the % filled */
      filled = (int)(((float)a->size/a->capacity+ROUNDUP)*100);
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
int findPrime(int n)
{
   int i, p;
   bool *prime = malloc((n+1)*sizeof(bool));

   for (i = 0; i <= n; i++) {
      prime[i] = true;
   }
   for (p = 2; p*p <= n; p++) {
      if (prime[p] == true) {
         for (i = p*p; i<=n; i += p) {
            prime[i] = false;
         }
      }
   }
   for (p = n; p >= 2; p--) {
      if (prime[p]) {
         free(prime);
         return p;
      }
   }
   free(prime);
   return 0;
}

/*
   Insert key/data pair
   - may cause resize, therefore 'a' might
   be changed due to a realloc() etc.
*/
void assoc_insert(assoc** a, void* key, void* data)
{
   assoc* aptr = *a;

   if (_resize_check(aptr)) {
      aptr = _resize_arr(aptr);
      *a = aptr;
   }
   _hash(aptr, key, data);
}

/* Master hash function that will point the key and data
   pointers to the correct hash location, accounting for dupes */
void _hash(assoc* a, void* key, void* data)
{
   int hash;
   if (a->keysize == 0) {
      /* Hash for strings */
      hash = _hash_str_run(a, (char*)key);
      if (hash != DUPE) {
         _point_hash_pairs(a, key, data, hash);
      }
   }
   else {
      /* Hash for numbers */
      hash = _hash_num_run(a, key);
      if (hash != DUPE) {
         _point_hash_pairs(a, key, data, hash);
      }
   }
}

void _point_hash_pairs(assoc* a, void* key, void* data, int hash)
{
   a->arr[hash].key = key;
   a->arr[hash].data = data;
   a->size += 1;
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

/* Double hashing to find probe decrement for str hashing
   Same as first hash but with smaller prime */
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

/* Loop through the hash table until a free space is found
   return a duplicate flag if duplicate KEY found */
int _hash_str_run(assoc* a, char *str)
{
   int hash, step, cmp;

   hash = _hash_str(a, str);
   if (a->arr[hash].key == NULL) {
      return hash;
   }
   else {
      step = _hash_str_step(a, str);
      while (a->arr[hash].key != NULL) {
         cmp = strcmp((char*)a->arr[hash].key, str);
         if (cmp == 0) {
            return DUPE;
         }
         hash = _probe(a, hash, step);
      }
      return hash;
   }
}

/* Hashing formula based off lecture notes hash function */
int _hash_num(assoc* a, void* key)
{
   int c;
   unsigned long hash = 5381;
   char* str;
   str = (char*)key;

   while ((c = (*str++))) {
      hash = 33*hash^c;
   }
   return (int) (hash%a->capacity);
}

/* Hashing formula based off lecture notes double hash function */
int _hash_num_step(assoc* a, void* key)
{
   int c;
   unsigned long hash = 5381;
   char* str;
   str = (char*)key;

   while ((c = (*str++))) {
      hash = 33*hash^c;
   }
   hash = ((hash/a->capacity)%a->capacity);
   if (hash < 1) {
      return 1;
   }
   else {
      return (int) hash;
   }
}

/* Function that returns the next free hash location after
   probing and checking for duplicates. Number specific. */
int _hash_num_run(assoc* a, void* key)
{
   int hash, step;

   hash = _hash_num(a, key);
   if (a->arr[hash].key == NULL) {
      return hash;
   }
   else {
      step = _hash_num_step(a, key);
      while (a->arr[hash].key != NULL) {
         if (strcmp(a->arr[hash].key, key) == 0) {
            return DUPE;
         }
         hash = _probe(a, hash, step);
      }
      return hash;
   }
}

/* Function that returns the hash location after 1 step */
int _probe(assoc* a, int hash, int step)
{
   if (hash-step < 0) {
      hash = _wrap_around(a, hash, step);
   }
   else {
      hash = hash - step;
   }
   return hash;
}

/* Function that will wrap around hash values when probing */
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
   int hash;

   if (a == NULL) {
      return NULL;
   }
   if (a->keysize == 0) {
      hash = _lookup_str_probe(a, (char*)key);
      if (hash == PROBEFAIL) {
         return NULL;
      }
      return a->arr[hash].data;
   }
   else {
      hash = _lookup_num_probe(a, key);
      if (hash == PROBEFAIL) {
         return NULL;
      }
      return a->arr[hash].data;
   }
}

/* Function that looks up a str key value in the array and returns the hash */
int _lookup_str_probe(assoc* a, void* key)
{
   int hash, step;

   hash = _hash_str(a, (char*)key);
   if (a->arr[hash].key == NULL) {
      return PROBEFAIL;
   }
   step = _hash_str_step(a, (char*)key);

   while (strcmp((char*)a->arr[hash].key, (char*)key) != 0) {
      hash = _probe(a, hash, step);
      if (a->arr[hash].key == NULL) {
         return PROBEFAIL;
      }
   }
   return hash;
}

/* Function that looks up a num key value in the array and returns the hash */
int _lookup_num_probe(assoc* a, void* key)
{
   int hash, step;

   hash = _hash_num(a, key);
   if (a->arr[hash].key == NULL) {
      return PROBEFAIL;
   }
   step = _hash_num_step(a, key);

   while (strcmp(a->arr[hash].key, (key)) != 0) {
      hash = _probe(a, hash, step);
      if (a->arr[hash].key == NULL) {
         return PROBEFAIL;
      }
   }
   return hash;
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
   void* key1;
   void* key2;
   void* key3;
   void* key4;
   void* key5;
   assoc* aa;

   aa = assoc_init(0);
   assert(aa->size     ==  0);
   assert(aa->keysize  ==  0);
   assert(aa->capacity == 17);
   assoc_free(aa);

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

   aa = assoc_init(0);
   aa->capacity = 17;
   aa->capacity *= SCALEFACTOR;
   assert(findPrime(aa->capacity) == 31);
   assert(findPrime(30) == 29);
   assert(findPrime(20) == 19);

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

   key1 = (void*)100;
   key2 = (void*)7389;
   key3 = (void*)1918;
   assert(_hash_num(aa, &key1) == 6);
   assert(_hash_num(aa, &key2) == 7);
   assert(_hash_num(aa, &key3) == 1);

   assert(_hash_num_step(aa, &key1) == 8);
   assert(_hash_num_step(aa, &key2) == 5);
   assert(_hash_num_step(aa, &key3) == 8);

   assert(_wrap_around(aa,  6,  9) == 8);
   assert(_wrap_around(aa, 10,  1) == 9);
   assert(_wrap_around(aa,  1, 10) == 2);
   assert(_wrap_around(aa,  0,  5) == 6);

   assert(_probe(aa, 8, 6) == 2);
   assert(_probe(aa, 8, 9) != 2);
   assert(_probe(aa, 8, 9) == 10);
   assert(_probe(aa, 10, 4) == 6);
   assert(_probe(aa, 6, 9) == 8);

/* No collisions so should just return normal hash index */
   assert(_hash_str_run(aa, "1234") == 0);
   assert(_hash_str_run(aa, "12345") == 9);
/* Now there should be a clash so probe by stepping back */
   aa->arr[9].key = "1234";
   assert(_hash_str_run(aa, "12345") != 9);
   assert(_hash_str_run(aa, "12345") == 8);
/* Probe twice as there will be two collisions */
   aa->arr[6].key = "test!";
   aa->arr[8].key = "world!";
   assert(_hash_str_run(aa, "hello") != 8);
   assert(_hash_str_run(aa, "hello") == 10);
/* Change the contents of [8] to "hello" so re-running the
   _hash_str_run fn will kick out a duplicate key flage (DUPE) */
   aa->arr[8].key = "hello";
   assert(_hash_str_run(aa, "hello") == DUPE);

   _point_hash_pairs(aa, "hello", (void*)1, 8);
   assert(strcmp(aa->arr[8].key, "world!") != 0);
   assert(strcmp(aa->arr[8].key, "hello" ) == 0);
   assert((long)aa->arr[8].data != 0);
   assert((long)aa->arr[8].data == 1);

/* hello is in cell [8] after testing of _hash_str_run _and
   _point_hash_pairs tests */
   assert(_lookup_str_probe(aa, "hello") == 8);
   assert(_lookup_str_probe(aa, "selodnaraf") == PROBEFAIL);
/* Add selodnaraf to the array and prompt some probing to find hello */
   aa->arr[8].key = "selodnaraf";
   aa->arr[10].key = "hello";
   assert(_lookup_str_probe(aa, "hello") != 8);
   assert(_lookup_str_probe(aa, "hello") == 10);

/* Key1 isnt in the array so will get PROBEFAIL */
   assert(_lookup_num_probe(aa, &key1) == PROBEFAIL);
/* Add key1 and key2 to the array */
   aa->arr[6].key = &key1;
   aa->arr[7].key = &key2;
   assert(_lookup_num_probe(aa, &key1) == 6);
   assert(_lookup_num_probe(aa, &key2) == 7);
/* Prompt probe to find key3 */
   aa->arr[1].key = &key1;
   aa->arr[4].key = &key3;
   assert(_lookup_num_probe(aa, &key3) != 1);
   assert(_lookup_num_probe(aa, &key3) == 4);

/* aa->size at the moment is 1 (Not actually reflective of strings
   in array due to way of testing) */
   assert(_resize_check(aa) == false);
/* Change size to 6, therefore requiring resize as adding more
   to the array will push size to 7 and therefore >60% full */
   aa->size += 5;
   assert(_resize_check(aa) != false);
   assert(_resize_check(aa) == true);
   assoc_free(aa);

   aa = assoc_init(sizeof(int));
   key4 = (void*)1728;
   key5 = (void*)89;

/* No collisions so should just return normal hash index
   with capacity at 17 now, because fresh init (11 before)*/
   assert(_hash_num_run(aa, &key1) == 2);
   assert(_hash_num_run(aa, &key2) == 11);
   assert(_hash_num_run(aa, &key3) == 7);
/* Now there should be a clash so probe by stepping back */
   aa->arr[2].key = &key4;
   assert(_hash_num_run(aa, &key1) != 2);
   assert(_hash_num_run(aa, &key1) == 10);
/* Probe twice as there will be two collisions */
   aa->arr[10].key = &key5;
   assert(_hash_num_run(aa, &key1) != 10);
   assert(_hash_num_run(aa, &key1) == 1);
/* Change the contents of [1] to key1 (100) so re-running the
   _hash_str_run fn will kick out a duplicate key flage (DUPE)
   after probing twice */
   aa->arr[1].key = &key1;
   assert(_hash_num_run(aa, &key1) == DUPE);
   assoc_free(aa);
   printf("Testing Succesful\n");
}

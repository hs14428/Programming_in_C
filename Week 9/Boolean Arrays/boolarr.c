#include "specific.h"
#include "boolarr.h"

void _bitBytePos(const unsigned int n, unsigned int* bitPos, unsigned int* bytePos);
bool _and(bool b1, bool b2);
bool _xor(bool b1, bool b2);
bool _or (bool b1, bool b2);

/* All elements initialised to "0" */
boolarr* boolarr_init(void)
{
   boolarr *ba;
   ba = ncalloc(1, sizeof(boolarr));
   ba->a = ncalloc(1, INITBYTES);
   ba->nbits  = 0;
   ba->nbytes = INITBYTES;
   return ba;
}

/* Create boolarr based on string e.g. "1100001" */
boolarr* boolarr_initstr(const char* str)
{
   unsigned int bits, i, j;
   boolarr *bn = boolarr_init();

/* Start from end of string */
   bits = strlen(str);
   for (i = bits, j = 0; i > 0; i--, j++) {
      switch (str[i-1]) {
         case '1':
            boolarr_set(bn, j, true);
            break;
         case '0':
            boolarr_set(bn, j, false);
            break;
         default:
            boolarr_free(bn);
            return NULL;
      }
   }
   bn->nbits = bits;
   return bn;
}

/* Return a deep copy */
boolarr* boolarr_clone(const boolarr* ba)
{
   boolarr *clone;
   if (ba == NULL) {
      return NULL;
   }
   clone = (boolarr*) ncalloc(1, sizeof(boolarr));
   clone->a = (byte*) ncalloc(ba->nbytes, INITBYTES);
   memcpy(clone->a, ba->a, ba->nbytes*sizeof(byte));
   clone->nbits  = ba->nbits;
   clone->nbytes = ba->nbytes;
   return clone;
}

/* Get number of bits in array */
unsigned int boolarr_size(const boolarr* ba)
{
   if (ba == NULL) {
      return 0;
   }
   return ba->nbits;
}

/* Return number of bits that are set true */
unsigned int boolarr_count1s(const boolarr* ba)
{
   byte mask = 128;
   unsigned int cnt = 0;
   unsigned int bytePos;

   if (ba == NULL) {
      return 0;
   }
   for (bytePos = 0; bytePos < ba->nbytes; bytePos++) {
      while (mask) {
         if (ba->a[bytePos] & mask) {
            cnt++;
         }
         mask = mask >> 1;
      }
   }
   return cnt;
}

/* Set nth bit on/off */
bool boolarr_set(boolarr* ba, const unsigned int n, const bool b)
{
   byte maskOn;
   byte maskOff = 255;
   unsigned int bitPos, bytePos;
   if (ba == NULL) {
      return false;
   }
/* Workout which byte we need to access, and which bitPos */
   _bitBytePos(n, &bitPos, &bytePos);
/* Resize */
   if (n >= ba->nbits) {
      ba->nbits = n + 1;
   }
   while (bytePos >= ba->nbytes) {
      ba->a = nrecalloc(ba->a, ba->nbytes, ba->nbytes*SCALEFACTOR);
      ba->nbytes *= SCALEFACTOR;
   }
/* Workout bitPos that is on in binary and save as mask */
   maskOn = 1 << bitPos;
   if (b) {
   /* Turn nth bit on/set it to 1 */
      ba->a[bytePos] = ba->a[bytePos]|maskOn;
   }
   else {
   /* Turn nth bit off/set it to 0 */
      maskOff -= maskOn;
      ba->a[bytePos] = ba->a[bytePos]&maskOff;
   }
   return true;
}

/* Get nth bit on/off status*/
bool boolarr_get(const boolarr* ba, const unsigned int n, bool* b)
{
   byte mask;
   byte result;
   unsigned int bitPos, bytePos;
   if ((ba == NULL) || (n >= ba->nbits)) {
      return false;
   }
   _bitBytePos(n, &bitPos, &bytePos);
/* Do a bitwise shift left of 1 for bitPos times, e.g. 1 << 3
   would be: shift 00000001 left 3 times to 00001000. Now the mask
   is equal to the bitPos 3 (4th bit) being on == 8 in decimal */
   mask = 1 << bitPos;
/* Bitwise & logical expression. Will stay on/true/1 if nth bit
   is on/true/1 or return off/false/0 if the bit is off/false/0 */
   result = ba->a[bytePos] & mask;
   if (result) {
      *b = true;
   }
   else {
      *b = false;
   }
   return true;
}

void _bitBytePos(const unsigned int n, unsigned int* bitPos, unsigned int* bytePos)
{
   *bitPos  = n%BITSPERBYTE;
   *bytePos = n/BITSPERBYTE;
}

/* Return if two arrays are the same (bitwise) */
bool boolarr_issame(const boolarr* b1, const boolarr* b2)
{
   unsigned int i;
   bool bit1 = 0, bit2 = 0;

   if ((b1 == NULL) && (b2 == NULL)) {
      return true;
   }
   if (b1 && b2) {
      for (i = 0; i < b1->nbits; i++) {
         boolarr_get(b1, i, &bit1);
         boolarr_get(b2, i, &bit2);
         if (bit1 != bit2) {
            return false;
         }
      }
      return true;
   }
   return false;
}

/* Store to string - rightmost bit is LSB */
bool boolarr_tostring(const boolarr* ba, char* str)
{
   int i;
   bool b;
   if ((ba == NULL) || (str == NULL) || (boolarr_size(ba) == 0)) {
      return false;
   }
   str[0] = '\0';
   for (i = ba->nbits - 1; i >= 0; i--) {
      if (boolarr_get(ba, i, &b)) {
         if (b) {
            str[ba->nbits-i-1] = '1';
         }
         else {
            str[ba->nbits-i-1] = '0';
         }
      }
   }
   str[ba->nbits] = '\0';
   return true;
}

/* Print out array & meta info */
bool boolarr_print(const boolarr* ba)
{
   char str[1000];
   if (ba == NULL) {
      return false;
   }
   boolarr_tostring(ba, str);
   printf("%s\n",str);
   printf("nBits = %d\nnBytes = %d\n", ba->nbits, ba->nbytes);
   return true;
}

/* Flip all bits */
bool boolarr_negate(boolarr* ba)
{
   unsigned int i;
   if ((ba == NULL) || (boolarr_size(ba) == 0)) {
      return false;
   }
   for (i = 0; i < ba->nbits; i++) {
      ba->a[i] = ~ba->a[i];
   }
   return true;
}

/* Functions dealing with 2 bitwise-arrays */
/* Must be the same length */
boolarr* boolarr_bitwise(const boolarr* ba, const boolarr* bb, const logicalop lo)
{
   unsigned int i;
   boolarr* bc;
   bool b1, b2, br;
/* Array of function pointers */
   bool(*f[])(bool, bool) = {_or, _and, _xor};

   if ((ba == NULL) || (boolarr_size(ba) == 0) || (boolarr_size(ba) != boolarr_size(bb))) {
      return NULL;
   }
   bc = boolarr_clone(ba);
   if ((bc == NULL)) {
      return NULL;
   }
   for (i = 0; i < ba->nbits; i++) {
      if (!boolarr_get(ba, i, &b1) || !boolarr_get(bb, i, &b2)) {
         /* Something went wrong with the get? */
         boolarr_free(bc);
         return NULL;
      }

   /* Which particular logic op */
      br = f[lo](b1, b2);

      if (!boolarr_set(bc, i, br)) {
      /* Something went wrong with the set? */
         boolarr_free(bc);
         return NULL;
      }
   }
   return bc;
}

/* Clears all space */
bool boolarr_free(boolarr* p)
{
   if (p == NULL) {
      return true;
   }
   free(p->a);
   free(p);
   return true;
}

bool _xor(bool b1, bool b2)
{
   return (b1 ^ b2);
}

bool _or(bool b1, bool b2)
{
   return (b1 | b2);
}

bool _and(bool b1, bool b2)
{
   return (b1 & b2);
}

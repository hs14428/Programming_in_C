#include <string.h>
#include <math.h>
#include "bool.h"

typedef unsigned char byte;

/* 128 bit number */
#define INITBYTES 16
#define SCALEFACTOR 2
#define BITSPERBYTE 8

typedef struct boolarr {
   byte* a;
   /* Number of bits stored */
   unsigned int nbits;
   /* Physical num of elements in array */
   unsigned int nbytes;
} boolarr;

#include "../../General/bool.h"
#include <math.h>
#include <ctype.h>

#define INITSIZE 17
#define SCALEFACTOR 2
#define CAPCHECK 60
#define ALPHA 26
#define PRIME 31
#define PRIME2 17
#define MAXPROBES 1000
#define PROBEFAIL -1
#define ROUNDUP 0.005
#define DUPE -1

/* change hash to dateframe*/
typedef struct hash {
   void* key;
   void* data;
} hash;

/* Capacity so hash* arr can be realloced */
/* Keysize to decide if int (>0) or str (0) */
/* Size will relate to the number of key/data pairs in hash arry */
typedef struct assoc {
   hash* arr;
   int size;
   int capacity;
   int keysize;
} assoc;

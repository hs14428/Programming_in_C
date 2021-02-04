#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define MAXLEN 50
#define SCALEFACTOR 2
#define BUFFERSIZE 1000
#define FIXEDSIZE 100000

typedef enum bool {false, true} bool;

typedef struct dictionary {
   char arr[MAXLEN];
   int size;
   int capacity;
} Dictionary;

void* ncalloc(int n, size_t size);
void on_error(const char* s);

int main(int argc, char* argv[MAXLEN])
{
   FILE *file;
   char input[MAXLEN];
   char buffer[BUFFERSIZE];
   Dictionary* dict;

   dict = dict_init();

   if ((file = fopen(argv[1], "r")) == NULL) {
      fprintf(stderr, "Failed to open %s\n", argv[1]);
      exit(EXIT_FAILURE);
   }
   if ((fgets(buffer, BUFFERSIZE, file) == NULL) {
      fprintf(stderr, "Failed to read %s file\n", argv[1]);
      exit(EXIT_FAILURE);
   }

   /* code */
   return 0;
}


Dictionary* dict_init(void);
{
   Dictionary *d = (Dictionary*) ncalloc(1, sizeof(Dictionary));
   d->arr = (char*) ncalloc(FIXEDSIZE, sizeof(char));
   d->size = 0;
   d->capacity = FIXEDSIZE;
   return d;
}

void dict_add(Dictionary* d, char str[MAXLEN])
{
   int len = strlen(str);
   if (d) {
      d->arr[d->size] = str;
      d->size = d->size + 1;
      if (d->size >= d->capacity) {
         d->arr = (char*) realloc(d->arr, sizeof(char)*d->capacity*SCALEFACTOR);
         d->capacity = c->capacity*SCALEFACTOR;
      }
   }
}

void* ncalloc(int n, size_t size)
{
   void* v = calloc(n, size);
   if(v==NULL){
      on_error("Cannot calloc() space");
   }
   return v;
}

void on_error(const char* s)
{
   fprintf(stderr, "%s\n", s);
   exit(EXIT_FAILURE);
}

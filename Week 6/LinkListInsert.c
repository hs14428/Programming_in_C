#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct dataframe {
   int i;
   struct dataframe* next;
};
typedef struct dataframe df;

void printl(df* front);
df* insert(df* top, df* f);

int main(void)
{
   df a = {10, NULL};
   df b = {20, NULL};
   df c = {30, NULL};
   df d = {40, NULL};
   df e = {25, NULL};
   df* top;

   a.next = &b;
   b.next = &c;
   c.next = &d;
   top = &a;


   top = insert(top, &e);
   printl(top);

   return 0;
}

df* insert(df* top, df* f)
{
   df *p1, *p2;
   if (top->i > f->i) {
      f->next = top;
      top = f;
   }
   p1 = top;
   p2 = top->next;
   while ((p2 != NULL) && (p2->i < f->i)) {
      p1 = p2;
      p2 = p2->next;
      printf("%d\n", f->i);
   }
   p1->next = f;
   f->next = p2;
   return top;
}

/*f = front*/
void printl(df* f)
{
   while (f != NULL) {
      printf("i=%d next=%p (%p)\n", f->i, (void*)f->next, (void*)f);
      f = f->next;
   }
}

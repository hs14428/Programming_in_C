#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#define INITLEN 10
#define SCALEFACTOR 2
#define MAXSTRSIZE 1000
typedef enum bool {false,true} bool;


typedef struct node
{
   char code;
   char d;
   int freq;
   bool seen;
   struct node* left;
   struct node* right;
}node;

typedef struct node_arr{
   int size;
   int capacity;
   node** array;
   int initial_freq;
}node_arr;



bool add_node(node_arr* n_arr,node* to_add);

void* safe_calloc(size_t nitems, size_t size);
int get_frequency(char c,char* str);
bool duplicate(char c, char* str);
bool free_arr(node_arr* b_arr);
void count_str(char* str,node_arr* n_arr);
bool insert(node_arr* n_arr,node* to_add);
node_arr* init_n_arr(void);
void make_space(node_arr* n_arr,int start_point);
int get_first_unseen(int start,node_arr* n_arr);
int add_freqs(node_arr* n_arr);
node* build_tree(node_arr* n_arr);
char *PrintTree(node *t);

void print_huff(node* t,char str[MAXSTRSIZE]);
void test(void);


int main(void)
{
   test();
   return 0;
}


void test(void)
{
   int i;
   char* test_str="anjjgujhh^^????";
   char test_seen[MAXSTRSIZE];
   char trees[MAXSTRSIZE];

   node_arr* test_arr;
   node* test_node;

   test_seen[0]='a';
   test_seen[1]='\0';
   assert(duplicate('a',test_seen)==true);
   assert(duplicate('x',test_seen)==false);
   assert(test_seen[1]=='x');
   assert(get_frequency('a',test_str)==1);
   assert(get_frequency('<',test_str)==0);

   test_seen[0]='\0';
   test_arr= init_n_arr();
   count_str(test_str,test_arr);
/*   assert(test_arr->size==1);
   count_str(test_str,test_arr);
   assert(test_arr->array[0]->d=='x');
   assert(test_arr->array[0]->freq==1);
   assert(test_arr->array[8]->d=='0');
   assert(test_arr->array[8]->freq==6);
   assert(test_arr->initial_freq==26);*/

   test_node=build_tree(test_arr);
   for(i=0;i<test_arr->size-1;i++)
   {
      printf(" %d\n", test_arr->array[i]->freq);
   }

   printf("%s\n",PrintTree(test_node));
   print_huff(test_node,trees);
   free_arr(test_arr);
}


char *PrintTree(node *t)
{
   char *str;
   assert((str = calloc(MAXSTRSIZE, sizeof(char))) != NULL);
   if(t == NULL)
   {

      return str;
   }
   if(t->d!=0)
   {
      sprintf(str, "%d->%c", t->freq ,t->d);
   }
   else
   {
      sprintf(str, "%d (%s) (%s)", t->freq , PrintTree(t->left), PrintTree(t->right));
   }
   return str;
}


void print_huff(node* t,char str[MAXSTRSIZE])
{

   if(t->left==NULL&&t->right==NULL)
   {
      strncat(str,&t->code,1);

      printf("%c= %s\n",t->d,++str);

   }
   else
   {
      strncat(str,&t->code,1);
      if(t->right)
      {
         print_huff(t->right,str);
         str[strlen(str)-1]='\0';
      }
      if(t->left)
      {
         print_huff(t->left,str);
      }
   }


}


node* node_from_freq(char c,char* seen_str,char* count_str)
{
   node* n;
   if(!duplicate(c,seen_str))
   {
      n = (node *)safe_calloc(1, sizeof(node));
      n->d =c;
      n->freq=get_frequency(c,count_str);
      n->left=NULL;
      n->right=NULL;
      n->seen=false;
      return n;
   }
   return NULL;
}





void count_str(char* str,node_arr* n_arr)
{
   node* letter_node;
   char seen_chars[1000];
   int i;
   seen_chars[0]='\0';
   i=0;
   while(str[i]!='\0')
   {
      letter_node=node_from_freq(str[i],seen_chars,str);
      add_node(n_arr,letter_node);
      i++;
   }
   n_arr->initial_freq=strlen(str);
}




bool duplicate(char c, char* seen_str)
{
   int len;
   if(get_frequency(c,seen_str)>0)
   {
      return true;
   }
   len=strlen(seen_str);
   seen_str[len]=c;
   seen_str[len+1]='\0';
   return false;
}

int get_frequency(char c,char* str)
{
   int count;
   int i;
   count=0;
   i=0;
   while(str[i]!='\0')
   {
      if(str[i]==c)
      {
         count+=1;
      }
      i++;
   }
   return count;
}



node* node_from_nodes(node* node_1,node* node_2)
{
   node *n = (node *)safe_calloc(1, sizeof(node));
   n->freq=node_1->freq+node_2->freq;
   n->left=node_1;
   n->right=node_2;
   n->seen=false;
   node_1->seen=true;
   node_1->code='0';
   node_2->code='1';
   node_2->seen=true;
   return n;
}


/*return void then cast when appropriate*/
void* safe_calloc(size_t nitems, size_t size)
{
   void* ptr;
   ptr = calloc(nitems, size);
   if(ptr==NULL)
   {
      fprintf(stderr,"No space\n");
   	exit(EXIT_FAILURE);
   }
   return ptr;
}


node_arr* init_n_arr(void)
{
   node_arr* n_arr;
   n_arr=(node_arr*)safe_calloc(1,sizeof(node_arr));
   n_arr->array=(node**)safe_calloc(INITLEN,sizeof(node*));
   n_arr->capacity=INITLEN;
   /*need to give an empty space at end for shuffling*/
   n_arr->size=1;
   return n_arr;
}





bool free_arr(node_arr* b_arr)
{
   int y;
   if(b_arr)
   {
      /*free all the bookcases*/
      for(y=0;y<b_arr->size-2;y++)
      {
         free(b_arr->array[y]);
      }
      /*free the array then free the whole struct*/
      free(b_arr->array);
      free(b_arr);
   }
   return true;
}



bool add_node(node_arr* n_arr,node* to_add)
{
   if(to_add)
   {
      insert(n_arr,to_add);
      n_arr->size+=1;
      if(n_arr->size==n_arr->capacity)
      {
         n_arr->array=(node**)realloc(n_arr->array,\
            sizeof(node*)*n_arr->capacity*SCALEFACTOR);

         if(n_arr->array==NULL)
         {
            fprintf(stderr,"No Space\n");
      	 	exit(EXIT_FAILURE);
         }
         n_arr->capacity=n_arr->capacity*SCALEFACTOR;
      }
      return true;
   }

   return false;
}


node* build_tree(node_arr* n_arr)
{
   int i;
   int j;
   node* new_node;
   while(n_arr->array[n_arr->size-2]->freq!=n_arr->initial_freq)
   {
      i=0;
      i=get_first_unseen(i,n_arr);
      j=get_first_unseen(i+1,n_arr);
      new_node=node_from_nodes(n_arr->array[i],n_arr->array[j]);
      add_node(n_arr,new_node);
   }
   return n_arr->array[n_arr->size-2];
}


int get_first_unseen(int start,node_arr* n_arr)
{
   int i;
   i=start;
   if(i>n_arr->size-2)
   {
      fprintf(stderr,"error\n");
      exit(EXIT_FAILURE);
   }
   while(n_arr->array[i]->seen==true)
   {
      i++;
      if(i>=n_arr->size-2)
      {
         fprintf(stderr,"error\n");
         exit(EXIT_FAILURE);
      }
   }
   return i;
}

bool insert(node_arr* n_arr,node* to_add)
{
   int i;
   for(i=0;i<n_arr->size-1;i++)
   {
      if(to_add->freq<= n_arr->array[i]->freq)
      {
         make_space(n_arr,i);
         n_arr->array[i]=to_add;
         return true;


      }
   }
   n_arr->array[n_arr->size-1]=to_add;
   return true;

}

void make_space(node_arr* n_arr,int start_point)
{
   int i;
   for(i=n_arr->size-1;i>start_point;i--)
   {
      n_arr->array[i]=n_arr->array[i-1];
   }

}

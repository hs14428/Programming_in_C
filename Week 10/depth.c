#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#define STRSIZE 5000

struct node{
    char c;
    struct node *left;
    struct node *right;
};
typedef struct node node;

node *MakeNode(char c);
void InsertRandom(node *t, node *n);
char *PrintTree(node *t);
int TreeDepth(node *t);
int leftdepth(node *t);
int rightdepth(node *t);

int main(void){
    
    char c;
    node *head = MakeNode('A');
    node *n;
    
    srand((int)time(NULL));
    for (c = 'B'; c < 'G'; c++) {
        n = MakeNode(c);
        InsertRandom(head, n);
    }

    printf("%s\n", PrintTree(head));
    printf("Tree Depth: %d\n", TreeDepth(head));
    
    return 0;
}

node *MakeNode(char c){
    
    node *n = (node *)calloc(1, sizeof(node));
    assert(n != NULL);
    n->c = c;
    
    return n;
}

void InsertRandom(node *t, node *n){
    
    /*LEFT*/
    if ((rand()%2) == 0) {
        if (t->left == NULL) {
            t->left = n;
        }
        else{
            InsertRandom(t->left, n);
        }
    }
    /*RIGHT*/
    else{
        if (t->right == NULL) {
            t->right = n;
        }
        else{
            InsertRandom(t->right, n);
        }
    }
    
}

char *PrintTree(node *t){
    
    char *str;
    
    assert((str = calloc(STRSIZE, sizeof(char))) != NULL);
    if (t == NULL) {
        strcpy(str, "*");
        return str;
    }
    sprintf(str, "%c(%s)(%s)", t->c, PrintTree(t->left), PrintTree(t->right));
    return str;
}

int TreeDepth(node *t){
    
    /*Node pointers*/
    node *l, *r;
    
    l = t;
    r = t;
    
    if (t!=NULL) {
        if (leftdepth(l) >= rightdepth(r)) {
            return leftdepth(l) + 1;
        }
        else{
            return rightdepth(r) + 1;
        }
    }
    return 0;
    
}

int leftdepth(node *t){
    
    if (t->left != NULL) {
        return leftdepth(t->left) + 1;
    }
        
    return 0;
    
}

int rightdepth(node *t){
    
    if (t->right != NULL) {
        return rightdepth(t->right) + 1;
    }
    
    return 0;
}

#include <stdio.h>

int prime(int a);

int main(void)
{
    int p;
    printf("How many prime numbers would you like to see? ");
    if (scanf("%d", &p) == 1)
    {
        prime(p);
    }
    return 0;
}

int prime(int a)
{
    int num = 2, cnt;
    int prime;
    for (num = 2, cnt = 0; cnt < a; num++)
    {
        int i;
        prime = 1;
        /* Is number prime*/
        for (i = 2; i < num; i++)
        {
            if (num%i == 0)
            {
                prime = 0;
            }
        }
        if (prime == 1)
        {
            printf("%d\n", num);
            cnt++;
        }
    }
    return 0;
}

#include <stdio.h>

int k = 7;

void func(int **s)
{
    int *i = &k;
    *s = i;
    printf("%d, %d, %d\n", k, *i, **s);
}
int main()
{
    int i = 3;
    int *p = &i;
    int **s = &p;

    func(s);
    printf("%d, %d, %d\n", i, *p, **s);
    return  0;
}
//a 777333 b 777777 c 777377 d 777337

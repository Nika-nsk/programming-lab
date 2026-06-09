#include <stdio.h>

int main(void)
{
        int a = 0, b = 0, res;

        scanf("%d", &a); // считываем целое значение в переменную a
        scanf("%d", &b); // считываем целое значение в переменную b

        res = a + b;
        printf("%d + %d = %d\n", a, b, res);
        
        return 0;
}
#include <stdio.h>

int main(void)
{
        int a = 0, b = 0, res;

        scanf("%d%d", &a, &b); // считываем два целых числа в переменные a и b

        res = a + b;
        printf("%d + %d = %d\n", a, b, res);
        
        return 0;
}
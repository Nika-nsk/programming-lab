#include <stdio.h>

int main(void)
{
        int a = 0, b = 0, values_read, res;

        // значение, возвращаемое функцией scanf,
        // присваиваем переменной values_read
        values_read = scanf("%d,%d", &a, &b); 

        // выводим количество успешно записанных переменных на экран
        printf("Number of values read: %d\n", values_read);

        res = a + b;
        printf("%d + %d = %d\n", a, b, res); 
        
        return 0;
}
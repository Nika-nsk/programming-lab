#include <stdio.h>

void show(int N, int matrix[N][N]){ // Вывести матрицу
    for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
    
        printf("%d ", matrix[i][j]);
    }
    printf("\n");}
}
void show_a(int N, int matrix[N][N], int x, int y){ // Вывести матрицу
    printf("x = %d, y = %d\n", x,y);
    for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
        if (i!=x && j!=y)
        printf("[%d][%d] %d\n", i,j, matrix[i][j]);
    }
    printf("\n");}
}
void main() {
    printf("Введите размер матрицы: ");
    int N = 0; // размер матрицы
    scanf("%d", &N);
    int matrix[N][N]; // Создаем матрицу
    printf("Введите значения матрицы: ");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &matrix[i][j]); // Заполняем матрицу
        }
    }
    show(N, matrix);

    int max= matrix[0][0]; // Макс элемент
    int x=0; // его индекс i
    int y=0; // его индекс j
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++){
            if (matrix[i][j]>max){
                max = matrix[i][j];
                x=i;
                y=j;
            }
        }
    }
    printf("max = %d\n",max);
    show_a(N, matrix, x, y);

 int c = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++){
            if (matrix[i][j]==0){
                c++;
                break;
            }
        }
    }

    printf("Количество нулей: %d", c);
}
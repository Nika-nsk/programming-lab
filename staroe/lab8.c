#include <stdio.h>
void show(int N, int matrix[N][N]){ // Вывести матрицу
    for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
    
        printf("%d ", matrix[i][j]);
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

    int max=0; // Макс элемент
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

    for (int i = 0; i < N; i++) { // Перенос столбца с максимальным элементом в конец
        int tmp = 0;
        for (int j = 0; j < N; j++){
            if(j == y){
                tmp = matrix[i][j]; // Временная переменная для запоминания элемента
            }
            if(j >= y && j != N-1){
                matrix[i][j] = matrix[i][j+1];
            }
            else if(j >= y && j == N-1){
                matrix[i][j] = tmp;
            }
        }
    }
    int stroka = 0;
    printf("\n");
    or (int j = 0; j < N; j++) { // перенос строки с максимальным элементом в конец
      /int tmp = 0;
       for (int i = 0; i < N; i++){
           if(i == x){
                tmp = matrix[i][j];
            }
            if(i >= x && i != N-1){
                matrix[i][j] = matrix[i+1][j];
            }
            else if(i >= x && i == N-1){
                matrix[i][j] = tmp;
            }
        }
   }
    N = N-1;
    show(N, matrix);

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




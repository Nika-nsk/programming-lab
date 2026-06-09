#include "struct.h"

int main() { //создала точки
    struct point* A = create_point(0, 4);
    struct point* B = create_point(0, 4);
    
    struct line my_line; // типо создала прямую
    my_line.p1 = *A;
    my_line.p2 = *B;
    
    check_parallel(A, B); //проверила парллельность
    
    free_point(A);
    free_point(B);
    
    return 0;
           }
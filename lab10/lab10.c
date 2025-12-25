#include <stdio.h>
struct point {
    int x;
    int y;
             };
struct line {
    struct point p1;
    struct point p2;
            };
struct point create_point(int x, int y) { //1 функция(создала точку)
    struct point p;
    p.x = x;
    p.y = y;
    return p;
                                        }
void check_parallel(struct point p1, struct point p2) { //2 ф(проверяю параллельность осям)
    if (p1.x==p2.x) {
        printf("Прямая параллельна оси ордтнат OY.\n");
                    } 
    else if (p1.y==p2.y) {
        printf("Пряиая параллельна оси абсцисс OX.\n");
                         }
    else {
        printf("Прямая не параллельна ни одной оси.\n");
         }
                                                      }
int main() {
    struct point A = create_point(0,5);
    struct point B = create_point(0,5);
    struct line my_line;
    my_line.p1 = A;
    my_line.p2 = B;
    check_parallel(my_line.p1, my_line.p2);
    return 0;
}

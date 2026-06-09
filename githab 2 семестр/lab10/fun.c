#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

struct point* create_point(int x, int y) { //1 функция(создала точку)
    struct point* p = malloc(sizeof(struct point));
    p->x = x; 
    p->y = y;  
    return p;
                                        }

void check_parallel(struct point* p1, struct point* p2) { //2 ф(проверяю параллельность осям)
      if (p1->x == p2->x) { 
        printf("Прямая параллельна оси ординат (OY).\n");
    } 
    else if (p1->y == p2->y) { 
        printf("Прямая параллельна оси абсцисс (OX).\n");
    } 
    else {
        printf("Прямая не параллельна ни одной оси.\n");
    }
                                                        }
void free_point(struct point* p) {

}                                      
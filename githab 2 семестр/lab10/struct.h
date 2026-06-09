#pragma once

struct point {  
    int x;  
    int y;  
             };

struct line {  
    struct point p1; 
    struct point p2;  
            };

struct point* create_point(int x, int y); 
void check_parallel(struct point* p1, struct point* p2);  
void free_point(struct point* p); 
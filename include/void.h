#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h> 
#define N 10
struct Flight {
    int flight_number;
    char destination[50];
    int time;
    float price;
};


typedef int (*cmp_func)(struct Flight, struct Flight);

void sort(struct Flight mass[], int n, cmp_func check);
void Print_mass(struct Flight mass[], int n);


int byTime(struct Flight a, struct Flight b);
int byPrice(struct Flight a, struct Flight b);
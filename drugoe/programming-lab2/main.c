#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "void.h"
#include <sys/time.h> 


int main() {
    srand(time(NULL));
struct Flight mass[N];
char cities[4][20] = {
        "Chelyabinsk",
        "Moscow     ",
        "Piter      ",
        "Samara     "
    };
struct timeval start_tv, end_tv;
    long long start_us, end_us;
    double elapsed_ms;
for (int i = 0; i < N; i++) {
        
        mass[i].flight_number = 100 + rand() % 900;
        mass[i].time = 60 + rand() % 300;
        mass[i].price = 3000 + rand() % 10000;

int cityIndex = rand() % 4;
 for (int j = 0; j < 20; j++) {
            mass[i].destination[j] = cities[cityIndex][j];
            if (cities[cityIndex][j] == '\0') break;
 }
}

    Print_mass(mass, N);
    gettimeofday(&start_tv, NULL);
    sort(mass, N, byTime);
    gettimeofday(&end_tv, NULL);
    Print_mass(mass, N);
    sort(mass, N, byPrice); 
    Print_mass(mass, N);
    start_us = start_tv.tv_sec * 1000000 + start_tv.tv_usec;
    end_us = end_tv.tv_sec * 1000000 + end_tv.tv_usec;
    elapsed_ms = (double)(end_us - start_us) / 1000.0;
 
    printf("Время выполнения: %.3f миллисекунд\n", elapsed_ms);
 
    return 0;
}
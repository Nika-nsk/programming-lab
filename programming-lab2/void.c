#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "void.h"
void sort(struct Flight mass[], int n, cmp_func check){
for (int i=0;i<n-1;i++){
    for (int j=0;j<n-i-1;j++){
        if (check(mass[j], mass[j+1])) {
        struct Flight temp=mass[j];
        mass[j] = mass[j + 1];
        mass[j + 1] = temp;
                                           }
                             }   
                        }
                                       }
int byTime(struct Flight a, struct Flight b) {
    return a.time > b.time; 
}

int byPrice(struct Flight a, struct Flight b) {
    return a.price > b.price; 
}
void Print_mass(struct Flight mass[], int n){

printf("Nomer\t\tGorod\t\t\t\tVremya\t\tCena\n");
   
    for (int i = 0; i < N; i++) {
        printf("%d\t\t%s\t\t\t%d\t\t%.2f\n", 
               mass[i].flight_number, 
               mass[i].destination, 
               mass[i].time, 
               mass[i].price);
    }
     printf("--------------------------------------------\n");
}
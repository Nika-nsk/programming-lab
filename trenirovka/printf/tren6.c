#include <stdio.h>

int main(void)
{

        char station_code;   // буквенный код метеостанции
        int station_number;  // числовой код метеостанции
        float temperature;   // температура
        double pressure; // атмфосферное давление

        station_code = 'F';
        station_number = 93;  
        temperature = 25.54;   
        pressure = 755.63;

        printf("|%c%.3d|%+6.2f|%.1f|\n", station_code,station_number,temperature, pressure);
        // |B095| -4.57|750.0|

        return 0;
}
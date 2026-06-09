#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// Структура игрока
typedef struct {
    int x, y;
} Point;

// Карта лабиринта (в реальности лучше читать из файла, здесь для примера массив)
char maze[7][11] = {
    "##########",
    "#S       #",
    "# ###### #",
    "# #    # #",
    "# #  #   #",
    "#    #  E#",
    "##########"
};

// Функция отрисовки (Render) со смайликами
void render(Point player) {
    printf("\033[H\033[2J"); // Очистка экрана

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 10; j++) {
            if (i == player.y && j == player.x) {
                printf("🐱"); // Игрок
            } else if (maze[i][j] == '#') {
                printf("🌳"); // Стена
            } else if (maze[i][j] == 'E') {
                printf("🏠"); // Выход
            } else {
                printf("  "); // Пустота
            }
        }
        printf("\n");
    }
    printf("\nУправление: ESDF (на английском). Выход: Q\n");
}

int main() {
    Point player = {1, 1}; // Стартовая позиция (S)

    // Настройка терминала (аналог stty cbreak -echo)
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char ch;
    while (1) {
        render(player);

        read(STDIN_FILENO, &ch, 1);

        if (ch == 'q' || ch == 'Q') break;

        int nextX = player.x;
        int nextY = player.y;

        // Аналог твоего switch-case
        switch (ch) {
            case 'e': case 'E': nextY--; break; // Вверх
            case 'd': case 'D': nextY++; break; // Вниз
            case 's': case 'S': nextX--; break; // Влево
            case 'f': case 'F': nextX++; break; // Вправо
        }

        // Проверка столкновений
        if (maze[nextY][nextX] != '#') {
            player.x = nextX;
            player.y = nextY;
        }

        // Проверка на победу
        if (maze[player.y][player.x] == 'E') {
            render(player);
            printf("ПОБЕДА! Вы дома.\n");
            break;
        }
    }

    // Возвращаем настройки терминала (аналог stty sane)
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return 0;
}

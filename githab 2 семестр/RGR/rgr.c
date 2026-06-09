#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

int limit = 20; 
int finished = 0;

void logmsg(const char* level, const char* text) {
    FILE* logfile = fopen("app.log", "a");
    if (logfile != NULL) {
        time_t now = time(NULL);
        char* timestamp = ctime(&now);
        timestamp[strcspn(timestamp, "\n")] = 0;
        fprintf(logfile, "[%s] [%s] %s\n", timestamp, level, text);
        fclose(logfile);
    }
}

void* timerthread(void* arg) {
    int counter = 0;
    while (counter < limit) {
        sleep(1);
        counter++;
        if (finished) {
            return NULL;
        }
    }
    logmsg("WARNING", "Timeout");
    printf("\n\n[Время вышло! Лимит %d сек]\n", limit);
    exit(0);
}

int check(const char* src, const char* user) {
    int faults = 0;
    int src_len = (int)strlen(src);
    int user_len = (int)strlen(user);
    
    for (int i = 0; i < src_len; i++) {
        if (i >= user_len) {
            faults += (src_len - i);
            break;
        }
        if (src[i] != user[i]) {
            faults++;
        }
    }
    if (user_len > src_len) {
        faults += (user_len - src_len);
    }
    return faults;
}

void test() {
    int result = check("тест", "тест");
    if (result == 0) {
        logmsg("INFO", "Test passed");
    } else {
        logmsg("ERROR", "Test failed");
        exit(1);
    }
}

int main() {
    system("chcp 65001 > nul");
    srand((unsigned int)time(NULL));
    
    logmsg("INFO", "Start app");
    test();

    FILE* cfg = fopen("config.txt", "r");
    if (cfg != NULL) {
        char line[100];
        if (fgets(line, sizeof(line), cfg) != NULL) {
            sscanf(line, "LIMIT=%d", &limit);
        }
        fclose(cfg);
    }

    const char* words[] = {"книга", "ручка", "экран", "поток", "слово", "время", "язык", "код"};
    const char* phrases[] = {
        "Я помню чудное мгновенье передо мной явилась ты.",
        "Все счастливые семьи похожи друг на друга.",
        "Белеет парус одинокий в тумане моря голубом.",
        "И какой же русский не любит быстрой езды."
    };
    const char* hard[] = {
        "Пароль: 123! Номер_строки = 45;", 
        "Ошибка №404: файл 'main.c' не найден...", 
        "printf(\"Успех! Скорость = %d\", 100);"
    };

    int menu = 0;
    int mode = 0;
    int snake = 1;
    int run = 1;
    char input[256];
    char name[50];
    char line[100];  
    const char* target;
    FILE* file;

    while (1) {
        printf("\n--- МЕНЮ ---\n");
        printf("1. Тренировка\n");
        printf("2. Лидеры\n");
        printf("3. Выход\n");
        printf("Выбор: ");
        if (scanf("%d", &menu) != 1) break;
        getchar();

        if (menu == 3) break;

        if (menu == 2) {
            printf("\nТаблица лидеров:\n");
            file = fopen("leaders.txt", "r");
            if (file == NULL) {
                printf("Пусто\n");
            } else {
                while (fgets(line, sizeof(line), file)) {
                    printf("%s", line);
                }
                fclose(file);
            }
            continue;
        }

        if (menu == 1) {
            printf("\nРежимы:\n");
            printf("1. Слово\n");
            printf("2. Предложение\n");
            printf("3. Текст (сложный)\n");
            printf("4. Змейка (усложнение)\n");
            printf("Выбор: ");
            scanf("%d", &mode);
            getchar();

            if (mode == 4) {
                snake = 1;
            }

            run = 1; 

            while (run) {
                if (mode == 4) {
                    if (snake == 1) target = words[rand() % 8];
                    else if (snake == 2) target = phrases[rand() % 4];
                    else target = hard[rand() % 3];
                    printf("\n[Змейка. Уровень: %d]\n", snake);
                } else {
                    if (mode == 1) target = words[rand() % 8];
                    else if (mode == 2) target = phrases[rand() % 4];
                    else target = hard[rand() % 3];
                }

                printf("Лимит: %d сек\n", limit);
                printf("Текст:\n%s\n\n", target);
                printf("Ввод: ");

                finished = 0;
                pthread_t thread;
                if (pthread_create(&thread, NULL, timerthread, NULL) != 0) {
                    logmsg("ERROR", "No thread");
                    return 1;
                }

                time_t start = time(NULL);
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    finished = 1; 
                }
                input[strcspn(input, "\n")] = 0;

                pthread_join(thread, NULL);
                time_t end = time(NULL);
                int elapsed = (int)difftime(end, start);

                int total_errors = check(target, input);
                int total_len = (int)strlen(target);
                int correct_chars = total_len - total_errors;
                if (correct_chars < 0) correct_chars = 0;

                double accuracy = 0.0;
                if (strlen(input) > 0 || total_errors > 0) {
                    accuracy = ((double)correct_chars / (correct_chars + total_errors)) * 100.0;
                }

                double speed = 0.0;
                if (elapsed > 0) {
                    speed = (((double)correct_chars / 2.0) / elapsed) * 60.0;
                }

                printf("\nРезультат:\n");
                printf("Верно: %d | Ошибки: %d | Время: %d сек\n", correct_chars, total_errors, elapsed);
                printf("Точность: %.1f%% | Скорость: %.1f зн/мин\n", accuracy, speed);

                if (mode == 4) {
                    if (total_errors == 0) {
                        if (snake < 3) {
                            snake++;
                            printf("[Успех! Уровень повышен]\n");
                        } else {
                            printf("[Максимальный уровень пройден!]\n");
                        }
                    } else {
                        snake = 1;
                        printf("[Ошибка! Сброс на уровень 1]\n");
                    }
                }

                printf("\nИмя для таблицы (одно слово): ");
                scanf("%49s", name);
                getchar();

                file = fopen("leaders.txt", "a");
                if (file != NULL) {
                    fprintf(file, "%s - Скр: %.1f | Точн: %.1f%%\n", name, speed, accuracy);
                    fclose(file);
                    printf("Сохранено\n");
                }

                printf("\nПродолжить тренировку? (1 - Да, 0 - Выйти в меню): ");
                scanf("%d", &run);
                getchar();
            }
        }
    }

    logmsg("INFO", "End session");
    return 0;
}

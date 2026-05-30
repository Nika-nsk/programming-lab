#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int turn = 0;

int N;
int num_threads;
int **A, **B, **C;

typedef struct {
    int start_row;
    int end_row;
} ThreadData;

void* child_thread(void* arg) {
    for (int i = 0; i < 5; i++) {
        printf("Дочерний поток: строка %d\n", i + 1);
        sleep(1);
    }
    return NULL;
}

void* sync_child_thread(void* arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        while (turn != 1) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Дочерний поток: строка %d\n", i + 1);
        turn = 0;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* print_lines_thread(void* arg) {
    char** lines = (char**)arg;
    for (int i = 0; i < 3; i++) {
        printf("%s\n", lines[i]);
        sleep(1);
    }
    return NULL;
}

void cleanup_handler(void* arg) {
    char** lines = (char**)arg;
    printf("[%s] получил сигнал отмены и завершает работу\n", lines);
}

void* print_lines_cleanup_thread(void* arg) {
    char** lines = (char**)arg;
    
    pthread_cleanup_push(cleanup_handler, lines);

    for (int i = 0; i < 3; i++) {
        printf("%s\n", lines[i]);
        sleep(1);
    }

    pthread_cleanup_pop(0);
    return NULL;
}

void* sleep_sort_thread(void* arg) {
    long value = (long)arg;
    usleep(value * 10000);
    printf("%ld ", value);
    return NULL;
}

void* multiply_tier(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    
    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    free(data);
    return NULL;
}

void print_matrix(int** matrix, const char* name) {
    printf("Матрица %s:\n", name);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Использование для Пункта 8: %s <размер N> <кол-во потоков>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    num_threads = atoi(argv[2]);

    pthread_t thread;
    printf("Пункт 1\n");
    
    pthread_create(&thread, NULL, child_thread, NULL);
    for (int i = 0; i < 5; i++) {
        printf("Родительский поток: строка %d\n", i + 1);
        sleep(1);
    }
    pthread_join(thread, NULL);

    printf("\nПункт 2\n");
    pthread_create(&thread, NULL, child_thread, NULL);
    pthread_join(thread, NULL);
    for (int i = 0; i < 5; i++) {
        printf("Родительский поток: строка %d\n", i + 1);
        sleep(1);
    }

    printf("\nПункт 3\n");
    pthread_t threads[4];

    char* text0[] = {"Поток 0: раз", "Поток 0: два", "Поток 0: три"};
    char* text1[] = {"Поток 1: красный", "Поток 1: желтый", "Поток 1: зеленый"};
    char* text2[] = {"Поток 2: утро", "Поток 2: день", "Поток 2: вечер"};
    char* text3[] = {"Поток 3: зима", "Поток 3: весна", "Поток 3: лето"};

    pthread_create(&threads[0], NULL, print_lines_thread, text0);
    pthread_create(&threads[1], NULL, print_lines_thread, text1);
    pthread_create(&threads[2], NULL, print_lines_thread, text2);
    pthread_create(&threads[3], NULL, print_lines_thread, text3);

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nПункт 4\n");
    pthread_create(&threads[0], NULL, print_lines_thread, text0);
    pthread_create(&threads[1], NULL, print_lines_thread, text1);
    pthread_create(&threads[2], NULL, print_lines_thread, text2);
    pthread_create(&threads[3], NULL, print_lines_thread, text3);

    sleep(2);

    for (int i = 0; i < 4; i++) {
        pthread_cancel(threads[i]);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nПункт 5\n");
    pthread_create(&threads[0], NULL, print_lines_cleanup_thread, text0);
    pthread_create(&threads[1], NULL, print_lines_cleanup_thread, text1);
    pthread_create(&threads[2], NULL, print_lines_cleanup_thread, text2);
    pthread_create(&threads[3], NULL, print_lines_cleanup_thread, text3);

    sleep(2);

    for (int i = 0; i < 4; i++) {
        pthread_cancel(threads[i]);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nПункт 6\n");
    long array[] = {5, 2, 9, 1, 7, 3, 8, 4, 6, 10};
    int n = 10;
    pthread_t sort_threads[10];

    printf("Исходный массив: ");
    for (int i = 0; i < n; i++) {
        printf("%ld ", array[i]);
    }
    printf("\nОтсортированный массив: ");

    for (int i = 0; i < n; i++) {
        pthread_create(&sort_threads[i], NULL, sleep_sort_thread, (void*)array[i]);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(sort_threads[i], NULL);
    }
    printf("\n");

    printf("\nПункт 7\n");
    turn = 0;
    pthread_create(&thread, NULL, sync_child_thread, NULL);

    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        while (turn != 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Родительский поток: строка %d\n", i + 1);
        turn = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    pthread_join(thread, NULL);

    printf("\nПункт 8\n");
    if (num_threads > N) {
        num_threads = N;
    }

    A = malloc(N * sizeof(int*));
    B = malloc(N * sizeof(int*));
    C = malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        A[i] = malloc(N * sizeof(int));
        B[i] = malloc(N * sizeof(int));
        C[i] = malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }

    if (N < 5) {
        print_matrix(A, "A");
        print_matrix(B, "B");
    }

    pthread_t* mat_threads = malloc(num_threads * sizeof(pthread_t));
    int rows_per_thread = N / num_threads;
    int extra_rows = N % num_threads;
    int current_row = 0;

   
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < num_threads; i++) {
        ThreadData* data = malloc(sizeof(ThreadData));
        data->start_row = current_row;
        data->end_row = current_row + rows_per_thread + (i < extra_rows ? 1 : 0);
        current_row = data->end_row;

        pthread_create(&mat_threads[i], NULL, multiply_tier, data);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(mat_threads[i], NULL);
    }

    
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;

    if (N < 5) {
        print_matrix(C, "C");
    }

    printf("Время вычислений: %.2f мс (Размер: %d, Потоков: %d)\n", elapsed_ms, N, num_threads);

    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
    free(mat_threads);
    

    return 0;
}

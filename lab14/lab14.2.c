#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct room {
    char name[30];
    int level;
    int number;
    int resolution;
};

typedef struct node {
    struct room data;
    struct node* next;
    struct node* prev;
    struct node* up;
    struct node* down;
} node_t;

node_t* newNode(struct room data) {
    node_t* node = (node_t*)malloc(sizeof(node_t));
    node->data = data;
    node->next = node->prev = node->up = node->down = NULL;
    return node;
}

node_t* appendBack(node_t* head, struct room data) {
    node_t* new_node = newNode(data);
    if (head == NULL) return new_node;

    node_t* cur = head;
    while (cur->next !=NULL){
        cur = cur->next;
    }
    cur->next = new_node;
    new_node->prev = cur;
    return head;
}

int main() {
    srand(time(NULL));
    node_t* S = NULL;
    node_t* bottom = NULL;
    int n;

    printf("Кол-во столбцов:");
    if (scanf("%d", &n) != 1) return 1;

    for (int i = 0; i<n; i++) {
        struct room r;
        sprintf(r.name, "Комната%d", i+1);
        r.level = rand() % 10 + 1;
        r.number = i + 1;
        r.resolution = 100;
        S = appendBack(S, r);
    }
    for (int i = 0; i<n; i++) {
        struct room r;
        sprintf(r.name, "Комната%d", n+i+1);
        r.level = rand() % 10 + 1;
        r.number = n + i + 1;
        r.resolution = 200;
        bottom = appendBack(bottom, r);
    }
    node_t* t = S;
    node_t* b = bottom;
    while (t !=NULL && b !=NULL) {
        t->down = b;
        b->up = t;
        t = t->next;
        b = b->next;
    }
    node_t* current = S;
    char cmd;

    printf("Управление:");
    printf("Вправо: D\n");
    printf("Вверх: W\n");
    printf("Влево: A\n");
    printf("Вниз: S\n");
    printf("Выход:0\n");

    while (current != NULL) {
        printf("Комната: %s (Уровень: %d, №: %d)\n", current->data.name, current->data.level, current->data.number);
        printf("Ваш ход:");
        scanf(" %c", &cmd);

        if (cmd == '0') break;
        
        node_t* nextStep =NULL;

        if (cmd =='w' || cmd == 'W') nextStep = current->up;
        else if (cmd =='s' || cmd =='S') nextStep = current->down;
        else if (cmd =='a' || cmd =='A') nextStep = current->prev;
        else if (cmd =='d' || cmd =='D') nextStep = current->next;
            if (nextStep !=NULL) {
                current = nextStep;
            } else {
                printf("!!! ТУПИК\n");
                if (cmd =='d' || cmd == 'D') {
                printf("Вернуться в начало к S?(1-да, 0-нет):");
                char choice;
                scanf(" %c", &choice);
                if (choice == '1') current = S;
                }
            }
        }
    printf("Игра завершена\n");
    return 0;
}
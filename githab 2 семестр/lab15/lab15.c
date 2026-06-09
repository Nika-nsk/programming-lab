#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
struct Empl {
    int id;
    char name[100];
    char level[20];
} ;

void compress() {
    FILE *in = fopen("database.dat", "rb");
    FILE *out = fopen("database.rle", "wb");
    if (!in || !out)
    return;
   
   
    char byte, next;
    int count = 1; 
     if (fread(&byte, 1, 1, in) == 1) { 
        while (fread(&next, 1, 1, in) == 1) {
            if (byte == next && count < 255) {
                count++;
            } else {
                fwrite(&count, 1, 1, out);
                fwrite(&byte, 1, 1, out);
                byte = next;
                count = 1;
            }
        }
        fwrite(&count, 1, 1, out);
        fwrite(&byte, 1, 1, out);
     } // a
     fclose(in);
     fclose(out);
}
void decompress() {
    FILE *in = fopen("database.rle", "rb");
    FILE *out = fopen("datacompress.dat", "wb");
    if (!in || !out)
    return;
    char count, value;
    while (fread(&count, 1, 1, in)==1) {
        fread(&value, 1, 1, in);
        for(int i=0; i<count; i++) {
            fwrite(&value, 1, 1, out);
        }
    }
    fclose(in);
    fclose(out);
}

int main(int text, char *text1[] ){
    if (text<2){
        printf("Кажется вы забыли указать имя файла\n");
        return 1;
    }

    FILE *file = fopen(text1[1], "r");
    if (file == NULL) {
        printf("Файл не удaлось открыть\n");
        return 1;
    }
    struct Empl staff[100];
    char line[200];
    int count = 0;
    
   
    printf("Имя        | ID         | Уровень\n");
    printf("-----------------------------------------\n");


    while (fgets(line, 200, file) != NULL) {
        char *p;
        p = strtok(line, ";");
        if (p !=NULL) {
            strcpy(staff[count].name, p);
        }
         p = strtok(NULL, ";");
        if (p != NULL) {
            staff[count].id = atoi(p); 
        }

        
        p = strtok(NULL, ";");
        if (p !=NULL) {
            strcpy(staff[count].level, p);
        }

        printf("%s     | %d     | %s\n", staff[count].name,staff[count].id, staff[count].level);
        count++;
    }
    fclose(file);

    FILE *bin = fopen("database.dat", "wb");
    fwrite(staff, sizeof(struct Empl), count, bin);
    fclose(bin);
    printf("\n Записали в database.dat\n");

    int ID;
    printf("ID:");
    scanf("%d", &ID);

    FILE *readbin = fopen("database.dat", "rb");
    struct Empl temp;
    int found = 0;

    while (fread(&temp, sizeof(struct Empl), 1, readbin)) {
        if (temp.id == ID) {
            printf("То, что вы искали:%s, %s\n", temp.name, temp.level);
            found = 1;
        }
    }
    if (found == 0) {
        printf("Вы ошиблись, такого сотрудника нет\n");
    }
    fclose(readbin);
    compress();
    decompress();
}
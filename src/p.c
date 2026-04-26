#include <stdio.h>
#include <string.h>

// Шифрование по твоему принципу: "Нет нулям!"
void xor_encrypt(char *text, char *key, char *output) {
    int text_len = strlen(text);
    int key_len = strlen(key);
    
    for (int i = 0; i < text_len; i++) {
        char res = text[i] ^ key[i % key_len];
        
        if (res == 0) {
            output[i] = 1; // Вместо 00 пишем 01, чтобы strlen не споткнулся
        } else {
            output[i] = res;
        }
    }
    output[text_len] = '\0'; // Настоящий конец строки
}

// Расшифровка по твоему принципу: "Вижу 1 — думаю 0"
void xor_decrypt(char *text, char *key, char *output) {
    int text_len = strlen(text); // Теперь работает до конца!
    int key_len = strlen(key);
    
    for (int i = 0; i < text_len; i++) {
        char current = text[i];
        
        if (current == 1) {
            output[i] = 0 ^ key[i % key_len]; // Восстанавливаем как из нуля
        } else {
            output[i] = current ^ key[i % key_len];
        }
    }
    output[text_len] = '\0';
}

int main() {
    // В этой строке и ключе при обычном XOR точно будет 00 (H ^ H = 0)
    char text[] = "Hello World";
    char key[] = "s"; 
    
    char encrypted[64];
    char decrypted[64];
     printf("Original: %s\n", text);
    xor_encrypt(text, key, encrypted);
    printf("Зашифровано (байты): ");
    for (int i = 0; i < strlen(text); i++) {
    printf("%02X ", (unsigned char)encrypted[i]); 
    // %02X выведет число в 16-ричном виде (например, 3B, 01, FF)
}
printf("\n");

    xor_decrypt(encrypted, key, decrypted);
    printf("Расшифровано: %s\n", decrypted);

    return 0;
}

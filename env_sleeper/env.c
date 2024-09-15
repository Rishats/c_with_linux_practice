#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ENV_FILE ".env"
#define SLEEP_DURATION 600

void read_env_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        perror("Ошибка: Не удалось открыть файл");
        printf("Файл %s не найден. Засыпаю на %d секунд...\n", filename, SLEEP_DURATION);
        sleep(SLEEP_DURATION);
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Удаляем символ новой строки
        line[strcspn(line, "\n")] = 0;
        
        // Разделяем строку по знаку '='
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "=");

        if (key && value) {
            printf("Переменная окружения: %s = %s\n", key, value);
        }
    }

    fclose(file);
}

int main() {
    read_env_file(ENV_FILE);
    return 0;
}


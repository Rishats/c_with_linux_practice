#include <stdio.h>
#include <stdlib.h>

int main() {
    // Открываем файл top.log для записи
    FILE *log_file = fopen("top.log", "w");
    if (log_file == NULL) {
        perror("Ошибка при открытии файла");
        return 1;
    }

    // Выполняем команду top с аргументом -b (batch mode)
    // чтобы она работала в неинтерактивном режиме и писала вывод в файл
    char *command = "top -b"; // -n 1 ограничивает вывод до одного цикла обновления

    // Открываем поток для чтения из команды
    FILE *top_output = popen(command, "r");
    if (top_output == NULL) {
        perror("Ошибка при выполнении команды");
        fclose(log_file);
        return 1;
    }

    // Читаем вывод команды top и записываем его в файл
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), top_output) != NULL) {
        fputs(buffer, log_file);
    }

    // Закрываем потоки
    pclose(top_output);
    fclose(log_file);

    printf("Лог команды top записан в файл top.log\n");

    return 0;
}


// main1.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("Executing main2...\n");

    char *args[] = {"./main2", NULL}; // Путь к бинарю main2
    execvp(args[0], args);  // Запуск main2

    // Если execvp завершится с ошибкой, выведем сообщение
    perror("Error executing main2");
    return 1;
}


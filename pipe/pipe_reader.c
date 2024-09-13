#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    char buffer[1024];
    ssize_t bytesRead;

    // Чтение данных с файлового дескриптора STDIN (0)
    while ((bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';  // Добавляем нуль-терминатор для строки
        printf("Received: %s", buffer);
    }

    printf("My PID: %d\n", getpid());

    // Если чтение завершилось (пайп закрыт), выводим сообщение
    if (bytesRead == 0) {
        printf("Pipe closed. Exiting...\n");
    } else {
        perror("Read error");
    }

    return 0;
}


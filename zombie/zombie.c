#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>  // Для printf

int main() {
    pid_t child_pid;
    /* Создание дочернего процесса. */
    printf("До fork()\n");
    child_pid = fork();  // Создаем новый процесс
    printf("После fork(), child_pid = %d\n", child_pid);

    if (child_pid > 0) {
        // Родительский процесс
        printf("Это родительский процесс с PID = %d, дочерний PID = %d\n", getpid(), child_pid);
        wait(NULL);  // Ждем завершения дочернего процесса
        sleep(60);   // Родительский процесс делает паузу
    } else if (child_pid == 0) {
        // Дочерний процесс
        printf("Это дочерний процесс с PID = %d\n", getpid());
        exit(0);  // Завершаем дочерний процесс
    } else {
        // Ошибка
        printf("Ошибка при создании процесса\n");
    }

    return 0;
}


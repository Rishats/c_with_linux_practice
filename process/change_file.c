#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void error_exit(const char* msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pid> <new_log_file>\n", argv[0]);
        exit(1);
    }

    pid_t target_pid = atoi(argv[1]);
    const char *new_file = argv[2];

    // Открываем новый файл для подмены
    int new_fd = open(new_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (new_fd == -1) {
        error_exit("Failed to open new log file");
    }

    // Прикрепляемся к процессу через ptrace
    if (ptrace(PTRACE_ATTACH, target_pid, NULL, NULL) == -1) {
        error_exit("PTRACE_ATTACH");
    }

    // Ожидаем, пока процесс остановится
    waitpid(target_pid, NULL, 0);

    // Дублируем дескриптор нового файла (например, 4)
    if (dup2(new_fd, 4) == -1) {
        error_exit("dup2 failed for new descriptor");
    }

    // Теперь перенаправляем дескриптор 3 (используемый процессом) на новый файл (через дескриптор 4)
    if (dup2(4, 3) == -1) {
        error_exit("dup2 failed for original descriptor");
    }

    // Закрываем новый файловый дескриптор 4, так как он теперь привязан к 3
    close(4);

    // Возвращаем управление процессу
    if (ptrace(PTRACE_DETACH, target_pid, NULL, NULL) == -1) {
        error_exit("PTRACE_DETACH");
    }

    printf("File descriptor successfully replaced.\n");

    return 0;
}


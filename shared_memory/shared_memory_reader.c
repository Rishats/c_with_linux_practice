#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *name = "/my_shared_memory";  // Имя разделяемой памяти, должно совпадать

    // Открываем объект разделяемой памяти, который уже создан первым процессом
    int shm_fd = shm_open(name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    // Определяем размер разделяемой памяти
    size_t size = 1024;  // Размер должен быть таким же, как в первом процессе

    // Отображаем разделяемую память в адресное пространство процесса
    void *ptr = mmap(NULL, size, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    while(1) {
        // Читаем данные из разделяемой памяти
        printf("Shared memory contents: %s\n", (char *)ptr);
    }

    // Освобождаем ресурсы
    if (munmap(ptr, size) == -1) {
        perror("munmap");
        return 1;
    }

    // Закрываем объект разделяемой памяти
    if (close(shm_fd) == -1) {
        perror("close");
        return 1;
    }
 
   return 0;
}


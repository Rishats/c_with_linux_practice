#include <stdio.h>      // For printf, perror, and sprintf
#include <sys/mman.h>   // For mmap, munmap, and PROT_* constants
#include <sys/stat.h>   // For mode constants (e.g., 0666)
#include <fcntl.h>      // For O_CREAT, O_RDWR, shm_open
#include <unistd.h>     // For ftruncate, close
#include <string.h>     // For using string functions if needed (not required here)

int main() {
    const char *name = "/my_shared_memory";
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    // Set the size of the shared memory region
    size_t size = 1024;

    // Resize the shared memory object to the desired size
    if (ftruncate(shm_fd, size) == -1) {
        perror("ftruncate");
        return 1;
    }

    // Map the shared memory object into the process address space
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // Write data to the shared memory
    sprintf(ptr, "Hello, shared memory!");

    // Print the contents of the shared memory
    printf("Shared memory contents: %s\n", (char *)ptr);

    printf("We sleep 600, check /proc/pid/fd for shared memory \n");

    int pidget = getpid();

    printf("PID of process: %d \n", pidget);

    while(1) {
      sprintf(ptr, "Go!");
    }

    sleep(600);

    // Unmap the shared memory object
    if (munmap(ptr, size) == -1) {
        perror("munmap");
        return 1;
    }

    // Close the shared memory file descriptor
    if (close(shm_fd) == -1) {
        perror("close");
        return 1;
    }

    // Unlink the shared memory object
    if (shm_unlink(name) == -1) {
        perror("shm_unlink");
        return 1;
    }

    return 0;
}

# Shared Memory Example in C

This repository contains two simple C programs that demonstrate how to create and use shared memory between two processes using POSIX shared memory APIs (`shm_open`, `mmap`, etc.).

## Files
1. **shared_memory_writer.c**: This program creates a shared memory object, writes data to it, and keeps it alive for other processes to access.
2. **shared_memory_reader.c**: This program opens an existing shared memory object and reads the data written by the writer.

## Prerequisites

- POSIX-compliant system (Linux, macOS, etc.).
- GCC (GNU Compiler Collection) or any C compiler installed.

## Compilation

You can compile the two C files using `gcc`:

```bash
gcc -o shared_memory_writer shared_memory_writer.c -lrt
gcc -o shared_memory_reader shared_memory_reader.c -lrt
```

The `-lrt` flag links the POSIX real-time library, which is required for shared memory functions.

## How to Run

### 1. Running the Writer
The `shared_memory_writer` program creates a shared memory object, writes a message to it, and continuously updates it. It also prints the process ID (PID) so that you can monitor it via `/proc/[pid]/fd`:

```bash
./shared_memory_writer
```

Output:

```
Shared memory contents: Hello, shared memory!
We sleep 600, check /proc/pid/fd for shared memory 
PID of process: 12345 
```

The writer process will keep running for 600 seconds, during which time you can observe and interact with the shared memory.

### 2. Running the Reader
In another terminal, you can run the `shared_memory_reader` program to read the data from the shared memory created by the writer:

```bash
./shared_memory_reader
```

Output:

```
Shared memory contents: Hello, shared memory!
```

This program reads the data written by the writer and prints it.

### Shared Memory Object
- The shared memory object created by the writer is named `/my_shared_memory`. You can view it in the `/dev/shm` directory:
  
  ```bash
  ls /dev/shm
  ```

  You should see the shared memory file listed, e.g. `/dev/shm/my_shared_memory`.

### Termination
- The writer process will run for 600 seconds. After that, it will clean up the shared memory using `shm_unlink`, removing the object from the system.
- You can manually stop the writer and reader processes with `Ctrl+C`.

## How it Works

- **Writer (shared_memory_writer.c)**:
  - Creates a shared memory object using `shm_open`.
  - Sets the size of the shared memory using `ftruncate`.
  - Maps the shared memory into the process's address space using `mmap`.
  - Writes a message into the shared memory and continuously updates it.
  - Cleans up the memory by unmapping it and closing the file descriptor.

- **Reader (shared_memory_reader.c)**:
  - Opens the shared memory object created by the writer using `shm_open`.
  - Maps the shared memory into its own address space.
  - Reads the message written by the writer and prints it.
  - Unmaps the memory and closes the file descriptor.

## Notes

- Ensure that the writer program is running before attempting to read the shared memory.
- The shared memory object will be removed from the system when the writer process finishes running or calls `shm_unlink`.


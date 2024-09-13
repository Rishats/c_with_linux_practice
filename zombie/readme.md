# Fork Example Program

This program demonstrates the usage of the `fork()` system call in C to create a new process. It shows how a parent process can create a child process, and how they can run simultaneously. The parent process waits for the child to finish using `wait()`, and then pauses for 60 seconds before terminating.

## How the Program Works

1. **Before Fork**:
   - The program prints "Before fork()".
   
2. **Forking**:
   - The `fork()` system call is used to create a new process. It returns the `PID` (Process ID) of the child process to the parent, and `0` to the child process.
   
3. **Parent Process**:
   - If `fork()` returns a positive value (`child_pid > 0`), it means the code is running in the parent process.
   - The parent process waits for the child process to finish using `wait()`, and then sleeps for 60 seconds.

4. **Child Process**:
   - If `fork()` returns `0`, the code is running in the child process.
   - The child process prints its PID and then exits.

5. **Error Handling**:
   - If `fork()` returns a negative value, it indicates an error occurred while creating the process.

## Output

The program prints messages from both the parent and the child process. Sample output could look like:

```
Before fork()
After fork(), child_pid = 1234
This is the parent process with PID = 5678, child PID = 1234
After fork(), child_pid = 0
This is the child process with PID = 1234
```

The output order may vary because both the parent and child processes run concurrently.

## Compilation

To compile the program, use the following command:

```bash
gcc -o zombie zombie.c
```

This will generate an executable file called `zombie`.

## Execution

To run the program, simply execute the binary:

```bash
./zombie
```

### Expected Behavior

- The parent process will wait for the child process to finish using `wait()`.
- The child process will print its PID and exit.
- The parent process will then sleep for 60 seconds before terminating.

### Notes

- The `wait()` call ensures that the parent process pauses until the child process finishes.
- After the child process terminates, the parent process waits for 60 seconds before completing, allowing you to observe the child-parent process interaction.

## Code Overview

### Libraries Used:

- `stdlib.h`: Provides the `exit()` function.
- `sys/types.h`: Defines data types used in system calls.
- `sys/wait.h`: Provides the `wait()` function.
- `unistd.h`: Provides the `fork()` and `sleep()` functions.
- `stdio.h`: Provides the `printf()` function for printing output.

### Key Functions:

- `fork()`: Creates a new process. The parent process receives the PID of the child, and the child receives `0`.
- `getpid()`: Returns the process ID of the current process.
- `wait()`: Causes the parent process to wait until the child process has finished execution.
- `exit()`: Terminates the child process.
- `sleep()`: Pauses execution of the parent process for 60 seconds.

## Conclusion

This program is a basic example of process creation using `fork()` in C. It demonstrates the relationship between a parent and a child process and how the parent can wait for the child to finish before proceeding.


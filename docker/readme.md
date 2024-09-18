# SIGKILL in Containers

This project demonstrates how a containerized application can handle signals like `SIGQUIT` when it's running as the PID 1 process. The behavior of PID 1 in containers is different from other processes, and this project showcases how to correctly handle signals to ensure proper shutdown or restart of a container.

## Overview

In Docker, PID 1 is the first process started inside a container. Typically, sending signals to processes like `SIGTERM`, `SIGKILL`, or `SIGQUIT` can terminate or restart processes. However, the behavior is different for PID 1 inside a container:

- The only signals that can be sent to PID 1 are those for which explicit signal handlers have been installed.
- **SIGKILL** cannot be caught or ignored, and it must be handled by the system or container orchestrator (like Docker or Tsuru).
- If no handler is set for signals like `SIGTERM`, the process may not respond, leaving the container stuck.

This project demonstrates:
- How to register a signal handler for `SIGQUIT` (`kill -3`).
- How an application running as PID 1 can handle signals properly inside a container.

## Prerequisites

Ensure you have the following installed on your system:
- Docker
- GCC (for compiling the C code if not using Docker)

## Building the Docker Image

To build the Docker image for the project, use the following command:

```bash
docker build -t sigkill_in_container .
```

This will compile the C program (`sigkill_in_container.c`) and create a Docker image with the application that demonstrates the signal handling behavior.

## Running the Container

To run the container:

```bash
docker run --name signal-test sigkill_in_container
```

In this case, a signal handler for `SIGQUIT` is registered, allowing you to send the `SIGQUIT` signal and trigger the handler.

### Example Usage

1. **With Signal Handler for `SIGQUIT`:**
   Run the container as shown:
   ```bash
   docker run --name signal-test sigkill_in_container
   ```

   In this case, a signal handler for `SIGQUIT` is registered, and you can send the signal using:
   ```bash
   docker exec signal-test kill -3 1
   ```

   The signal handler will catch the `SIGQUIT` signal, and the application will exit properly, stopping the container.

2. **Attempting Other Signals (Which Won't Work Inside the Container):**
   If you try to send other signals like `SIGTERM` or `SIGKILL` inside the container, they will not work because the program does not have handlers for those signals:
   
   - Sending `SIGKILL` or `SIGTERM` from within the container:
     ```bash
     docker exec signal-test kill -9 1   # SIGKILL (won't work inside the container)
     docker exec signal-test kill -15 1  # SIGTERM (won't work inside the container)
     ```

   Since the application does not have a handler for `SIGTERM`, and `SIGKILL` cannot be handled by any process, the container will not stop. Only `SIGQUIT` will work because we explicitly registered a handler for it.

## Killing the Container from the Host

To forcibly kill the container from the host (for example, if `SIGKILL` is needed):

```bash
docker kill signal-test
```

This sends a `SIGKILL` to the container from outside, which cannot be ignored or handled, and the container will stop.

## Code Explanation

The C program `sigkill_in_container.c` demonstrates:

- If no signal handler is registered for a specific signal (like `SIGTERM`), the signal won't stop the application from inside the container.
- When a signal handler is registered for `SIGQUIT`, it catches the signal and exits gracefully when `kill -3` is used inside the container.

By registering signal handlers, you ensure the application can shut down gracefully or take the necessary action when a signal is received.

## Conclusion

This project provides an example of how to handle signals in Docker containers, particularly when your application runs as PID 1. **Only signals for which a handler is registered (like `SIGQUIT`) will work inside the container**. Signals like `SIGKILL` or `SIGTERM` won't work unless they are handled externally by Docker or from the host system.

## Additional Resources

- [Docker Signal Handling Documentation](https://docs.docker.com/engine/reference/builder/#cmd)

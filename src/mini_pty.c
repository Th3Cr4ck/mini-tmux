#include "mini_pty.h"

#include <pty.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int pty_start(Pty *pty) {

    char pty_name[256];

    pid_t pid = forkpty(&pty->master_fd, pty_name, NULL, NULL);

    if (pid == -1) {
        perror("forkpty failed");
        return -1;
    }

    // Hijo
    if (pid == 0) {
        execl("/bin/bash", "bash", (char *)NULL);

        perror("execl failed");
        exit(1);
    }

    // Padre
    pty->child_pid = pid;

    printf("PTY creado: %s\r\n", pty_name);
    printf("PID shell: %d\r\n", pid);

    return 0;
}

ssize_t pty_read(Pty *pty, char *buffer, size_t size) {
    return read(pty->master_fd, buffer, size);
}

ssize_t pty_write(Pty *pty, const char *buffer, size_t size) {
    return write(pty->master_fd, buffer, size);
}

void pty_destroy(Pty *pty) {

    int status;

    waitpid(pty->child_pid, &status, 0);

    close(pty->master_fd);
}

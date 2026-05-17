#ifndef MINI_PTY_H
#define MINI_PTY_H

#include <sys/types.h>
#include <stddef.h>

typedef struct {
  int master_fd;
  char ptyName[256];
  pid_t child_pid;
} Pty ;

int pty_start(Pty *pty);
ssize_t pty_read(Pty *pty, char *buf, size_t size);
ssize_t pty_write(Pty *pty, const char *buf, size_t size);
void pty_destroy(Pty *pty);

#endif

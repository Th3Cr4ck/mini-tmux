#include "event_loop.h"

#include <poll.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_BUFFER 4096

void event_loop_run(Pty *pty) {
  struct pollfd fds[2];
  // stdin
  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;

  // pty
  fds[1].fd = pty->master_fd;
  fds[1].events = POLLIN;

  char buffer[MAX_BUFFER];

  while (1) {

    int ret = poll(fds, 2, -1);

    if (ret == -1) {
      perror("poll");
      break;
    }

    /*
     * PTY died
     */
    if (fds[1].revents & (POLLHUP | POLLERR)) {
      break;
    }

    /*
     * stdin -> pty
     */
    if (fds[0].revents & POLLIN) {
      ssize_t n = read(STDIN_FILENO, buffer, sizeof(buffer));

      if (n <= 0)
        break;

      ssize_t written = pty_write(pty, buffer, n);

      if (written <= 0)
        break;
    }

    /*
     * pty -> stdout
     */
    if (fds[1].revents & POLLIN) {
      ssize_t n = pty_read(pty, buffer, sizeof(buffer));

      if (n <= 0)
        break;

      ssize_t written = write(STDOUT_FILENO, buffer, n);

      if (written <= 0)
        break;
    }
  }
}

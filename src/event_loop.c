#include "event_loop.h"

#include <poll.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_BUFFER 4096

static void ctrlB_command();

void event_loop_run(Session *session) {

  struct pollfd fds[2];
  // stdin
  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;

  // pty
  fds[1].fd = session->pty.master_fd;
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

      if (buffer[0] == 0x02) // Ctrl+B
      {
        printf("CTRL+B detected!\r\n");
        ctrlB_command();
      }

      ssize_t written = session_write_pty(session, buffer, n);

      if (written <= 0)
        break;
    }

    /*
     * pty -> stdout
     */
    if (fds[1].revents & POLLIN) {
      ssize_t n = session_read_pty(session, buffer, sizeof(buffer));

      if (n <= 0)
        break;

      ssize_t written = write(STDOUT_FILENO, buffer, n);

      if (written <= 0)
        break;
    }
  }
}

void ctrlB_command() {
  char command[3];
  ssize_t n = read(STDIN_FILENO, command, sizeof(command));

  if (n <= 0)
    return;

  if (command[0] == 'c')
    printf("Crear otro PTY\r\n");
  else
    printf("CTRL+B + %c command not defined\r\n", command[0]);
}

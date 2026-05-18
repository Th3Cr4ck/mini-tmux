#include "event_loop.h"

#include <poll.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_BUFFER 4096

static void ctrlB_command();

void event_loop_run(SessionManager *sm) {

  struct pollfd fds[2];

  // stdin
  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;

  // pty
  for (size_t i = 1; i <= sm->count; i++) {
    fds[i].fd = sm->sessions[i - 1].pty.master_fd;
    fds[i].events = POLLIN;
    // printf("fds[%lu].fd = %d\r\n", i, fds[i-1].fd);
    // printf("fds[%lu].events = %d\r\n", i, fds[i-1].events);
  }

  char buffer[MAX_BUFFER];

  while (1) {

    int ret = poll(fds, 2, -1);

    if (ret == -1) {
      perror("poll");
      break;
    }

    struct pollfd *active_fds = NULL;
    active_fds = &fds[sm->active_index+1];
    if (active_fds == NULL) continue;

    /*
     * PTY died
     */
    if (active_fds->revents & (POLLHUP | POLLERR)) {
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

      ssize_t written =
          session_write_pty(&sm->sessions[sm->active_index], buffer, n);

      if (written <= 0)
        break;
    }

    /*
     * pty -> stdout
     */
    if (active_fds->revents & POLLIN) {
      ssize_t n = session_read_pty(&sm->sessions[sm->active_index], buffer,
                                   sizeof(buffer));

      if (n <= 0)
        break;

      ssize_t written = write(STDOUT_FILENO, buffer, n);

      if (written <= 0)
        break;
    }
  }
}

void ctrlB_command() {
  // Esperar por el segundo comando despues de CTRL+break

  char command[3];
  ssize_t n = read(STDIN_FILENO, command, sizeof(command));

  if (n <= 0)
    return;

  if (command[0] == 'c')
    printf("Crear otra session\r\n");
  else
    printf("CTRL+B + %c command not defined\r\n", command[0]);
}

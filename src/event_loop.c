#include "event_loop.h"

#include <poll.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER 4096

static int ctrlB_command(SessionManager *sm);

void event_loop_run(SessionManager *sm) {

  char buffer[MAX_BUFFER];

  while (1) {

    /* POLL STRUCT */
    struct pollfd *fds = malloc((sm->count + 1) * sizeof(struct pollfd));
    if (fds == NULL) {
      perror("Eror al hacer malloc para 'fds'");
      break;
    }

    // stdin
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    // pty
    for (size_t i = 1; i <= sm->count; i++) {
      fds[i].fd = sm->sessions[i - 1].pty.master_fd;
      fds[i].events = POLLIN;
    }

    /* POLL */
    int ret = poll(fds, sm->count + 1, -1);
    if (ret == -1) {
      perror("poll");
      break;
    }

    struct pollfd *active_fds = NULL;
    active_fds = &fds[sm->active_index + 1];

    /*
     * PTY died
     */
    if (active_fds->revents & (POLLHUP | POLLERR)) {
      session_destroy(sm, sm->sessions[sm->active_index].id);
      if (sm->count > 0)
        continue;
      else
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
        ctrlB_command(sm);
        // session_write_pty(&sm->sessions[sm->active_index], "\r\n", 2);
        free(fds);
        continue;
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

    free(fds);
  }
}

static int ctrlB_command(SessionManager *sm) {
  // Esperar por el segundo comando despues de CTRL+break

  char command[3];
  ssize_t n = read(STDIN_FILENO, command, sizeof(command));

  char *msg = calloc(256, sizeof(char));

  if (n <= 0)
    return 0;

  if (command[0] == 'c') {
    sprintf(msg, "Crear otra session\r\n");
    write(STDOUT_FILENO, msg, strlen(msg));

    if (session_create(sm) == -1) {
      sprintf(msg, "Error al crear nueva sesion\r\n");
      write(STDOUT_FILENO, msg, strlen(msg));
      perror("Error al crear nueva sesion");
      free(msg);
      return -1;
    }
  } else {
    sprintf(msg, "CTRL+B + %c command not defined\r\n", command[0]);
    write(STDOUT_FILENO, msg, strlen(msg));
  }

  free(msg);
  return 0;
}

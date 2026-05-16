#include <poll.h>
#include <pty.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>


int main() {

  int master_fd;
  char ptyName[256];

  pid_t pid = forkpty(&master_fd, ptyName, NULL, NULL);
  if (pid == -1) {
    perror("forkpty failed");
    return -1;
  }

  // Despues del fork, el programa hijo y el padre ejecutan
  // el siguiente codigo:

  if (pid > 0)
    printf("PID hijo visto por el padre = %0d\n", pid);
  else
    printf("PID hijo visto por el hijo = %0d\n", pid);

  // Programa que ejecutará el hijo (pty)
  if (pid == 0) {
    execl("/bin/bash", "bash", (char *)NULL);
    // Si llegamos aquí, execl falló
    perror("execl failed");
    exit(1);
  }

  // Padre: leer del PTY y mostrar en stdout
  // Padre: leer del stdin y mandar a PTY
  puts("Entrando a PTY");

  struct pollfd fds[2];
  // Obervar si hay algo para leer en stdin
  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;
  // Observar si hay algo para leer en pty
  fds[1].fd = master_fd;
  fds[1].events = POLLIN;

  const int MAX_BUFFER = 1024;
  char buffer[MAX_BUFFER];
  size_t n;

  while (1) {

    int p = poll(fds, 2, -1);

    if (p == -1) {
      perror("poll failed");
      break;
    }

    // stdin -> pty
    if (fds[0].revents & POLLIN) {
      n = read(STDIN_FILENO, buffer, MAX_BUFFER);
      if (n > 0)
        write(master_fd, buffer, n);
    }

    // pty -> stdout
    if (fds[1].revents & POLLIN) {
      n = read(master_fd, buffer, MAX_BUFFER);
      if (n > 0)
        write(STDOUT_FILENO, buffer, n);
    }
  }

  // Padre: esperar al hijo y luego cerrar
  int status;
  waitpid(pid, &status, 0);
  close(master_fd);

  return 0;
}

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "mini_pty.h"
#include "mini_termios.h"
#include "event_loop.h"


int main() {

  /**********************/
  /* RAW TERMINAL */
  /**********************/
  term_save_orig();
  atexit(term_restore_orig);

  /**********************/
  /* PTY */
  /**********************/
  Pty pty;

  if (pty_start(&pty) == -1)
    return 1;

  event_loop_run(&pty);

  int status;
  waitpid(pty.child_pid, &status, 0);

  pty_destroy(&pty);

  return 0;
}

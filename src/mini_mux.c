#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "event_loop.h"
#include "mini_termios.h"
#include "session.h"

int main() {

  /**********************/
  /* RAW TERMINAL */
  /**********************/
  term_save_orig();
  atexit(term_restore_orig);

  term_enter_raw_mode();

  /**********************/
  /* Session */
  /**********************/
  SessionManager *sm = NULL;

  if (session_manager_init(&sm) == -1)
    return 1;

  if (session_create(sm) == -1)
    return 1;

  event_loop_run(sm);

  session_manager_destroy(sm);

  puts("Saliendo de mini-tmux");
  return 0;
}

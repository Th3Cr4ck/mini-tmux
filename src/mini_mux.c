#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#include "session.h"
#include "mini_termios.h"
#include "event_loop.h"


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

  event_loop_run(&(sm->sessions[0]));

  session_manager_destroy(sm);

  return 0;
}

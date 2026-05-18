#include "session.h"
#include "mini_pty.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int session_manager_init(SessionManager **sm) {

  *sm = malloc(sizeof(SessionManager));

  if (*sm == NULL) {
    perror("malloc failed");
    return -1;
  }

  (*sm)->sessions = NULL;
  (*sm)->count = 0;
  (*sm)->active_index = -1;
  (*sm)->next_id = 0;

  return 0;
}

void session_manager_destroy(SessionManager *sm) {
  while (sm->count > 0) {
    session_destroy(sm, sm->sessions[0].id);
  }

  free(sm);
}

int session_create(SessionManager *sm) {

  // Create Pty
  Pty new_pty;
  if (pty_start(&new_pty) == -1)
    return -1;

  // Create session
  Session new_session = {new_pty, sm->next_id++};

  // Realloc sessions
  int new_count = sm->count + 1;
  Session *ses_re = realloc(sm->sessions, sizeof(Session) * new_count);
  if (ses_re == NULL) {
    perror("realloc of sessions failed");
    return -1;
  }

  // Add new session
  sm->sessions = ses_re;
  sm->active_index = new_count-1;
  sm->sessions[sm->active_index] = new_session;
  sm->count = new_count;

  return 0;
}

void session_destroy(SessionManager *sm, int id) {

  for (size_t i = 0; i < sm->count; i++) {

    if (sm->sessions[i].id == id) {

      // Set a new active session if destroyed session is the active one
      if (i == sm->active_index) {
        if (sm->count == 1) // If there are no more sessions
          sm->active_index = -1;
        else if (i == 0) // If it is the first session
          sm->active_index++;
        else
          sm->active_index--;
      }

      // Eliminar session
      pty_destroy(&sm->sessions[i].pty);

      // Recorrer las demas sesiones
      for (size_t j = i; j < sm->count - 1; j++) {
        sm->sessions[j] = sm->sessions[j + 1];
      }

      // Realloc sessions
      int new_count = sm->count - 1;

      if (new_count == 0) {
        free(sm->sessions);
        sm->sessions = NULL;
      } else {
        Session *ses_re = realloc(sm->sessions, sizeof(Session) * new_count);
        if (ses_re == NULL) {
          perror("realloc of sessions failed");
          return;
        }
        sm->sessions = ses_re;
      }

      sm->count = new_count;

      return;
    }
  }

  printf("No se encontró session con id=%d\r\n", id);
}

ssize_t session_read_pty(Session *session, char *buffer, size_t size) {
  return pty_read(&(session->pty), buffer, size);
}

ssize_t session_write_pty(Session *session, const char *buffer, size_t size) {
  return pty_write(&(session->pty), buffer, size);
}

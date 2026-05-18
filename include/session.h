#ifndef SESSION_H
#define SESSION_H

#include "mini_pty.h"
#include <sys/types.h>

typedef struct {
  Pty pty;
  int id;
} Session;

typedef struct {
  Session *sessions;
  size_t count;
  size_t active_index;
  int next_id;
} SessionManager;

int session_manager_init(SessionManager **sm);
void session_manager_destroy(SessionManager *sm);
int session_create(SessionManager *sm);
void session_destroy(SessionManager *sm, int id);
ssize_t session_read_pty(Session *session, char *buffer, size_t size);
ssize_t session_write_pty(Session *session, const char *buffer, size_t size);

#endif

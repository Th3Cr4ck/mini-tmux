#include "mini_termios.h"
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void term_save_orig() {
  tcgetattr(STDIN_FILENO, &orig_termios);
}

void term_restore_orig() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

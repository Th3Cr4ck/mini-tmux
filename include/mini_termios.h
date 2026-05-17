#ifndef MINI_TERMIOS_H
#define MINI_TERMIOS_H

void term_save_orig();
void term_restore_orig();
void term_enter_raw_mode();

#endif

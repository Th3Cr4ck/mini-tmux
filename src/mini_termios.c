#include "mini_termios.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

static void print_termios_flags(const struct termios *t, const char *label);

struct termios orig_termios;

void term_save_orig() { tcgetattr(STDIN_FILENO, &orig_termios); }

void term_restore_orig() { 
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
  puts("Exiting raw mode");
}

void term_enter_raw_mode() {
  struct termios t;

  tcgetattr(STDIN_FILENO, &t);

  cfmakeraw(&t);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &t);

  print_termios_flags(&t, "Termios raw mode");
}

static void print_termios_flags(const struct termios *t, const char *label) {
  printf("\r\n=== %s ===\r\n", label);

  // c_iflag - Input modes
  printf("c_iflag (input modes):\r\n");
  printf("  IGNBRK  : %s\r\n", (t->c_iflag & IGNBRK) ? "ON" : "OFF");
  printf("  BRKINT  : %s\r\n", (t->c_iflag & BRKINT) ? "ON" : "OFF");
  printf("  IGNPAR  : %s\r\n", (t->c_iflag & IGNPAR) ? "ON" : "OFF");
  printf("  PARMRK  : %s\r\n", (t->c_iflag & PARMRK) ? "ON" : "OFF");
  printf("  INPCK   : %s\r\n", (t->c_iflag & INPCK) ? "ON" : "OFF");
  printf("  ISTRIP  : %s\r\n", (t->c_iflag & ISTRIP) ? "ON" : "OFF");
  printf("  INLCR   : %s\r\n", (t->c_iflag & INLCR) ? "ON" : "OFF");
  printf("  IGNCR   : %s\r\n", (t->c_iflag & IGNCR) ? "ON" : "OFF");
  printf("  ICRNL   : %s\r\n", (t->c_iflag & ICRNL) ? "ON" : "OFF");
  printf("  IUCLC   : %s\r\n", (t->c_iflag & IUCLC) ? "ON" : "OFF");
  printf("  IXON    : %s\r\n", (t->c_iflag & IXON) ? "ON" : "OFF");
  printf("  IXANY   : %s\r\n", (t->c_iflag & IXANY) ? "ON" : "OFF");
  printf("  IXOFF   : %s\r\n", (t->c_iflag & IXOFF) ? "ON" : "OFF");
  printf("  IMAXBEL : %s\r\n", (t->c_iflag & IMAXBEL) ? "ON" : "OFF");
  printf("  IUTF8   : %s\r\n", (t->c_iflag & IUTF8) ? "ON" : "OFF");

  // c_oflag - Output modes
  printf("\r\nc_oflag (output modes):\r\n");
  printf("  OPOST   : %s\r\n", (t->c_oflag & OPOST) ? "ON" : "OFF");
  printf("  OLCUC   : %s\r\n", (t->c_oflag & OLCUC) ? "ON" : "OFF");
  printf("  ONLCR   : %s\r\n", (t->c_oflag & ONLCR) ? "ON" : "OFF");
  printf("  OCRNL   : %s\r\n", (t->c_oflag & OCRNL) ? "ON" : "OFF");
  printf("  ONOCR   : %s\r\n", (t->c_oflag & ONOCR) ? "ON" : "OFF");
  printf("  ONLRET  : %s\r\n", (t->c_oflag & ONLRET) ? "ON" : "OFF");
  printf("  OFILL   : %s\r\n", (t->c_oflag & OFILL) ? "ON" : "OFF");
  printf("  OFDEL   : %s\r\n", (t->c_oflag & OFDEL) ? "ON" : "OFF");
  printf("  NLDLY   : %d\r\n", t->c_oflag & NLDLY);
  printf("  CRDLY   : %d\r\n", t->c_oflag & CRDLY);
  printf("  TABDLY  : %d\r\n", t->c_oflag & TABDLY);
  printf("  BSDLY   : %d\r\n", t->c_oflag & BSDLY);
  printf("  VTDLY   : %d\r\n", t->c_oflag & VTDLY);
  printf("  FFDLY   : %d\r\n", t->c_oflag & FFDLY);

  // c_cflag - Control modes
  printf("\r\nc_cflag (control modes):\r\n");
  printf("  CSIZE   : %d\r\n", t->c_cflag & CSIZE);
  printf("  CS5     : %s\r\n", (t->c_cflag & CS5) ? "ON" : "OFF");
  printf("  CS6     : %s\r\n", (t->c_cflag & CS6) ? "ON" : "OFF");
  printf("  CS7     : %s\r\n", (t->c_cflag & CS7) ? "ON" : "OFF");
  printf("  CS8     : %s\r\n", (t->c_cflag & CS8) ? "ON" : "OFF");
  printf("  CSTOPB  : %s\r\n", (t->c_cflag & CSTOPB) ? "ON" : "OFF");
  printf("  CREAD   : %s\r\n", (t->c_cflag & CREAD) ? "ON" : "OFF");
  printf("  PARENB  : %s\r\n", (t->c_cflag & PARENB) ? "ON" : "OFF");
  printf("  PARODD  : %s\r\n", (t->c_cflag & PARODD) ? "ON" : "OFF");
  printf("  HUPCL   : %s\r\n", (t->c_cflag & HUPCL) ? "ON" : "OFF");
  printf("  CLOCAL  : %s\r\n", (t->c_cflag & CLOCAL) ? "ON" : "OFF");
  // printf("  CCTS_OFLOW : %s\r\n", (t->c_cflag & CCTS_OFLOW) ? "ON" : "OFF");
  printf("  CRTSCTS : %s\r\n", (t->c_cflag & CRTSCTS) ? "ON" : "OFF");
  // printf("  CRTS_IFLOW : %s\r\n", (t->c_cflag & CRTS_IFLOW) ? "ON" : "OFF");
  // printf("  MDMBUF  : %s\r\n", (t->c_cflag & MDMBUF) ? "ON" : "OFF");

  // c_lflag - Local modes
  printf("\r\nc_lflag (local modes):\r\n");
  printf("  ISIG    : %s\r\n", (t->c_lflag & ISIG) ? "ON" : "OFF");
  printf("  ICANON  : %s\r\n", (t->c_lflag & ICANON) ? "ON" : "OFF");
  printf("  XCASE   : %s\r\n", (t->c_lflag & XCASE) ? "ON" : "OFF");
  printf("  ECHO    : %s\r\n", (t->c_lflag & ECHO) ? "ON" : "OFF");
  printf("  ECHOE   : %s\r\n", (t->c_lflag & ECHOE) ? "ON" : "OFF");
  printf("  ECHOK   : %s\r\n", (t->c_lflag & ECHOK) ? "ON" : "OFF");
  printf("  ECHONL  : %s\r\n", (t->c_lflag & ECHONL) ? "ON" : "OFF");
  printf("  NOFLSH  : %s\r\n", (t->c_lflag & NOFLSH) ? "ON" : "OFF");
  printf("  TOSTOP  : %s\r\n", (t->c_lflag & TOSTOP) ? "ON" : "OFF");
  printf("  ECHOCTL : %s\r\n", (t->c_lflag & ECHOCTL) ? "ON" : "OFF");
  printf("  ECHOPRT : %s\r\n", (t->c_lflag & ECHOPRT) ? "ON" : "OFF");
  printf("  ECHOKE  : %s\r\n", (t->c_lflag & ECHOKE) ? "ON" : "OFF");
  printf("  FLUSHO  : %s\r\n", (t->c_lflag & FLUSHO) ? "ON" : "OFF");
  printf("  PENDIN  : %s\r\n", (t->c_lflag & PENDIN) ? "ON" : "OFF");
  printf("  IEXTEN  : %s\r\n", (t->c_lflag & IEXTEN) ? "ON" : "OFF");
  printf("  EXTPROC : %s\r\n", (t->c_lflag & EXTPROC) ? "ON" : "OFF");

  // c_cc - Special characters
  printf("\r\nc_cc (special characters):\r\n");
  printf("  VINTR   : %d (0x%02x)\r\n", t->c_cc[VINTR], t->c_cc[VINTR]);
  printf("  VQUIT   : %d (0x%02x)\r\n", t->c_cc[VQUIT], t->c_cc[VQUIT]);
  printf("  VERASE  : %d (0x%02x)\r\n", t->c_cc[VERASE], t->c_cc[VERASE]);
  printf("  VKILL   : %d (0x%02x)\r\n", t->c_cc[VKILL], t->c_cc[VKILL]);
  printf("  VEOF    : %d (0x%02x)\r\n", t->c_cc[VEOF], t->c_cc[VEOF]);
  printf("  VTIME   : %d (0x%02x)\r\n", t->c_cc[VTIME], t->c_cc[VTIME]);
  printf("  VMIN    : %d (0x%02x)\r\n", t->c_cc[VMIN], t->c_cc[VMIN]);
  printf("  VSWTC   : %d (0x%02x)\r\n", t->c_cc[VSWTC], t->c_cc[VSWTC]);
  printf("  VSTART  : %d (0x%02x)\r\n", t->c_cc[VSTART], t->c_cc[VSTART]);
  printf("  VSTOP   : %d (0x%02x)\r\n", t->c_cc[VSTOP], t->c_cc[VSTOP]);
  printf("  VSUSP   : %d (0x%02x)\r\n", t->c_cc[VSUSP], t->c_cc[VSUSP]);
  printf("  VEOL    : %d (0x%02x)\r\n", t->c_cc[VEOL], t->c_cc[VEOL]);
  printf("  VREPRINT: %d (0x%02x)\r\n", t->c_cc[VREPRINT], t->c_cc[VREPRINT]);
  printf("  VDISCARD: %d (0x%02x)\r\n", t->c_cc[VDISCARD], t->c_cc[VDISCARD]);
  printf("  VWERASE : %d (0x%02x)\r\n", t->c_cc[VWERASE], t->c_cc[VWERASE]);
  printf("  VLNEXT  : %d (0x%02x)\r\n", t->c_cc[VLNEXT], t->c_cc[VLNEXT]);
  printf("  VEOL2   : %d (0x%02x)\r\n", t->c_cc[VEOL2], t->c_cc[VEOL2]);
}

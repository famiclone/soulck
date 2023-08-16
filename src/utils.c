#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"

struct termios term;

void die(const char *s) {
  perror(s);
  exit(1);
}

void enableMouse() {
  write(STDOUT_FILENO, "\x1b[?1000h", 8);
  write(STDOUT_FILENO, "\x1b[?1006h", 8);
}

void disableMouse() {
  write(STDOUT_FILENO, "\x1b[?1000l", 8);
  write(STDOUT_FILENO, "\x1b[?1006l", 8);
}

void disableRaw() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1) {
    die("tcsetattr");
  }

  disableMouse();
}

void enableRaw() {
  if (tcgetattr(STDIN_FILENO, &term) == -1)
    die("tcseattr");

  struct termios raw = term;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag &= ~(CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  enableMouse();

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tsceattr");
}

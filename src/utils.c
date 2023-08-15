#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"

void die(const char *s) {
  perror(s);
  exit(1);
}

void disableRaw(struct termios term) {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1) {
    die("tcsetattr");
  }
}

void enableRaw(struct termios term) {
  if (tcgetattr(STDIN_FILENO, &term) == -1)
    die("tcseattr");

  struct termios raw = term;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag &= ~(CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tsceattr");
}

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

enum clientStatus { Offline, Away, Online };

struct clientState {
  enum clientStatus status;
};

struct clientState C;

struct termState {
  struct termios term;
};

struct termState S;

void die(const char *s) {
  perror(s);
  exit(1);
}

void clearScreen() {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &S.term) == -1) {
    die("tcsetattr");
  }
}

void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &S.term) == -1)
    die("tcseattr");
  atexit(disableRawMode);

  struct termios raw = S.term;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag &= ~(CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tsceattr");
}

void keyHandler() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN)
      die("read");
  }

  switch (c) {
  case 'q':
    clearScreen();
    exit(0);
    break;
  default:
    printf("%d\n", c);
    break;
  }
}

void renderText(const char *s, int posX, int posY) {
  char buf[32];
  sprintf(buf, "\x1b[%d;%dH", posX, posY);

  write(STDOUT_FILENO, buf, strlen(buf));
  write(STDOUT_FILENO, s, 13);
}

void init() {
  C.status = Offline;
  const char *s = C.status == Offline ? "Offline" : "Online";

  write(STDOUT_FILENO, "\033[1;1H", 7);
  write(STDOUT_FILENO, s, strlen(s));
}

int main() {
  enableRawMode();

  while (1) {
    clearScreen();

    init();
    keyHandler();
  }

  return 0;
}

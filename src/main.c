#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

enum clientStatus { Offline, Away, Online };

struct clientState {
  enum clientStatus status;
};

struct clientState C;

struct termState {
  struct termios term;
};

struct termState S;

typedef void (*RenderText)(const char *s, int posX, int posY, int bgColor, int color);
typedef void (*ClearScreen)();

typedef struct {
  struct termios term;
  RenderText renderText;
  ClearScreen clearScreen;
} TerminalRenderer;

void clearScreen() {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
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
    disableRaw(S.term);
    exit(0);
    break;
  default:
    printf("%d\n", c);
    break;
  }
}

void renderText(const char *s, int posX, int posY, int bgColor, int color) {
  char buf[32];
  sprintf(buf, "\x1b[%d;%dH", posX, posY);

  write(STDOUT_FILENO, buf, strlen(buf));

  char colorBuf[32];
  sprintf(colorBuf, "\x1b[%dm", bgColor);
  write(STDOUT_FILENO, colorBuf, strlen(colorBuf));
  write(STDOUT_FILENO, s, strlen(s));
  write(STDOUT_FILENO, "\x1b[0m", 4);
}

void hr() {
  renderText("------------------------------------------------------------------------------------------------------------------------------", 2, 1, 40, 43);
}

void init() {
  C.status = Online;
  const char *s = C.status == Offline ? "Offline" : "Online";
  int color = C.status == Offline ? 41 : 42;

  renderText(s, 1, 1, color, 40);
  hr();
}

int main() {
  enableRaw(S.term);

  while (1) {
    clearScreen();

    init();
    keyHandler();
  }

  return 0;
}

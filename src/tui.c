#include "tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clear() {
  write(STDOUT_FILENO, "\x1b[2J", 4);
}

void move(int x, int y) {
  char b[32];
  sprintf(b, "\x1b[%d;%dH", x, y);
  write(STDOUT_FILENO, b, strlen(b));
}

void renderText(const char *s, int x, int y, int bg) {
  move(x, y);

  char colorBuf[32];
  sprintf(colorBuf, "\x1b[%dm", bg);
  write(STDOUT_FILENO, colorBuf, strlen(colorBuf));
  write(STDOUT_FILENO, s, strlen(s));
  write(STDOUT_FILENO, "\x1b[0m", 4);
}

void renderHeader(int status) {
  char buf[32];
  char *statusString = status == 0 ? "Offline" : status == 1 ? "Away" : "Online";
  sprintf(buf, "%s", statusString);
  renderText(buf, 1, 1, 41);
};

void renderTabs(int activeTab) {
  char *tabs[] = {" Transfers ", " Rooms  ", "  Search  ", "  Users  ", "  Browse  "};
  for (int i = 0; i < 5; i++) {
    char *tab = tabs[i];
    int bg = i == activeTab ? 45 : 40;
    renderText(tab, 2, 1 + (i) * 10, bg);
  }
};

void logo(int x, int y) {
  move(x, y);
  write(STDOUT_FILENO, "      #         \n", 32);
  write(STDOUT_FILENO, "     ####       \n", 32);
}

void frame() {
  renderText("┌──────────────────────────────────────────────────────────────────────────────────────────────┐", 3, 1, 0);
}

void renderPage(int activeTab) {
  switch (activeTab) {
    case 0:
      frame();
      break;
    case 1:
      frame();
      break;
    case 2:
      logo(32, 32);
      break;
    case 3:
      frame();
      break;
    case 4:
      frame();
      break;
  }
}

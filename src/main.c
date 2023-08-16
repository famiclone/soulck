#include "tui.h"
#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

typedef struct {
  int activeTab;
  int status;
} State;

TUI tui = {
    .clear = clear,
    .renderText = renderText,
    .renderHeader = renderHeader,
    .renderTabs = renderTabs,
    .renderPage = renderPage
};

State state = {
  .activeTab = 2,
  .status = 0
};

void keyHandler() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }
  switch (c) {
    case '1':
      state.activeTab = 0;
      break;
    case '2':
      state.activeTab = 1;
      break;
    case '3':
      state.activeTab = 2;
      break;
    case '4':
      state.activeTab = 3;
      break;
    case '5':
      state.activeTab = 4;
      break;
    case 'q':
      tui.clear();
      disableRaw();
      exit(0);
      break;
  }
}

int main() {
  enableRaw();

  while (1) {
    tui.clear();

    tui.renderHeader(state.status);
    tui.renderTabs(state.activeTab);
    tui.renderPage(state.activeTab);

    keyHandler();
  }

  return 0;
}

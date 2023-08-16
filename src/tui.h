// TUI tui
// tui.clear();
#include <unistd.h>

#ifndef TUI_H
#define TUI_H

typedef void (*ClearFunc)();
typedef void (*RenderText)(const char *s, int x, int y, int bg);
typedef void (*RenderHeader)(int status);
typedef void (*RenderTabs)(int activeTab);
typedef void (*RenderPage)(int activeTab);

typedef struct {
  ClearFunc clear;
  RenderText renderText;
  RenderHeader renderHeader;
  RenderTabs renderTabs;
  RenderPage renderPage;
} TUI;

void clear();
void renderText(const char *s, int x, int y, int bg);
void renderHeader(int status);
void renderTabs(int activeTab);
void renderPage(int activeTab);

#endif

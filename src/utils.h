#ifndef UTILS_H
#define UTILS_H

void die(const char *s);
void enableRaw(struct termios term);
void disableRaw(struct termios term);

#endif

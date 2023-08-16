#ifndef CONFIG_H
#define CONFIG_H

struct configData {
  char *key;
  char *value;
};

typedef configData (*Load)();

#endif

// modules/args_parser/config.h

#ifndef CONFIG_H
#define CONFIG_H

#include "constants.h"

#include <netinet/in.h> // for htons()
#include <stdbool.h>

typedef struct {
  char hostname[256];
  uint16_t portNumber;
  char gameKindName[128];
} Config;

bool parse_config_file(const char *filename, Config *config);

#endif // CONFIG_H

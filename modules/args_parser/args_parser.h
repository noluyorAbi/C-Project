#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <stdbool.h>

typedef struct {
  char game_id[14];      // 13 characters + null terminator (mandatory)
  int player_number;     // 1 or 2 (optional, default: 1)
  char config_file[256]; // Path to the configuration file (optional, default:
                         // client.conf)
} GameConfig;

bool parse_args(int argc, char *argv[], GameConfig *config);
void print_usage(const char *prog_name);

#endif

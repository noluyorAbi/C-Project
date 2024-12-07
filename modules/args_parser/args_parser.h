// args_parser/args_parser.h

#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <stdbool.h>

// Stores game configuration
typedef struct {
  char game_id[14];  // Game ID
  int player_number; // Player number
} GameConfig;

// Parses command-line arguments
bool parse_args(int argc, char *argv[], GameConfig *config);

// Displays usage information
void print_usage(const char *prog_name);

#endif
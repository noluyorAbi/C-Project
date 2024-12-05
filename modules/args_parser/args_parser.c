// args_parser/args_parser.c

#include "args_parser.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

/**
 * Parses command-line arguments and populates the GameConfig structure.
 */
bool parse_args(int argc, char *argv[], GameConfig *config) {
  if (argc != 5) { // Expect exactly 4 arguments plus the program name
    fprintf(stderr, "Error: Incorrect number of arguments.\n");
    return false;
  }

  // Initialize the structure with empty values
  memset(config->game_id, 0, sizeof(config->game_id));
  config->player_number = 0;

  // Iterate over the command-line arguments to parse them
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-g") == 0) {
      if (i + 1 < argc) {
        // Validate that GAME-ID contains exactly 13 digits
        size_t len = strlen(argv[i + 1]);
        if (len != 13) {
          fprintf(stderr, "Error: GAME-ID must be exactly 13 digits long.\n");
          return false;
        }
        strncpy(config->game_id, argv[i + 1], 13);
        config->game_id[13] = '\0'; // Ensure null terminator
        i++; // Skip the next argument since it has been processed
      } else {
        fprintf(stderr, "Error: -g requires a value.\n");
        return false;
      }
    } else if (strcmp(argv[i], "-p") == 0) {
      if (i + 1 < argc) {
        // Validate that the player number is either 1 or 2
        if (strlen(argv[i + 1]) != 1
            || (argv[i + 1][0] != '1' && argv[i + 1][0] != '2')) {
          fprintf(stderr, "Error: Player number must be either 1 or 2.\n");
          return false;
        }
        config->player_number = argv[i + 1][0] - '0';
        i++; // Skip the next argument since it has been processed
      } else {
        fprintf(stderr, "Error: -p requires a value.\n");
        return false;
      }
    } else {
      fprintf(stderr, "Unknown parameter: %s\n", argv[i]);
      return false;
    }
  }

  // Ensure both parameters are set correctly
  if (strlen(config->game_id) != 13
      || (config->player_number != 1 && config->player_number != 2)) {
    fprintf(stderr, "Error: Invalid or missing arguments.\n");
    return false;
  }

  return true;
}

/**
 * Displays the usage help.
 */
void print_usage(const char *prog_name) {
  printf("Usage: %s -g <GAME-ID> -p <1|2>\n", prog_name);
  printf("  -g <GAME-ID> : 13-digit game ID\n");
  printf("  -p <1|2>     : Player number (1 or 2)\n");
}

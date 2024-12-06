// modules/args_parser/args_parser.c

#include "args_parser.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

/**
 * Parse command-line arguments and populate the GameConfig structure.
 * Only -g is mandatory. -p and -c are optional.
 */
bool parse_args(int argc, char *argv[], GameConfig *config) {
  // Set default values for optional arguments
  config->player_number = 1; // Standardspielerzahl
  strncpy(config->config_file, "client.conf", sizeof(config->config_file) - 1);
  config->config_file[sizeof(config->config_file) - 1] = '\0';

  // Initialize game_id to empty
  memset(config->game_id, 0, sizeof(config->game_id));

  // Iterate through arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-g") == 0) {
      if (i + 1 < argc) {
        // Validate that GAME-ID contains exactly 13 digits
        size_t len = strlen(argv[i + 1]);
        bool valid = true;
        if (len != 13) {
          valid = false;
        } else {
          for (size_t j = 0; j < len; j++) {
            if (!isdigit((unsigned char) argv[i + 1][j])) {
              valid = false;
              break;
            }
          }
        }
        if (!valid) {
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
    } else if (strcmp(argv[i], "-c") == 0) {
      if (i + 1 < argc) {
        strncpy(config->config_file, argv[i + 1],
                sizeof(config->config_file) - 1);
        config->config_file[sizeof(config->config_file) - 1] =
          '\0'; // Ensure null terminator
        i++;    // Skip the next argument since it has been processed
      } else {
        fprintf(stderr, "Error: -c requires a filename.\n");
        return false;
      }
    } else {
      fprintf(stderr, "Unknown parameter: %s\n", argv[i]);
      return false;
    }
  }

  // Ensure that the mandatory parameter -g is set
  if (strlen(config->game_id) != 13) {
    fprintf(stderr,
            "Error: Missing or invalid mandatory argument -g <GAME-ID>.\n");
    return false;
  }

  return true;
}

/**
 * Displays the usage help.
 */
void print_usage(const char *prog_name) {
  printf("Usage: %s -g <GAME-ID> [-p <1|2>] [-c <config_file>]\n", prog_name);
  printf("  -g <GAME-ID>      : 13-digit game ID (mandatory)\n");
  printf(
    "  -p <1|2>          : Player number (1 or 2) (optional, default: 1)\n");
  printf("  -c <config_file>  : Path to configuration file (optional, default: "
         "client.conf)\n");
}

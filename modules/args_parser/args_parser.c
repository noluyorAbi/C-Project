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
  config->player_number = 1; // Default player number
  strncpy(config->config_file, "client.conf", sizeof(config->config_file) - 1);
  config->config_file[sizeof(config->config_file) - 1] = '\0';

  // Initialize game_id to empty
  memset(config->game_id, 0, sizeof(config->game_id));

  // Iterate through arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-g") == 0) {
      if (i + 1 < argc) {
        // Validate that GAME-ID contains exactly 13 characters (now accepts
        // letters)
        size_t len = strlen(argv[i + 1]);
        if (len != 13) {
          fprintf(stderr, "Fehler: GAME-ID muss genau 13 Zeichen lang sein.\n");
          return false;
        }
        strncpy(config->game_id, argv[i + 1], 13);
        config->game_id[13] = '\0'; // Ensure null terminator
        i++; // Skip the next argument since it has been processed
      } else {
        fprintf(stderr, "Fehler: -g benötigt einen Wert.\n");
        return false;
      }
    } else if (strcmp(argv[i], "-p") == 0) {
      if (i + 1 < argc) {
        // Validate that the player number is either 1 or 2
        if (strlen(argv[i + 1]) != 1
            || (argv[i + 1][0] != '1' && argv[i + 1][0] != '2')) {
          fprintf(stderr, "Fehler: Spielerzahl muss entweder 1 oder 2 sein.\n");
          return false;
        }
        config->player_number = argv[i + 1][0] - '0';
        i++; // Skip the next argument since it has been processed
      } else {
        fprintf(stderr, "Fehler: -p benötigt einen Wert.\n");
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
        fprintf(stderr, "Fehler: -c benötigt einen Dateinamen.\n");
        return false;
      }
    } else {
      fprintf(stderr, "Unbekannter Parameter: %s\n", argv[i]);
      return false;
    }
  }

  // Ensure that the mandatory parameter -g is set
  if (strlen(config->game_id) != 13) {
    fprintf(
      stderr,
      "Fehler: Fehlendes oder ungültiges Pflichtargument -g <GAME-ID>.\n");
    return false;
  }

  return true;
}

/**
 * Displays the usage help.
 */
void print_usage(const char *prog_name) {
  printf("Usage: %s -g <GAME-ID> [-p <1|2>] [-c <config_file>]\n", prog_name);
  printf("  -g <GAME-ID>      : 13-character game ID (mandatory)\n");
  printf(
    "  -p <1|2>          : Player number (1 or 2) (optional, default: 1)\n");
  printf("  -c <config_file>  : Path to configuration file (optional, default: "
         "client.conf)\n");
}
// modules/args_parser/config.c

#include "config.h"

#include "constants.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to trim leading and trailing whitespace
static void trim_whitespace(char *str) {
  // Trim leading space
  char *start = str;
  while (isspace((unsigned char) *start))
    start++;
  if (start != str) {
    memmove(str, start, strlen(start) + 1);
  }

  // Trim trailing space
  char *end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char) *end))
    end--;
  *(end + 1) = '\0';
}

// Parses the configuration file and populates the Config structure. Returns
// true on success, false on failure.

bool parse_config_file(const char *filename, Config *config) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Error opening configuration file");
    return false;
  }

  // Set default values using macros
  strncpy(config->hostname, HOSTNAME, sizeof(config->hostname) - 1);
  config->hostname[sizeof(config->hostname) - 1] = '\0';

  config->portNumber = htons(PORTNUMBER);

  strncpy(config->gameKindName, GAMEKINDNAME, sizeof(config->gameKindName) - 1);
  config->gameKindName[sizeof(config->gameKindName) - 1] = '\0';

  char line[512];
  while (fgets(line, sizeof(line), file)) {
    // Remove any newline characters
    line[strcspn(line, "\r\n")] = '\0';

    // Ignore empty lines and comments
    if (line[0] == '\0' || line[0] == '#') {
      continue;
    }

    // Split the line into key and value
    char *equals_sign = strchr(line, '=');
    if (!equals_sign) {
      fprintf(stderr, "Invalid line in config file: %s\n", line);
      fclose(file);
      return false;
    }

    *equals_sign = '\0';
    char *key = line;
    char *value = equals_sign + 1;

    // Trim whitespace
    trim_whitespace(key);
    trim_whitespace(value);

    // Match keys and assign values
    if (strcasecmp(key, "Hostname") == 0) {
      strncpy(config->hostname, value, sizeof(config->hostname) - 1);
      config->hostname[sizeof(config->hostname) - 1] = '\0';
    } else if (strcasecmp(key, "PortNumber") == 0) {
      // Convert to integer and then to network byte order
      int port = atoi(value);
      if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid PortNumber: %s\n", value);
        fclose(file);
        return false;
      }
      config->portNumber = htons((uint16_t) port);
    } else if (strcasecmp(key, "GameKindName") == 0) {
      strncpy(config->gameKindName, value, sizeof(config->gameKindName) - 1);
      config->gameKindName[sizeof(config->gameKindName) - 1] = '\0';
    } else {
      // For extensibility: Add additional parameters here
      fprintf(stderr, "Unknown configuration parameter: %s\n", key);
      // Optional: Ignore unknown parameters or handle them as errors
      fclose(file);
      return false;
    }
  }

  fclose(file);

  // Check if all required parameters are set
  if (config->hostname[0] == '\0') {
    fprintf(stderr, "Hostname not set in configuration file.\n");
    return false;
  }
  if (config->portNumber == 0) {
    fprintf(stderr, "PortNumber not set or invalid in configuration file.\n");
    return false;
  }
  if (config->gameKindName[0] == '\0') {
    fprintf(stderr, "GameKindName not set in configuration file.\n");
    return false;
  }

  return true;
}

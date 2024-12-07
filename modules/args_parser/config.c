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
    perror("Fehler beim Öffnen der Konfigurationsdatei");
    return false;
  }

  // Set default values using macros (only for optional parameters)
  strncpy(config->hostname, HOSTNAME, sizeof(config->hostname) - 1);
  config->hostname[sizeof(config->hostname) - 1] = '\0';

  config->portNumber = htons(PORTNUMBER);

  // Removed default initialization for gameKindName to make it required
  // If a default value is desired, uncomment the following lines and ensure
  // GAMEKINDNAME is defined strncpy(config->gameKindName, GAMEKINDNAME,
  // sizeof(config->gameKindName) - 1);
  // config->gameKindName[sizeof(config->gameKindName) - 1] = '\0';

  // Initialize gameKindName as an empty string to check if it's set in the
  // config file
  config->gameKindName[0] = '\0';

  char line[512];
  int line_number = 0;
  while (fgets(line, sizeof(line), file)) {
    line_number++;

    // Remove any newline characters
    line[strcspn(line, "\r\n")] = '\0';

    // Ignore empty lines and comments
    if (line[0] == '\0' || line[0] == '#') {
      continue;
    }

    // Count the number of '=' in the line
    int equals_count = 0;
    for (char *p = line; *p != '\0'; p++) {
      if (*p == '=') {
        equals_count++;
      }
    }

    if (equals_count != 1) {
      fprintf(stderr, "Ungültiges Format in Zeile %d: %s\n", line_number, line);
      fclose(file);
      return false;
    }

    // Split the line into key and value
    char *equals_sign = strchr(line, '=');
    if (!equals_sign) {
      // This should not happen as we've already checked the number of '='
      fprintf(stderr, "Ungültige Zeile in der Konfigurationsdatei: %s\n", line);
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
      char *endptr;
      long port = strtol(value, &endptr, 10);
      if (*endptr != '\0') {
        fprintf(stderr,
                "Ungültiger PortNumber (nicht-numerisch) in Zeile %d: %s\n",
                line_number, value);
        fclose(file);
        return false;
      }
      if (port <= 0 || port > 65535) {
        fprintf(
          stderr,
          "Ungültiger PortNumber (außerhalb des Bereichs) in Zeile %d: %s\n",
          line_number, value);
        fclose(file);
        return false;
      }
      config->portNumber = htons((uint16_t) port);
    } else if (strcasecmp(key, "GameKindName") == 0) {
      strncpy(config->gameKindName, value, sizeof(config->gameKindName) - 1);
      config->gameKindName[sizeof(config->gameKindName) - 1] = '\0';
    } else {
      // For extensibility: Add additional parameters here
      fprintf(stderr, "Unbekannter Konfigurationsparameter in Zeile %d: %s\n",
              line_number, key);
      // Optional: Ignore unknown parameters or handle them as errors
      fclose(file);
      return false;
    }
  }

  fclose(file);

  // Check if all required parameters are set
  if (config->hostname[0] == '\0') {
    fprintf(stderr, "Hostname ist in der Konfigurationsdatei nicht gesetzt.\n");
    return false;
  }
  if (config->portNumber == 0) {
    fprintf(stderr, "PortNumber ist in der Konfigurationsdatei nicht gesetzt "
                    "oder ungültig.\n");
    return false;
  }
  if (config->gameKindName[0] == '\0') {
    fprintf(stderr,
            "GameKindName ist in der Konfigurationsdatei nicht gesetzt.\n");
    return false;
  }

  return true;
}

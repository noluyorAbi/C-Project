// main.c

#include "./modules/args_parser/args_parser.h"
#include "./modules/args_parser/config.h"
#include "./modules/args_parser/constants.h" // Einbindung der Konstanten, falls benötigt

#include <arpa/inet.h> // Für ntohs()
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  GameConfig game_config;

  // Parse command-line arguments
  if (!parse_args(argc, argv, &game_config)) {
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }

  // Ausgabe der geparsten Kommandozeilenargumente
  printf("GAME-ID: %s\n", game_config.game_id);
  printf("Spielerzahl: %d\n", game_config.player_number);
  printf("Konfigurationsdatei: %s\n", game_config.config_file);

  // Parse the configuration file
  Config app_config;
  if (!parse_config_file(game_config.config_file, &app_config)) {
    fprintf(stderr, "Failed to parse configuration file: %s\n",
            game_config.config_file);
    return EXIT_FAILURE;
  }

  // Ausgabe der konfigurierten Parameter
  printf("Hostname: %s\n", app_config.hostname);
  printf(
    "Portnummer: %u\n",
    ntohs(
      app_config.portNumber)); // Konvertiert zurück in Host-Byte-Reihenfolge
  printf("GameKindName: %s\n", app_config.gameKindName);

  // Weitere Nutzung der Konfigurationsparameter, z.B. Verbindungsaufbau zum
  // Game-Server
  // ...

  return EXIT_SUCCESS;
}

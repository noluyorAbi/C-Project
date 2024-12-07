#include "./modules/args_parser/args_parser.h"
#include "./modules/shared_memory/shared_memory.c"

#include <stdio.h>

#define GAMEKINDNAME = "NMMorris"

int main(int argc, char *argv[]) {
  GameConfig config;

  if (!parse_args(argc, argv, &config)) {
    print_usage(argv[0]);
    return 1;
  }
  const int PLAYER_NUMBER = config.player_number;
  const char *GAME_ID = config.game_id;

  printf("GAME-ID: %s\n", GAME_ID);
  printf("Spielerzahl: %d\n", PLAYER_NUMBER);

  return 0;
}
#include "./modules/args_parser/args_parser.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
  GameConfig config;
  if (!parse_args(argc, argv, &config)) {
    print_usage(argv[0]);
    return 1;
  }
  printf("GAME-ID: %s\n", config.game_id);
  printf("Spielerzahl: %d\n", config.player_number);
  return 0;
}

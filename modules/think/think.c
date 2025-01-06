#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int think(char *gameState) {
  printf("Thinker: TEST Received game state:\n%s\n", gameState - sizeof(int));

  return EXIT_SUCCESS;
}

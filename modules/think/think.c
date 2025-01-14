#include "think.h"

#include "../modules/shared_memory/shared_memory.h"
#include "mcts.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PIECES 18 // Maximale Anzahl an Spielsteinen

int think(char *gameState) {
  // Initialisiere den Spielstatus
  GameState state;
  memcpy(state.board, gameState, sizeof(state.board));
  state.player_number = 1; // Oder hole den aktuellen Spieler, falls verfügbar

  // Zeitlimit in Millisekunden für MCTS
  int time_limit_ms = 3000;

  // Führe MCTS aus, um den besten Zug zu finden
  char *best_move = mcts_decide_move(&state, time_limit_ms);

  if (best_move) {
    // Gib den besten Zug aus
    printf("Best move: %s\n", best_move);

    // Optionale Aktion: Zug auf dem Brett ausführen (falls nötig)
    place_piece_on_board(state.board, best_move);

    // Speicher freigeben
    free(best_move);

    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "MCTS konnte keinen Zug finden.\n");
    return EXIT_FAILURE;
  }
}
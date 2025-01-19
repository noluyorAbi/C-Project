#include "think.h"

#include "nmm.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PIECES 18

void log_next_action(const char board[], char myPiece, char opponentPiece,
                     int placedPieces) {
  GameAction action =
    determine_next_action(board, myPiece, opponentPiece, placedPieces);

  switch (action) {
  case SET:
    printf("Next action: set a piece\n");
    break;
  case REMOVE:
    printf("Next action: remove an opponent's piece\n");
    break;
  case MOVE:
    printf("Next action: move a piece\n");
    break;
  case FINISHED:
    printf("Game is over\n");
    break;
  default:
    printf("Unknown action\n");
  }
}

int think(char *gameState) {
  char board[25];
  for (int i = 0; i < 25; i++) {
    board[i] = '+';
  }
  printf("\nThinker: Received game state:\n");

  char positions[MAX_PIECES][10];
  int pieceCount = 0;

  char *line = strtok(gameState, "\n");
  while (line != NULL) {
    if ((strncmp(line, "+ PIECE", 7) == 0) && strlen(line) == 13) {
      if (pieceCount < MAX_PIECES) {
        strncpy(positions[pieceCount], line + 7,
                sizeof(positions[pieceCount]) - 1);
        positions[pieceCount][sizeof(positions[pieceCount]) - 1] = '\0';
        pieceCount++;
      } else {
        fprintf(stderr, "Error: Too many PIECE entries!\n");
        break;
      }
    }

    line = strtok(NULL, "\n");
  }

  for (int i = 0; i < pieceCount; i++) {
    place_piece_on_board(board, positions[i]);
  }

  printf("\n"
         " %c--------%c--------%c\n"
         " |        |        |\n"
         " |  %c-----%c-----%c  |\n"
         " |  |     |     |  |\n"
         " |  |   %c-%c-%c   |  |\n"
         " |  |   |   |   |  |\n"
         " %c--%c---%c   %c---%c--%c\n"
         " |  |   |   |   |  |\n"
         " |  |   %c-%c-%c   |  |\n"
         " |  |     |     |  |\n"
         " |  %c-----%c-----%c  |\n"
         " |        |        |\n"
         " %c--------%c--------%c\n"
         "\n",
         board[0], board[1], board[2], board[3], board[4], board[5], board[6],
         board[7], board[8], board[9], board[10], board[11], board[12],
         board[13], board[14], board[15], board[16], board[17], board[18],
         board[19], board[20], board[21], board[22], board[23]);

  log_next_action(board, 'O', 'X', pieceCount);

  char *move = find_next_free_spot(board);
  if (move == NULL) {
    fprintf(stderr, "Thinker: No free spots left!\n");
    return EXIT_FAILURE;
  }

  if (write(pipe_fd[1], move, strlen(move)) == -1) {
    fprintf(stderr, "Thinker: Failed to write to the pipe.\n");
    return EXIT_FAILURE;
  }

  fprintf(stdout, "Thinker: Wrote '%s' to the pipe successfully.\n", move);
  return EXIT_SUCCESS;
}

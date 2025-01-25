#include "nmm.h"

#include <stdbool.h>

int count_pieces(const char *board, char symbol) {
  int count = 0;
  for (int i = 0; i < 24; i++) {
    if (board[i] == symbol)
      count++;
  }
  return count;
}

bool is_game_over(const char board[], char myPiece, char opponentPiece) {
  int myPieceCount = count_pieces(board, myPiece);
  int opponentPieceCount = count_pieces(board, opponentPiece);

  if (myPieceCount < 3 || opponentPieceCount < 3) {
    return true;
  }

  for (int i = 0; i < MAX_POSITIONS; i++) {
    if (board[i] == myPiece) {
      for (int j = 0; j < MAX_POSITIONS; j++) {
        if (board[j] == '+') {
          return false;
        }
      }
    }
  }

  return true;
}

GameAction determine_next_action(int placedPieces, int moveCounter) {
  if (placedPieces < 9 && moveCounter == 0) {
    return SET;
  }

  return MOVE;
}
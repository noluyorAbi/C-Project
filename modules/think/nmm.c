#include "nmm.h"

#include <stdbool.h>

int count_pieces(const char board[], char piece) {
  int count = 0;
  for (int i = 0; i < MAX_POSITIONS; i++) {
    if (board[i] == piece) {
      count++;
    }
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

GameAction determine_next_action(const char board[], char myPiece,
                                 char opponentPiece, int placedPieces) {
  if (is_game_over(board, myPiece, opponentPiece)) {
    return FINISHED;
  }

  if (placedPieces < 9) {
    return SET;
  }

  int myPieceCount = count_pieces(board, myPiece);

  if (myPieceCount > 3) {
    return MOVE;
  } else if (myPieceCount == 3) {
    return MOVE;
  }

  for (int i = 0; i < MAX_POSITIONS; i++) {
    if (board[i] == opponentPiece && !is_in_mill(board, i)) {
      return REMOVE;
    }
  }

  return FINISHED;
}

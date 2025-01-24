#include "nmm.h"

#include <stdbool.h>
#include <stdio.h>

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

static int just_formed_mill(const char board[], char myPiece,
                            char opponentPiece) {
  const int mills[][3] = {
    {0, 1, 2},    {3, 4, 5},    {6, 7, 8},    {9, 10, 11},
    {12, 13, 14}, {15, 16, 17}, {18, 19, 20}, {21, 22, 23},
    {0, 9, 21},   {3, 10, 18},  {6, 11, 15},  {1, 4, 7},
    {16, 19, 22}, {8, 12, 17},  {5, 13, 20},  {2, 14, 23}};

  for (int i = 0; i < 16; i++) {
    int a = mills[i][0], b = mills[i][1], c = mills[i][2];
    if (board[a] == myPiece && board[b] == myPiece && board[c] == myPiece) {
      return 1;
    }
  }
  return 0;
}

GameAction determine_next_action(const char board[], char myPiece,
                                 char opponentPiece, int placedPieces) {
  if (just_formed_mill(board, myPiece, opponentPiece)) {
    return REMOVE;
  }

  printf("Placed pieces: %d\n", placedPieces);
  if (placedPieces < 9) {
    return SET;
  }

  return MOVE;
}
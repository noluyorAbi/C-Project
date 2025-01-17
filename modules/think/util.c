#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define false 0
#define true 1
typedef int bool;

void place_piece_on_board(char board[], const char *position) {
  if (strlen(position) < 2) {
    printf("Invalid position string '%s'.\n", position);
    return;
  }

  char prefix = position[strlen(position) - 6];
  char row = position[strlen(position) - 2];
  char col = position[strlen(position) - 1];

  int base_index;

  switch (row) {
  case 'A':
    base_index = 0;
    break;
  case 'B':
    base_index = 3;
    break;
  case 'C':
    base_index = 6;
    break;
  default:
    printf("Invalid row character '%c' in position '%s'.\n", row, position);
    return;
  }

  int col_index = col - '0';
  if (col_index < 0 || col_index > 7) {
    printf("Invalid column character '%c' in position '%s'.\n", col, position);
    return;
  }

  int board_index = base_index + col_index;

  if (prefix == '0') {
    board[board_index] = 'O';
  } else if (prefix == '1') {
    board[board_index] = 'X';
  } else {
    printf("Invalid prefix character '%c' in position '%s'.\n", prefix,
           position);
  }
}

int is_position_free(const char board[], char row, char col) {
  int base_index;

  switch (row) {
  case 'A':
    base_index = 0;
    break;
  case 'B':
    base_index = 3;
    break;
  case 'C':
    base_index = 6;
    break;
  default:
    return 0;
  }

  int col_index = col - '0';
  if (col_index < 0 || col_index > 7) {
    return 0;
  }

  int board_index = base_index + col_index;
  return (board[board_index] == '+');
}

char *find_next_free_spot(const char board[]) {
  static char move[16];

  char row;
  int col_num;
  char col;

  for (row = 'A'; row <= 'C'; row++) {
    for (col_num = 0; col_num <= 7; col_num++) {
      col = (char) ('0' + col_num);
      if (is_position_free(board, row, col)) {
        snprintf(move, sizeof(move), "PLAY %c%c\n", row, col);
        return move;
      }
    }
  }

  return NULL;
}

int check_miller(const char board[], char last_row, char last_col,
                 char player_piece) {
  static const int mills[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6},
                                  {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

  int base_index;
  switch (last_row) {
  case 'A':
    base_index = 0;
    break;
  case 'B':
    base_index = 3;
    break;
  case 'C':
    base_index = 6;
    break;
  default:
    return 0;
  }

  int col_index = last_col - '0';
  if (col_index < 0 || col_index > 7) {
    return 0;
  }

  int last_move_index = base_index + col_index;

  for (int i = 0; i < 8; i++) {
    int mill[3] = {mills[i][0], mills[i][1], mills[i][2]};

    int last_move_in_mill = 0;
    for (int j = 0; j < 3; j++) {
      if (mill[j] == last_move_index) {
        last_move_in_mill = 1;
        break;
      }
    }

    if (last_move_in_mill) {
      int is_mill = 1;
      for (int j = 0; j < 3; j++) {
        if (board[mill[j]] != player_piece) {
          is_mill = 0;
          break;
        }
      }

      if (is_mill) {
        return 1;
      }
    }
  }

  return 0;
}

static const int all_mills[][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6},
                                   {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

int is_in_mill(const char board[], int index) {
  if (board[index] == '+' || board[index] == 'C') {
    return 0;
  }

  char myPiece = board[index];

  int num_mills = sizeof(all_mills) / sizeof(all_mills[0]);
  for (int i = 0; i < num_mills; i++) {
    if (all_mills[i][0] == index || all_mills[i][1] == index
        || all_mills[i][2] == index) {
      if (board[all_mills[i][0]] == myPiece && board[all_mills[i][1]] == myPiece
          && board[all_mills[i][2]] == myPiece) {
        return 1;
      }
    }
  }
  return 0;
}

int check_removable(const char board[], char myPiece, char opponentPiece) {
  int candidate_mill = -1;
  int candidate_non_mill = -1;

  for (int i = 0; i < 25; i++) {
    if (board[i] == opponentPiece) {
      if (is_in_mill(board, i)) {
        if (candidate_mill < 0) {
          candidate_mill = i;
        }
      } else {
        candidate_non_mill = i;
        break;
      }
    }
  }

  if (candidate_non_mill >= 0) {
    return candidate_non_mill;
  }

  if (candidate_mill >= 0) {
    return candidate_mill;
  }

  return -1;
}

void remove_stone(char board[], int index) {
  if (index < 0 || index >= 25) {
    fprintf(stderr, "remove_stone: Ungültiger Index: %d\n", index);
    return;
  }
  if (board[index] == 'X' || board[index] == 'O') {
    board[index] = 'C';
  } else {
    fprintf(stderr,
            "remove_stone: Kein gültiger Stein zum Entfernen an Index %d.\n",
            index);
  }
}
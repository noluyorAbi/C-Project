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

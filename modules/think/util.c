#include "util.h"

#include <stdio.h>
#include <string.h>

int get_board_index(const char *position) {
  size_t len = strlen(position);
  if (len < 2) {
    printf("String '%s' ist zu kurz, um den Buchstaben zu überprüfen.\n",
           position);
    return -1;
  }

  char row = position[len - 2];
  char col = position[len - 1];

  int board_index = -1;

  switch (row) {
  case 'A':
    switch (col) {
    case '0':
      board_index = 0;
      break;
    case '1':
      board_index = 1;
      break;
    case '2':
      board_index = 2;
      break;
    case '3':
      board_index = 14;
      break;
    case '4':
      board_index = 23;
      break;
    case '5':
      board_index = 22;
      break;
    case '6':
      board_index = 21;
      break;
    case '7':
      board_index = 9;
      break;
    default:
      printf("Ungültige Spalte '%c' in Position '%s'.\n", col, position);
      return -1;
    }
    break;
  case 'B':
    switch (col) {
    case '0':
      board_index = 3;
      break;
    case '1':
      board_index = 4;
      break;
    case '2':
      board_index = 5;
      break;
    case '3':
      board_index = 13;
      break;
    case '4':
      board_index = 20;
      break;
    case '5':
      board_index = 19;
      break;
    case '6':
      board_index = 18;
      break;
    case '7':
      board_index = 10;
      break;
    default:
      printf("Ungültige Spalte '%c' in Position '%s'.\n", col, position);
      return -1;
    }
    break;
  case 'C':
    switch (col) {
    case '0':
      board_index = 6;
      break;
    case '1':
      board_index = 7;
      break;
    case '2':
      board_index = 8;
      break;
    case '3':
      board_index = 12;
      break;
    case '4':
      board_index = 17;
      break;
    case '5':
      board_index = 16;
      break;
    case '6':
      board_index = 15;
      break;
    case '7':
      board_index = 11;
      break;
    default:
      printf("Ungültige Spalte '%c' in Position '%s'.\n", col, position);
      return -1;
    }
    break;
  default:
    printf("Ungültige Reihe '%c' in Position '%s'.\n", row, position);
    return -1;
  }

  return board_index;
}

void place_piece_on_board(char board[], const char *position) {
  size_t len = strlen(position);
  if (len < 6) {
    printf("String '%s' ist zu kurz, um den Buchstaben zu überprüfen.\n",
           position);
    return;
  }

  char prefix = position[len - 6];
  int board_index = get_board_index(position);

  if (board_index == -1) {
    return;
  }

  if (prefix == '0') {
    board[board_index] = 'O';
  } else if (prefix == '1') {
    board[board_index] = 'X';
  } else {
    printf("Ungültiger Präfix '%c' in Position '%s'.\n", prefix, position);
  }
}

int is_position_free(const char board[], char row, char col) {
  int base_index = (row - 'A') * 8;

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
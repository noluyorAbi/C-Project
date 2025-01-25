#include "util.h"

#include "../../modules/args_parser/args_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

const char *index_to_position(int index) {
  if (index < 0 || index >= 24) {
    fprintf(stderr, "Invalid index: %d\n", index);
    return NULL;
  }

  static const char *positions[24] = {
    "A0", "A1", "A2", // 0-2
    "B0", "B1", "B2", // 3-5
    "C0", "C1", "C2", // 6-8
    "A7", "B7", "C7", // 9-11
    "C3", "B3", "A3", // 12-14
    "C6", "C5", "C4", // 15-17
    "B6", "B5", "B4", // 18-20
    "A6", "A5", "A4"  // 21-23
  };

  return positions[index];
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

int is_position_free(const int occupiedPositions[], char row, char col) {
  char pos[3];
  pos[0] = row;
  pos[1] = col;
  pos[2] = '\0';

  int board_index = get_board_index(pos);
  if (board_index == -1) {
    return 0;
  }
  return (occupiedPositions[board_index] == 0);
}

static const int all_mills[][3] = {
  {0, 1, 2},    {3, 4, 5},    {6, 7, 8},    {9, 10, 11},
  {12, 13, 14}, {15, 16, 17}, {18, 19, 20}, {21, 22, 23},
  {0, 9, 21},   {3, 10, 18},  {6, 11, 15},  {1, 4, 7},
  {16, 19, 22}, {8, 12, 17},  {5, 13, 20},  {2, 14, 23}};

int is_in_mill(const char board[], int index) {
  if (board[index] != 'X' && board[index] != 'O') {
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

char *find_next_free_spot(const char board[], const int occupiedPositions[]) {
  static char move[16];

  typedef struct {
    char row;
    char col;
    int index;
  } FreePos;

  FreePos freePositions[24];
  int freeCount = 0;

  for (char row = 'A'; row <= 'C'; row++) {
    for (int col_num = 0; col_num <= 7; col_num++) {
      char col = (char) ('0' + col_num);

      if (is_position_free(occupiedPositions, row, col)) {
        char tempStr[3];
        tempStr[0] = row;
        tempStr[1] = col;
        tempStr[2] = '\0';

        int idx = get_board_index(tempStr);
        if (idx >= 0 && idx < 25) {
          freePositions[freeCount].row = row;
          freePositions[freeCount].col = col;
          freePositions[freeCount].index = idx;
          freeCount++;
        }
      }
    }
  }

  if (freeCount == 0) {
    return NULL;
  }

  char tempBoard[25];
  memcpy(tempBoard, board, 25);

  for (int i = 0; i < freeCount; i++) {
    int idx = freePositions[i].index;
    if (can_form_mill(tempBoard, idx, 'O')) {
      snprintf(move, sizeof(move), "PLAY %c%c\n", freePositions[i].row,
               freePositions[i].col);
      return move;
    }
  }

  for (int i = 0; i < freeCount; i++) {
    int idx = freePositions[i].index;
    if (can_form_mill(tempBoard, idx, 'X')) {
      snprintf(move, sizeof(move), "PLAY %c%c\n", freePositions[i].row,
               freePositions[i].col);
      return move;
    }
  }

  snprintf(move, sizeof(move), "PLAY %c%c\n", freePositions[0].row,
           freePositions[0].col);
  return move;
}

int check_removable(const char board[], char myPiece, char opponentPiece) {
  int candidate_mill = -1;
  int candidate_non_mill = -1;

  for (int i = 0; i < 24; i++) {
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

int can_form_mill(char board[], int pos, char myPiece) {
  if (board[pos] != '+') {
    return 0;
  }

  board[pos] = myPiece;
  int result = is_in_mill(board, pos);
  board[pos] = '+';

  return result;
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

int find_opponent_piece(const char board[], char opponentPiece) {
  int indices[24];
  int count = 0;

  for (int i = 0; i < 24; i++) {
    if (board[i] == opponentPiece) {
      indices[count++] = i;
    }
  }

  if (count == 0) {
    return -1;
  }

  int randomIndex = rand() % count;
  return indices[randomIndex];
}

static int my_player_number = -1;

void set_player_number(int player) {
  my_player_number = player;
}

char get_my_symbol(const char *gameState) {
  return (EXTERN_PLAYER_NUMBER == 0) ? 'O' : 'X';
}
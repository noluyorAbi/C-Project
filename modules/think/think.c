#include "think.h"

#include "nmm.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PIECES 1000

static const int adjacency_list[24][4] = {
  // A0 (0)
  {1, 9, -1, -1}, // Adjacent to A1 (1), A7 (9)
  // A1 (1)
  {0, 2, 4, -1}, // Adjacent to A0 (0), A2 (2), B1 (4)
  // A2 (2)
  {1, 14, -1, -1}, // Adjacent to A1 (1), A3 (14)
  // B0 (3)
  {4, 10, -1, -1}, // Adjacent to B1 (4), B7 (10)
  // B1 (4)
  {1, 3, 5, 13}, // Adjacent to A1 (1), B0 (3), B2 (5), B3 (13)
  // B2 (5)
  {4, 13, -1, -1}, // Adjacent to B1 (4), B3 (13)
  // C0 (6)
  {7, 11, -1, -1}, // Adjacent to C1 (7), C7 (11)
  // C1 (7)
  {6, 8, 12, -1}, // Adjacent to C0 (6), C2 (8), C3 (12)
  // C2 (8)
  {7, 12, -1, -1}, // Adjacent to C1 (7), C3 (12)
  // A7 (9)
  {0, 10, 21, -1}, // Adjacent to A0 (0), B7 (10), A6 (21)
  // B7 (10)
  {3, 9, 11, 18}, // Adjacent to B0 (3), A7 (9), C7 (11), B6 (18)
  // C7 (11)
  {6, 10, 15, -1}, // Adjacent to C0 (6), B7 (10), C6 (15)
  // C3 (12)
  {8, 13, 17, -1}, // Adjacent to C2 (8), B3 (13), C4 (17)
  // B3 (13)
  {4, 12, 14, 20}, // Adjacent to B1 (4), C3 (12), A3 (14), B4 (20)
  // A3 (14)
  {2, 13, 23, -1}, // Adjacent to A2 (2), B3 (13), A4 (23)
  // C6 (15)
  {11, 16, -1, -1}, // Adjacent to C7 (11), C5 (16)
  // C5 (16)
  {15, 17, 19, -1}, // Adjacent to C6 (15), C4 (17), B5 (19)
  // C4 (17)
  {12, 16, -1, -1}, // Adjacent to C3 (12), C5 (16)
  // B6 (18)
  {10, 19, -1, -1}, // Adjacent to B7 (10), B5 (19)
  // B5 (19)
  {16, 18, 20, -1}, // Adjacent to C5 (16), B6 (18), B4 (20)
  // B4 (20)
  {13, 19, -1, -1}, // Adjacent to B3 (13), B5 (19)
  // A6 (21)
  {9, 22, -1, -1}, // Adjacent to A7 (9), A5 (22)
  // A5 (22)
  {21, 23, 19, -1}, // Adjacent to A6 (21), A4 (23), B5 (19)
  // A4 (23)
  {14, 22, -1, -1} // Adjacent to A3 (14), A5 (22)
};

int is_valid_position(const char *position) {
  if (strlen(position) != 2)
    return 0;
  char row = position[0];
  char col = position[1];
  return (row >= 'A' && row <= 'C') && (col >= '0' && col <= '7');
}

int move_counter = 0;
static int current_move_number = 0;
static int last_mill_move_number = -1;
static int mill_positions[16][3];
static int num_mills = 0;
static const int all_mills[16][3] = {
  {0, 1, 2},    {3, 4, 5},    {6, 7, 8},    {9, 10, 11},
  {12, 13, 14}, {15, 16, 17}, {18, 19, 20}, {21, 22, 23},
  {0, 9, 21},   {3, 10, 18},  {6, 11, 15},  {1, 4, 7},
  {16, 19, 22}, {8, 12, 17},  {5, 13, 20},  {2, 14, 23}};

int think(char *gameState) {
  //  printf("\n------------\n");
  //  printf("%s\n", gameState);
  //  printf("------------\n");

  char board[25];
  for (int i = 0; i < 25; i++) {
    board[i] = '+';
  }
  printf("\nThinker: Received game state:\n");

  char positions[MAX_PIECES][10];
  int pieceCount = 0;

  char my_piece = get_my_symbol(gameState);
  char opponent_piece = (my_piece == 'X') ? 'O' : 'X';
  printf("DEBUG: My symbol is '%c', opponent is '%c'\n", my_piece,
         opponent_piece);

  char *line = strtok(gameState, "\n");
  while (line != NULL) {
    //    printf("line: %s\n", line);
    //    printf("strncmp(line, \"+ PIECE\", 7) == 0: %d\n",
    //           strncmp(line, "+ PIECE", 7) == 0);
    //    printf("strlen(line) == 13: %d\n", strlen(line) == 13);

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

  int occupiedPositions[25] = {0};
  char occupiedStrings[25][3];

  for (int i = 0; i < pieceCount; i++) {
    size_t len = strlen(positions[i]);
    if (len >= 2) {
      switch (positions[i][len - 6]) {
      case '0':
        switch (positions[i][len - 2]) {
        case 'A':
          switch (positions[i][len - 1]) {
          case '0':
            board[0] = 'O';
            occupiedPositions[0] = 1;
            strncpy(occupiedStrings[0], "A0", 3);
            break;
          case '1':
            board[1] = 'O';
            occupiedPositions[1] = 1;
            strncpy(occupiedStrings[1], "A1", 3);
            break;
          case '2':
            board[2] = 'O';
            occupiedPositions[2] = 1;
            strncpy(occupiedStrings[2], "A2", 3);
            break;
          case '3':
            board[14] = 'O';
            occupiedPositions[14] = 1;
            strncpy(occupiedStrings[14], "A3", 3);
            break;
          case '4':
            board[23] = 'O';
            occupiedPositions[23] = 1;
            strncpy(occupiedStrings[23], "A4", 3);
            break;
          case '5':
            board[22] = 'O';
            occupiedPositions[22] = 1;
            strncpy(occupiedStrings[22], "A5", 3);
            break;
          case '6':
            board[21] = 'O';
            occupiedPositions[21] = 1;
            strncpy(occupiedStrings[21], "A6", 3);
            break;
          case '7':
            board[9] = 'O';
            occupiedPositions[9] = 1;
            strncpy(occupiedStrings[9], "A7", 3);
            break;
          default:
            break;
          }
          break;
        case 'B':
          switch (positions[i][len - 1]) {
          case '0':
            board[3] = 'O';
            occupiedPositions[3] = 1;
            strncpy(occupiedStrings[3], "B0", 3);
            break;
          case '1':
            board[4] = 'O';
            occupiedPositions[4] = 1;
            strncpy(occupiedStrings[4], "B1", 3);
            break;
          case '2':
            board[5] = 'O';
            occupiedPositions[5] = 1;
            strncpy(occupiedStrings[5], "B2", 3);
            break;
          case '3':
            board[13] = 'O';
            occupiedPositions[13] = 1;
            strncpy(occupiedStrings[13], "B3", 3);
            break;
          case '4':
            board[20] = 'O';
            occupiedPositions[20] = 1;
            strncpy(occupiedStrings[20], "B4", 3);
            break;
          case '5':
            board[19] = 'O';
            occupiedPositions[19] = 1;
            strncpy(occupiedStrings[19], "B5", 3);
            break;
          case '6':
            board[18] = 'O';
            occupiedPositions[18] = 1;
            strncpy(occupiedStrings[18], "B6", 3);
            break;
          case '7':
            board[10] = 'O';
            occupiedPositions[10] = 1;
            strncpy(occupiedStrings[10], "B7", 3);
            break;
          default:
            break;
          }
          break;
        case 'C':
          switch (positions[i][len - 1]) {
          case '0':
            board[6] = 'O';
            occupiedPositions[6] = 1;
            strncpy(occupiedStrings[6], "C0", 3);
            break;
          case '1':
            board[7] = 'O';
            occupiedPositions[7] = 1;
            strncpy(occupiedStrings[7], "C1", 3);
            break;
          case '2':
            board[8] = 'O';
            occupiedPositions[8] = 1;
            strncpy(occupiedStrings[8], "C2", 3);
            break;
          case '3':
            board[12] = 'O';
            occupiedPositions[12] = 1;
            strncpy(occupiedStrings[12], "C3", 3);
            break;
          case '4':
            board[17] = 'O';
            occupiedPositions[17] = 1;
            strncpy(occupiedStrings[17], "C4", 3);
            break;
          case '5':
            board[16] = 'O';
            occupiedPositions[16] = 1;
            strncpy(occupiedStrings[16], "C5", 3);
            break;
          case '6':
            board[15] = 'O';
            occupiedPositions[15] = 1;
            strncpy(occupiedStrings[15], "C6", 3);
            break;
          case '7':
            board[11] = 'O';
            occupiedPositions[11] = 1;
            strncpy(occupiedStrings[11], "C7", 3);
            break;
          default:
            break;
          }
          break;
        default:
          break;
        }
        break;
      case '1':
        switch (positions[i][len - 2]) {
        case 'A':
          switch (positions[i][len - 1]) {
          case '0':
            board[0] = 'X';
            occupiedPositions[0] = 1;
            strncpy(occupiedStrings[0], "A0", 3);
            break;
          case '1':
            board[1] = 'X';
            occupiedPositions[1] = 1;
            strncpy(occupiedStrings[1], "A1", 3);
            break;
          case '2':
            board[2] = 'X';
            occupiedPositions[2] = 1;
            strncpy(occupiedStrings[2], "A2", 3);
            break;
          case '3':
            board[14] = 'X';
            occupiedPositions[14] = 1;
            strncpy(occupiedStrings[14], "A3", 3);
            break;
          case '4':
            board[23] = 'X';
            occupiedPositions[23] = 1;
            strncpy(occupiedStrings[23], "A4", 3);
            break;
          case '5':
            board[22] = 'X';
            occupiedPositions[22] = 1;
            strncpy(occupiedStrings[22], "A5", 3);
            break;
          case '6':
            board[21] = 'X';
            occupiedPositions[21] = 1;
            strncpy(occupiedStrings[21], "A6", 3);
            break;
          case '7':
            board[9] = 'X';
            occupiedPositions[9] = 1;
            strncpy(occupiedStrings[9], "A7", 3);
            break;
          default:
            break;
          }
          break;
        case 'B':
          switch (positions[i][len - 1]) {
          case '0':
            board[3] = 'X';
            occupiedPositions[3] = 1;
            strncpy(occupiedStrings[3], "B0", 3);
            break;
          case '1':
            board[4] = 'X';
            occupiedPositions[4] = 1;
            strncpy(occupiedStrings[4], "B1", 3);
            break;
          case '2':
            board[5] = 'X';
            occupiedPositions[5] = 1;
            strncpy(occupiedStrings[5], "B2", 3);
            break;
          case '3':
            board[13] = 'X';
            occupiedPositions[13] = 1;
            strncpy(occupiedStrings[13], "B3", 3);
            break;
          case '4':
            board[20] = 'X';
            occupiedPositions[20] = 1;
            strncpy(occupiedStrings[20], "B4", 3);
            break;
          case '5':
            board[19] = 'X';
            occupiedPositions[19] = 1;
            strncpy(occupiedStrings[19], "B5", 3);
            break;
          case '6':
            board[18] = 'X';
            occupiedPositions[18] = 1;
            strncpy(occupiedStrings[18], "B6", 3);
            break;
          case '7':
            board[10] = 'X';
            occupiedPositions[10] = 1;
            strncpy(occupiedStrings[10], "B7", 3);
            break;
          default:
            break;
          }
          break;
        case 'C':
          switch (positions[i][len - 1]) {
          case '0':
            board[6] = 'X';
            occupiedPositions[6] = 1;
            strncpy(occupiedStrings[6], "C0", 3);
            break;
          case '1':
            board[7] = 'X';
            occupiedPositions[7] = 1;
            strncpy(occupiedStrings[7], "C1", 3);
            break;
          case '2':
            board[8] = 'X';
            occupiedPositions[8] = 1;
            strncpy(occupiedStrings[8], "C2", 3);
            break;
          case '3':
            board[12] = 'X';
            occupiedPositions[12] = 1;
            strncpy(occupiedStrings[12], "C3", 3);
            break;
          case '4':
            board[17] = 'X';
            occupiedPositions[17] = 1;
            strncpy(occupiedStrings[17], "C4", 3);
            break;
          case '5':
            board[16] = 'X';
            occupiedPositions[16] = 1;
            strncpy(occupiedStrings[16], "C5", 3);
            break;
          case '6':
            board[15] = 'X';
            occupiedPositions[15] = 1;
            strncpy(occupiedStrings[15], "C6", 3);
            break;
          case '7':
            board[11] = 'X';
            occupiedPositions[11] = 1;
            strncpy(occupiedStrings[11], "C7", 3);
            break;
          default:
            break;
          }
          break;
        default:
          break;
        }
        break;
      default:
        break;
      }
    } else {
      printf("String '%s' ist zu kurz, um den Buchstaben zu überprüfen.\n",
             positions[i]);
    }
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

  //  printf("--------------------\n");
  //  for (int i = 0; i < 25; i++) {
  //    if (occupiedPositions[i] == 1) {
  //      printf("Occupied position: %s\n", occupiedStrings[i]);
  //    }
  //  }
  //  printf("--------------------\n");

  // get my piece prefix

  int numberOfMyPieces = count_pieces(board, my_piece);

  int previous_mills[num_mills][3];
  memcpy(previous_mills, mill_positions, sizeof(previous_mills));
  int previous_num_mills = num_mills;
  num_mills = 0;

  for (int i = 0; i < 16; i++) {
    int a = all_mills[i][0];
    int b = all_mills[i][1];
    int c = all_mills[i][2];

    if (board[a] == my_piece && board[b] == my_piece && board[c] == my_piece) {
      int sorted[3] = {a, b, c};

      if (sorted[0] > sorted[1]) {
        int tmp = sorted[0];
        sorted[0] = sorted[1];
        sorted[1] = tmp;
      }
      if (sorted[1] > sorted[2]) {
        int tmp = sorted[1];
        sorted[1] = sorted[2];
        sorted[2] = tmp;
      }
      if (sorted[0] > sorted[1]) {
        int tmp = sorted[0];
        sorted[0] = sorted[1];
        sorted[1] = tmp;
      }

      mill_positions[num_mills][0] = sorted[0];
      mill_positions[num_mills][1] = sorted[1];
      mill_positions[num_mills][2] = sorted[2];
      num_mills++;
    }
  }

  int new_mill_formed = 0;
  for (int i = 0; i < num_mills; i++) {
    int is_new = 1;
    for (int j = 0; j < previous_num_mills; j++) {
      if (mill_positions[i][0] == previous_mills[j][0]
          && mill_positions[i][1] == previous_mills[j][1]
          && mill_positions[i][2] == previous_mills[j][2]) {
        is_new = 0;
        break;
      }
    }

    if (is_new) {
      new_mill_formed = 1;
      last_mill_move_number = current_move_number;
      break;
    }
  }

  printf("new_mill_formed: %d\n", new_mill_formed);

  GameAction action;
  if (new_mill_formed) {
    action = REMOVE;
  } else {
    action = determine_next_action(numberOfMyPieces, move_counter);
  }

  current_move_number++;

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

  if (action == SET) {
    char *move = find_next_free_spot(board, occupiedPositions);
    if (move == NULL) {
      fprintf(stderr, "Thinker: No free spots left!\n");
      return EXIT_FAILURE;
    }

    if (write(pipe_fd[1], move, strlen(move)) == -1) {
      fprintf(stderr, "Thinker: Failed to write to the pipe.\n");
      return EXIT_FAILURE;
    }
  } else if (action == REMOVE) {
    int remove_index = find_opponent_piece(board, opponent_piece);
    if (remove_index == -1) {
      fprintf(stderr, "Thinker: No removable opponent pieces found.\n");
      return EXIT_FAILURE;
    }
    const char *pos = index_to_position(remove_index);
    if (!pos) {
      fprintf(stderr, "Thinker: Invalid remove index %d.\n", remove_index);
      return EXIT_FAILURE;
    }
    char command[16];
    snprintf(command, sizeof(command), "PLAY %s\n", pos);
    if (write(pipe_fd[1], command, strlen(command)) == -1) {
      fprintf(stderr, "Thinker: Failed to write to the pipe.\n");
      return EXIT_FAILURE;
    }
    fprintf(stdout, "Thinker: Wrote '%s' to the pipe successfully.\n", command);
    return EXIT_SUCCESS;
  } else if (action == MOVE) {
    move_counter++;
    struct Move {
      int from;
      int to;
    };

    struct Move moves[100];
    int moveCount = 0;

    int canFly = (numberOfMyPieces == 3);

    for (int i = 0; i < 24; i++) {
      if (board[i] == my_piece) {
        if (canFly) {
          for (int j = 0; j < 24; j++) {
            if (board[j] == '+') {
              moves[moveCount].from = i;
              moves[moveCount].to = j;
              moveCount++;
              if (moveCount >= 100)
                break;
            }
          }
        } else {
          for (int j = 0; j < 4; j++) {
            int adj = adjacency_list[i][j];
            if (adj == -1)
              break;
            if (adj < 0 || adj >= 24)
              continue;
            if (board[adj] == '+') {
              moves[moveCount].from = i;
              moves[moveCount].to = adj;
              moveCount++;
              if (moveCount >= 100)
                break;
            }
          }
        }
        if (moveCount >= 100)
          break;
      }
    }

    if (moveCount == 0) {
      fprintf(stderr, "Thinker: No moves available.\n");
      return EXIT_FAILURE;
    }

    // Prioritize moves that form a mill
    int selected_move = -1;
    for (int i = 0; i < moveCount; i++) {
      char tempBoard[24];
      memcpy(tempBoard, board, 24);
      tempBoard[moves[i].from] = '+';
      tempBoard[moves[i].to] = my_piece;
      if (is_in_mill(tempBoard, moves[i].to)) {
        selected_move = i;
        break;
      }
    }

    if (selected_move == -1) {
      // No mill-forming move found; select a random move (e.g., first
      // available)
      selected_move = 0;
    }

    const char *from_pos = index_to_position(moves[selected_move].from);
    const char *to_pos = index_to_position(moves[selected_move].to);

    if (!from_pos || !to_pos) {
      fprintf(stderr, "Thinker: Invalid move positions.\n");
      return EXIT_FAILURE;
    }

    char command[32];
    snprintf(command, sizeof(command), "PLAY %s:%s\n", from_pos, to_pos);
    if (write(pipe_fd[1], command, strlen(command)) == -1) {
      fprintf(stderr, "Thinker: Failed to write to the pipe.\n");
      return EXIT_FAILURE;
    }
    fprintf(stdout, "Thinker: Wrote '%s' to the pipe successfully.\n", command);
    return EXIT_SUCCESS;
  } else if (action == FINISHED) {
    printf("Game is over\n");
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Unknown action\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
#include "think.h"

#include "nmm.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PIECES 100

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

int is_valid_position(const char *position) {
  if (strlen(position) != 2)
    return 0;
  char row = position[0];
  char col = position[1];
  return (row >= 'A' && row <= 'C') && (col >= '0' && col <= '7');
}

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

  log_next_action(board, 'O', 'X', pieceCount);

  char *move = find_next_free_spot(board, occupiedPositions);
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

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
    size_t len = strlen(positions[i]);
    if (len >= 2) {
      switch (positions[i][len - 6]) {
      case '0':
        // printf("Der Buchstabe im String '%s' ist '0'\n", positions[i]);
        switch (positions[i][len - 2]) {
        case 'A':
          // printf("Der Buchstabe im String '%s' ist 'A'\n", positions[i]);
          switch (positions[i][len - 1]) {
          case '0':
            // printf("Wert ist A0\n");
            board[0] = 'O';
            break;
          case '1':
            // printf("Wert ist A1\n");
            board[1] = 'O';
            break;
          case '2':
            // printf("Wert ist A2\n");
            board[2] = 'O';
            break;
          case '3':
            // printf("Wert ist A3\n");
            board[14] = 'O';
            break;
          case '4':
            // printf("Wert ist A4\n");
            board[23] = 'O';
            break;
          case '5':
            // printf("Wert ist A5\n");
            board[22] = 'O';
            break;
          case '6':
            // printf("Wert ist A6\n");
            board[21] = 'O';
            break;
          case '7':
            // printf("Wert ist A7\n");
            board[9] = 'O';
            break;
          default:
            // printf("Wert ist nicht zwischen 0 und 7\n");
            break;
          }
          break;
        case 'B':
          // printf("Der Buchstabe im String '%s' ist 'B'\n", positions[i]);
          switch (positions[i][len - 1]) {
          case '0':
            // printf("Wert ist B0\n");
            board[3] = 'O';
            break;
          case '1':
            // printf("Wert ist B1\n");
            board[4] = 'O';
            break;
          case '2':
            // printf("Wert ist B2\n");
            board[5] = 'O';
            break;
          case '3':
            // printf("Wert ist B3\n");
            board[13] = 'O';
            break;
          case '4':
            // printf("Wert ist B4\n");
            board[20] = 'O';
            break;
          case '5':
            // printf("Wert ist B5\n");
            board[19] = 'O';
            break;
          case '6':
            // printf("Wert ist B6\n");
            board[18] = 'O';
            break;
          case '7':
            // printf("Wert ist B7\n");
            board[10] = 'O';
            break;
          default:
            // printf("Wert ist nicht zwischen 0 und 7\n");
            break;
          }
          break;
        case 'C':
          // printf("Der Buchstabe im String '%s' ist 'C'\n", positions[i]);
          switch (positions[i][len - 1]) {
          case '0':
            // printf("Wert ist C0\n");
            board[6] = 'O';
            break;
          case '1':
            // printf("Wert ist C1\n");
            board[7] = 'O';
            break;
          case '2':
            // printf("Wert ist C2\n");
            board[8] = 'O';
            break;
          case '3':
            // printf("Wert ist C3\n");
            board[12] = 'O';
            break;
          case '4':
            // printf("Wert ist C4\n");
            board[17] = 'O';
            break;
          case '5':
            // printf("Wert ist C5\n");
            board[16] = 'O';
            break;
          case '6':
            // printf("Wert ist C6\n");
            board[15] = 'O';
            break;
          case '7':
            // printf("Wert ist C7\n");
            board[11] = 'O';
            break;
          default:
            // printf("Wert ist nicht zwischen 0 und 7\n");
            break;
          }
          break;
        default:
          printf("Der Buchstabe im String '%s' ist weder 'A', 'B' noch 'C' (er "
                 "ist '%c')\n",
                 positions[i], positions[i][len - 2]);
          break;
        }
        break;
      case '1':
        // printf("Der Buchstabe im String '%s' ist '1'\n", positions[i]);
        switch (positions[i][len - 2]) {
        case 'A':
          // printf("Der Buchstabe im String '%s' ist 'A'\n", positions[i]);
          switch (positions[i][len - 1]) {
          case '0':
            // printf("Wert ist A0\n");
            board[0] = 'X';

            break;
          case '1':
            // printf("Wert ist A1\n");
            board[1] = 'X';

            break;
          case '2':
            // printf("Wert ist A2\n");
            board[2] = 'X';

            break;
          case '3':
            // printf("Wert ist A3\n");
            board[14] = 'X';

            break;
          case '4':
            // printf("Wert ist A4\n");
            board[23] = 'X';

            break;
          case '5':
            // printf("Wert ist A5\n");
            board[22] = 'X';

            break;
          case '6':
            // printf("Wert ist A6\n");
            board[21] = 'X';

            break;
          case '7':
            // printf("Wert ist A7\n");
            board[9] = 'X';

            break;
          default:
            // printf("Wert ist nicht zwischen 0 und 7\n");
            break;
          }
          break;
        case 'B':
          // printf("Der Buchstabe im String '%s' ist 'B'\n", positions[i]);
          switch (positions[i][len - 1]) {
          case '0':
            // printf("Wert ist B0\n");
            board[3] = 'X';

            break;
          case '1':
            // printf("Wert ist B1\n");
            board[4] = 'X';

            break;
          case '2':
            // printf("Wert ist B2\n");
            board[5] = 'X';

            break;
          case '3':
            // printf("Wert ist B3\n");
            board[13] = 'X';

            break;
          case '4':
            // printf("Wert ist B4\n");
            board[20] = 'X';

            break;
          case '5':
            // printf("Wert ist B5\n");
            board[19] = 'X';

            break;
          case '6':
            // printf("Wert ist B6\n");
            board[18] = 'X';

            break;
          case '7':
            // printf("Wert ist B7\n");
            board[10] = 'X';

            break;
          default:
            // printf("Wert ist nicht zwischen 0 und 7\n");
            break;
          }
          break;
        case 'C':
          // printf("Der Buchstabe im String '%s' ist 'C'\n", positions[i]);
          switch (positions[i][len - 1]) {
          case '0':
            // printf("Wert ist C0\n");
            board[6] = 'X';

            break;
          case '1':
            // printf("Wert ist C1\n");
            board[7] = 'X';

            break;
          case '2':
            // printf("Wert ist C2\n");
            board[8] = 'X';

            break;
          case '3':
            // printf("Wert ist C3\n");
            board[12] = 'X';

            break;
          case '4':
            // printf("Wert ist C4\n");
            board[17] = 'X';

            break;
          case '5':
            // printf("Wert ist C5\n");
            board[16] = 'X';

            break;
          case '6':
            // printf("Wert ist C6\n");
            board[15] = 'X';

            break;
          case '7':
            // printf("Wert ist C7\n");
            board[11] = 'X';

            break;
          default:
            // printf("Wert ist nicht zwischen 0 und 7\n");
            break;
          }
          break;
        default:
          printf("Der Buchstabe im String '%s' ist weder 'A', 'B' noch 'C'(er "
                 "ist '%c')\n",
                 positions[i], positions[i][len - 2]);
          break;
        }
        break;
      default:
        printf(
          "Der Buchstabe im String '%s' ist weder '0' noch '1' (er ist '%c')\n",
          positions[i], positions[i][len - 2]);
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

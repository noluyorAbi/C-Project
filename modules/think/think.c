#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PIECES 18 // Maximale Anzahl an PIECEs

int think(char *gameState) {
  int board[25];

  printf("Thinker: TEST Received game state:\n%s\n", gameState);

  // Array zur Speicherung der Positionen
  char positions[MAX_PIECES][10]; // Angenommen: Maximal 100 Positionen, jede
                                  // Position ist max. 10 Zeichen lang
  int pieceCount = 0;

  // Zeilenweise parsen
  char *line = strtok(gameState, "\n");
  while (line != NULL) {
    // Prüfen, ob die Zeile mit "+ PIECE" beginnt
    if ((strncmp(line, "+ PIECE", 7) == 0) && strlen(line) == 13) {
      // Position extrahieren
      if (pieceCount < MAX_PIECES) {
        strncpy(positions[pieceCount], line + 7,
                sizeof(positions[pieceCount]) - 1);
        positions[pieceCount][sizeof(positions[pieceCount]) - 1] =
          '\0'; // Null-terminieren
        pieceCount++;
      } else {
        fprintf(stderr, "Error: Too many PIECE entries!\n");
        break;
      }
    }
    // Nächste Zeile
    line = strtok(NULL, "\n");
  }

  // Ausgabe der gespeicherten Positionen
  printf("Stored positions:\n");
  for (int i = 0; i < pieceCount; i++) {
    printf("Position %d: %s\n", i, positions[i]);
  }

  // Switch case um Steine zu printen
  for (int i = 0; i < pieceCount; i++) {
    size_t len = strlen(positions[i]);
    if (len >= 2) {
      switch (positions[i][len - 6]) {
      case '0':
        printf("Der Buchstabe im String '%s' ist '0'\n", positions[i]);
        switch (positions[i][len - 2]) {
        case 'A':
          printf("Der Buchstabe im String '%s' ist 'A'\n", positions[i]);
          switch (positions[i][len - 1]) {
          case '0':
            printf("Wert ist A0\n");
            board[0] = 0;
            break;
          case '1':
            printf("Wert ist A1\n");
            board[1] = 0;
            break;
          case '2':
            printf("Wert ist A2\n");
            board[2] = 0;
            break;
          case '3':
            printf("Wert ist A3\n");
            board[14] = 0;
            break;
          case '4':
            printf("Wert ist A4\n");
            board[23] = 0;
            break;
          case '5':
            printf("Wert ist A5\n");
            board[22] = 0;
            break;
          case '6':
            printf("Wert ist A6\n");
            board[21] = 0;
            break;
          case '7':
            printf("Wert ist A7\n");
            board[9] = 0;
            break;
          default:
            printf("Wert ist nicht zwischen 0 und 7\n");
            break;
          }
          break;
        case 'B':
          printf("Der Buchstabe im String '%s' ist 'B'\n", positions[i]);
          switch (positions[i][len - 1]) {
          case '0':
            printf("Wert ist B0\n");
            board[3] = 0;
            break;
          case '1':
            printf("Wert ist B1\n");
            board[4] = 0;
            break;
          case '2':
            printf("Wert ist B2\n");
            board[5] = 0;
            break;
          case '3':
            printf("Wert ist B3\n");
            board[13] = 0;
            break;
          case '4':
            printf("Wert ist B4\n");
            board[20] = 0;
            break;
          case '5':
            printf("Wert ist B5\n");
            board[19] = 0;
            break;
          case '6':
            printf("Wert ist B6\n");
            board[18] = 0;
            break;
          case '7':
            printf("Wert ist B7\n");
            board[10] = 0;
            break;
          default:
            printf("Wert ist nicht zwischen 0 und 7\n");
            break;
          }
          break;
        case 'C':
          printf("Der Buchstabe im String '%s' ist 'C'\n", positions[i]);
          switch (positions[i][len - 1]) {
          case '0':
            printf("Wert ist C0\n");
            board[6] = 0;
            break;
          case '1':
            printf("Wert ist C1\n");
            board[7] = 0;
            break;
          case '2':
            printf("Wert ist C2\n");
            board[8] = 0;
            break;
          case '3':
            printf("Wert ist C3\n");
            board[12] = 0;
            break;
          case '4':
            printf("Wert ist C4\n");
            board[17] = 0;
            break;
          case '5':
            printf("Wert ist C5\n");
            board[16] = 0;
            break;
          case '6':
            printf("Wert ist C6\n");
            board[15] = 0;
            break;
          case '7':
            printf("Wert ist C7\n");
            board[11] = 0;
            break;
          default:
            printf("Wert ist nicht zwischen 0 und 7\n");
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
        printf("Der Buchstabe im String '%s' ist '1'\n", positions[i]);
        switch (positions[i][len - 2]) {
        case 'A':
          printf("Der Buchstabe im String '%s' ist 'A'\n", positions[i]);
          switch (positions[i][len - 1]) {
          case '0':
            printf("Wert ist A0\n");
            board[0] = 1;
            break;
          case '1':
            printf("Wert ist A1\n");
            board[1] = 1;

            break;
          case '2':
            printf("Wert ist A2\n");
            board[2] = 1;

            break;
          case '3':
            printf("Wert ist A3\n");
            board[14] = 1;

            break;
          case '4':
            printf("Wert ist A4\n");
            board[23] = 1;

            break;
          case '5':
            printf("Wert ist A5\n");
            board[22] = 1;

            break;
          case '6':
            printf("Wert ist A6\n");
            board[21] = 1;

            break;
          case '7':
            printf("Wert ist A7\n");
            board[9] = 1;

            break;
          default:
            printf("Wert ist nicht zwischen 0 und 7\n");
            break;
          }
          break;
        case 'B':
          printf("Der Buchstabe im String '%s' ist 'B'\n", positions[i]);
          switch (positions[i][len - 1]) {
          case '0':
            printf("Wert ist B0\n");
            board[3] = 1;

            break;
          case '1':
            printf("Wert ist B1\n");
            board[4] = 1;

            break;
          case '2':
            printf("Wert ist B2\n");
            board[5] = 1;

            break;
          case '3':
            printf("Wert ist B3\n");
            board[13] = 1;

            break;
          case '4':
            printf("Wert ist B4\n");
            board[20] = 1;

            break;
          case '5':
            printf("Wert ist B5\n");
            board[19] = 1;

            break;
          case '6':
            printf("Wert ist B6\n");
            board[18] = 1;

            break;
          case '7':
            printf("Wert ist B7\n");
            board[10] = 1;

            break;
          default:
            printf("Wert ist nicht zwischen 0 und 7\n");
            break;
          }
          break;
        case 'C':
          printf("Der Buchstabe im String '%s' ist 'C'\n", positions[i]);
          switch (positions[i][len - 1]) {
          case '0':
            printf("Wert ist C0\n");
            board[6] = 1;

            break;
          case '1':
            printf("Wert ist C1\n");
            board[7] = 1;

            break;
          case '2':
            printf("Wert ist C2\n");
            board[8] = 1;

            break;
          case '3':
            printf("Wert ist C3\n");
            board[12] = 1;

            break;
          case '4':
            printf("Wert ist C4\n");
            board[17] = 1;

            break;
          case '5':
            printf("Wert ist C5\n");
            board[16] = 1;

            break;
          case '6':
            printf("Wert ist C6\n");
            board[15] = 1;

            break;
          case '7':
            printf("Wert ist C7\n");
            board[11] = 1;

            break;
          default:
            printf("Wert ist nicht zwischen 0 und 7\n");
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
         " |  %c-----5-----%c  |\n"
         " |  |     |     |  |\n"
         " |  |  %c--8--%c  |  |\n"
         " |  |  |     |  |  |\n"
         "%c-%c-%c    %c-%c-%c\n"
         " |  |  |     |  |  |\n"
         " |  |  %c--17--%c |\n"
         " |  |     |     |  |\n"
         " |  %c----20----%c |\n"
         " |        |        |\n"
         " %c-------%c-------%c\n"
         "\n",
         board[0], board[1], board[2], board[3], board[5], board[6], board[8],
         board[9], board[10], board[11], board[12], board[13], board[14],
         board[15], board[17], board[18], board[20], board[21], board[22],
         board[23]);

  return EXIT_SUCCESS;
}

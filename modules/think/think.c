#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PIECES 18 // Maximale Anzahl an PIECEs

int think(char *gameState) {
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
            break;
          case '1':
            printf("Wert ist A1\n");
            break;
          case '2':
            printf("Wert ist A2\n");
            break;
          case '3':
            printf("Wert ist A3\n");
            break;
          case '4':
            printf("Wert ist A4\n");
            break;
          case '5':
            printf("Wert ist A5\n");
            break;
          case '6':
            printf("Wert ist A6\n");
            break;
          case '7':
            printf("Wert ist A7\n");
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
            break;
          case '1':
            printf("Wert ist B1\n");
            break;
          case '2':
            printf("Wert ist B2\n");
            break;
          case '3':
            printf("Wert ist B3\n");
            break;
          case '4':
            printf("Wert ist B4\n");
            break;
          case '5':
            printf("Wert ist B5\n");
            break;
          case '6':
            printf("Wert ist B6\n");
            break;
          case '7':
            printf("Wert ist B7\n");
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
            break;
          case '1':
            printf("Wert ist C1\n");
            break;
          case '2':
            printf("Wert ist C2\n");
            break;
          case '3':
            printf("Wert ist C3\n");
            break;
          case '4':
            printf("Wert ist C4\n");
            break;
          case '5':
            printf("Wert ist C5\n");
            break;
          case '6':
            printf("Wert ist C6\n");
            break;
          case '7':
            printf("Wert ist C7\n");
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
            break;
          case '1':
            printf("Wert ist A1\n");
            break;
          case '2':
            printf("Wert ist A2\n");
            break;
          case '3':
            printf("Wert ist A3\n");
            break;
          case '4':
            printf("Wert ist A4\n");
            break;
          case '5':
            printf("Wert ist A5\n");
            break;
          case '6':
            printf("Wert ist A6\n");
            break;
          case '7':
            printf("Wert ist A7\n");
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
            break;
          case '1':
            printf("Wert ist B1\n");
            break;
          case '2':
            printf("Wert ist B2\n");
            break;
          case '3':
            printf("Wert ist B3\n");
            break;
          case '4':
            printf("Wert ist B4\n");
            break;
          case '5':
            printf("Wert ist B5\n");
            break;
          case '6':
            printf("Wert ist B6\n");
            break;
          case '7':
            printf("Wert ist B7\n");
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
            break;
          case '1':
            printf("Wert ist C1\n");
            break;
          case '2':
            printf("Wert ist C2\n");
            break;
          case '3':
            printf("Wert ist C3\n");
            break;
          case '4':
            printf("Wert ist C4\n");
            break;
          case '5':
            printf("Wert ist C5\n");
            break;
          case '6':
            printf("Wert ist C6\n");
            break;
          case '7':
            printf("Wert ist C7\n");
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

  return EXIT_SUCCESS;
}

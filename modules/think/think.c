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
        break;
      case '1':
        printf("Der Buchstabe im String '%s' ist '1'\n", positions[i]);
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

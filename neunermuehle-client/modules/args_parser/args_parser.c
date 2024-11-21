// args_parser/args_parser.c

#include "args_parser.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

/**
 * Parst die Kommandozeilenargumente und füllt die GameConfig-Struktur.
 */
bool
parse_args (int argc, char *argv[], GameConfig *config) {
  if (argc != 5) { // Erwartet genau 4 Argumente zusätzlich zum Programmnamen
    fprintf (stderr, "Fehler: Falsche Anzahl von Argumenten.\n");
    return false;
  }

  // Initialisiere die Struktur mit leeren Werten
  memset (config->game_id, 0, sizeof (config->game_id));
  config->player_number = 0;

  for (int i = 1; i < argc; i++) {
    if (strcmp (argv[i], "-g") == 0) {
      if (i + 1 < argc) {
        // Überprüfe, ob GAME-ID genau 13 Ziffern enthält
        size_t len = strlen (argv[i + 1]);
        if (len != 13) {
          fprintf (stderr,
                   "Fehler: GAME-ID muss genau 13 Ziffern lang sein.\n");
          return false;
        }
        for (size_t j = 0; j < len; j++) {
          if (!isdigit ((unsigned char) argv[i + 1][j])) {
            fprintf (stderr,
                     "Fehler: GAME-ID darf nur aus Ziffern bestehen.\n");
            return false;
          }
        }
        strncpy (config->game_id, argv[i + 1], 13);
        config->game_id[13] = '\0'; // Nullterminator sicherstellen
        i++; // Überspringe das nächste Argument, da es bereits
             // verarbeitet wurde
      } else {
        fprintf (stderr, "Fehler: -g benötigt einen Wert.\n");
        return false;
      }
    } else if (strcmp (argv[i], "-p") == 0) {
      if (i + 1 < argc) {
        // Überprüfe, ob Spielerzahl entweder 1 oder 2 ist
        if (strlen (argv[i + 1]) != 1
            || (argv[i + 1][0] != '1' && argv[i + 1][0] != '2')) {
          fprintf (stderr,
                   "Fehler: Spielerzahl muss entweder 1 oder 2 sein.\n");
          return false;
        }
        config->player_number = argv[i + 1][0] - '0';
        i++; // Überspringe das nächste Argument, da es bereits
             // verarbeitet wurde
      } else {
        fprintf (stderr, "Fehler: -p benötigt einen Wert.\n");
        return false;
      }
    } else {
      fprintf (stderr, "Unbekannter Parameter: %s\n", argv[i]);
      return false;
    }
  }

  // Überprüfe, ob beide Parameter korrekt gesetzt wurden
  if (strlen (config->game_id) != 13
      || (config->player_number != 1 && config->player_number != 2)) {
    fprintf (stderr, "Fehler: Ungültige oder fehlende Argumente.\n");
    return false;
  }

  return true;
}

/**
 * Zeigt die Nutzungshilfe an.
 */
void
print_usage (const char *prog_name) {
  printf ("Nutzung: %s -g <GAME-ID> -p <1|2>\n", prog_name);
  printf ("  -g <GAME-ID> : 13-stellige Spiel-ID\n");
  printf ("  -p <1|2>     : Spielerzahl (1 oder 2)\n");
}

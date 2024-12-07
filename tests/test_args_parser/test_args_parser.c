#include "../../modules/args_parser/args_parser.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Hilfsfunktion, um Kommandozeilenargumente zu simulieren
bool teste_argumente(int argc, char *argv[], bool erwartetes_ergebnis) {
  GameConfig config;
  bool ergebnis = parse_args(argc, argv, &config);

  if (ergebnis != erwartetes_ergebnis) {
    fprintf(stderr, "Test fehlgeschlagen für Argumente: ");
    for (int i = 0; i < argc; i++) {
      fprintf(stderr, "%s ", argv[i]);
    }
    fprintf(stderr, "\nErwartetes Ergebnis: %s, Erhaltenes Ergebnis: %s\n",
            erwartetes_ergebnis ? "true" : "false",
            ergebnis ? "true" : "false");
    return false;
  }

  // Wenn der Test bestanden wurde und das Ergebnis wahr ist, prüfe die
  // Konfiguration
  if (ergebnis) {
    printf("Test erfolgreich mit GAME-ID: %s, Spieler: %d\n", config.game_id,
           config.player_number);
  } else {
    printf("Test erfolgreich (wie erwartet fehlgeschlagen).\n");
  }

  return true;
}

int main() {
  int tests_fehlgeschlagen = 0;

  // Testfälle

  // 1. Gültige Eingabe
  char *gueltige_argumente[] = {"programm", "-g", "1234567890123", "-p", "1"};
  tests_fehlgeschlagen += !teste_argumente(5, gueltige_argumente, true);

  // 2. Gültige Eingabe (Spieler 2)
  char *gueltige_argumente_spieler2[] = {"programm", "-g", "9876543210123",
                                         "-p", "2"};
  tests_fehlgeschlagen +=
    !teste_argumente(5, gueltige_argumente_spieler2, true);

  // 3. Fehlende Spiel-ID
  char *fehlende_spiel_id[] = {"programm", "-g", "-p", "1"};
  tests_fehlgeschlagen += !teste_argumente(4, fehlende_spiel_id, false);

  // 4. Fehlende Spielerzahl
  char *fehlende_spielerzahl[] = {"programm", "-g", "1234567890123", "-p"};
  tests_fehlgeschlagen += !teste_argumente(4, fehlende_spielerzahl, false);

  // 5. Ungültige Spiel-ID-Länge
  char *ungueltige_spiel_id_laenge[] = {"programm", "-g", "12345", "-p", "1"};
  tests_fehlgeschlagen +=
    !teste_argumente(5, ungueltige_spiel_id_laenge, false);

  // 6. Spiel-ID mit Nicht-Ziffern
  char *ungueltige_spiel_id_zeichen[] = {"programm", "-g", "12345678abcd3",
                                         "-p", "1"};
  tests_fehlgeschlagen +=
    !teste_argumente(5, ungueltige_spiel_id_zeichen, false);

  // 7. Ungültige Spielerzahl
  char *ungueltige_spielerzahl[] = {"programm", "-g", "1234567890123", "-p",
                                    "3"};
  tests_fehlgeschlagen += !teste_argumente(5, ungueltige_spielerzahl, false);

  // 8. Zusätzliche Argumente
  char *zusatz_argumente[] = {"programm", "-g", "1234567890123",
                              "-p",       "1",  "-x"};
  tests_fehlgeschlagen += !teste_argumente(6, zusatz_argumente, false);

  // 9. Fehlende Parameter
  char *fehlende_parameter[] = {"programm"};
  tests_fehlgeschlagen += !teste_argumente(1, fehlende_parameter, false);

  // 10. Richtige Reihenfolge, gemischte Parameter
  char *korrekte_reihenfolge[] = {"programm", "-p", "2", "-g", "8765432101234"};
  tests_fehlgeschlagen += !teste_argumente(5, korrekte_reihenfolge, true);

  // 11. Falsche Reihenfolge (Parameter durcheinander)
  char *falsche_reihenfolge[] = {"programm", "-p", "-g", "8765432101234", "2"};
  tests_fehlgeschlagen += !teste_argumente(5, falsche_reihenfolge, false);

  // 12. Spielerzahl nicht numerisch
  char *nicht_numerische_spielerzahl[] = {"programm", "-g", "1234567890123",
                                          "-p", "x"};
  tests_fehlgeschlagen +=
    !teste_argumente(5, nicht_numerische_spielerzahl, false);

  // Zusammenfassung
  if (tests_fehlgeschlagen == 0) {
    printf("Alle Tests erfolgreich bestanden!\n");
  } else {
    printf("%d Tests fehlgeschlagen.\n", tests_fehlgeschlagen);
  }

  return tests_fehlgeschlagen == 0 ? 0 : 1;
}
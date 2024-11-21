// args_parser/args_parser.h

#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <stdbool.h>

/**
 * Struktur zur Speicherung der Spielkonfiguration.
 */
typedef struct {
  char game_id[14];  // 13 Ziffern + Nullterminator
  int player_number; // Spielerzahl (1 oder 2)
} GameConfig;

/**
 * Parst die Kommandozeilenargumente.
 *
 * @param argc Argumentanzahl
 * @param argv Argumentvektor
 * @param config Zeiger auf GameConfig-Struktur zur Speicherung der Ergebnisse
 * @return true bei erfolgreichem Parsen, false bei Fehlern
 */
bool
parse_args (int argc, char *argv[], GameConfig *config);

/**
 * Zeigt die Nutzungshilfe an.
 *
 * @param prog_name Name des Programms
 */
void
print_usage (const char *prog_name);

#endif // ARGS_PARSER_H

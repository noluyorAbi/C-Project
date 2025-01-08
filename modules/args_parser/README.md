# args_parser

## Inhalt des Ordners

Der `args_parser`-Ordner beinhaltet folgende Dateien:

1. args_parser.h
2. args_parser.c
3. config.h
4. config.c
5. constants.h

## Funktionalität

Der `args_parser`-Ordner enthält Funktionen, die die Verarbeitung von Kommandozeilenargumenten sowie das Einlesen und Verarbeiten einer Konfigurationsdatei ermöglichen. Das Hauptziel dieser Implementierung ist es, die Konfiguration eines Spiels zu initialisieren, indem sowohl die übergebenen Kommandozeilenargumente als auch die Konfigurationsdatei berücksichtigt werden.

### args_parser

- `args_parser.h` und `args_parser.c` definieren und implementieren die Funktionen zur Verarbeitung von Kommandozeilenargumenten:
  - `parse_args`: Diese Funktion verarbeitet die Eingabeparameter des Programms. Das Pflichtargument ist `-g` gefolgt von einer 13-stelligen `GAME-ID`. Optionale Argumente sind `-p` für die Spielernummer (1 oder 2, Standardwert ist 1) und `-c` für den Pfad zu einer Konfigurationsdatei (Standard ist "client.conf").
  - `print_usage`: Diese Funktion gibt Anweisungen zur Benutzung des Programms aus.

### config

- `config.h` und `config.c` enthalten die Definition und Implementierung der Funktionen zur Verarbeitung der Konfigurationsdatei:
  - `parse_config_file`: Diese Funktion liest die Konfigurationsdatei ein und füllt die `Config`-Struktur mit den entsprechenden Werten wie `Hostname`, `PortNumber` und `GameKindName`.

### Konstanten

- `constants.h` definiert die Standardwerte für einige der Konfigurationseinstellungen:
  - `GAMEKINDNAME`, `PORTNUMBER`, `HOSTNAME`.

## Abhängigkeiten

Das `args_parser`-Modul verwendet Standardbibliotheken wie `<stdbool.h>`, `<string.h>`, `<stdio.h>`, `<netinet/in.h>` und `<ctype.h>`. Diese Bibliotheken sind erforderlich, um grundlegende Operationen wie Zeichenkettenverarbeitung, Dateizugriff und Netzwerkoperationen durchzuführen.

## Verwendung

Um dieses Modul zu verwenden, sollte das Hauptprogramm die `parse_args`-Funktion aufrufen, um die Kommandozeilenparameter zu analysieren, und die `parse_config_file`-Funktion, um Konfigurationsdaten aus einer Datei zu laden. Beispielaufruf im Code könnte folgendermaßen aussehen:

- Initialisieren einer `GameConfig`-Struktur und Aufruf von `parse_args` mit den gegebenen Argumenten.
- Einlesen einer Konfigurationsdatei mittels `parse_config_file`, um die `Config`-Struktur zu füllen.

## Interne Struktur

Der Code ist in folgende Teile gegliedert:

- Definitionen und Implementierungen zur Argumentverarbeitung (`args_parser.h` und `args_parser.c`).
- Definitionen und Implementierungen zur Konfigurationsdateiverarbeitung (`config.h` und `config.c`).
- Definition von Konstanten für Standardkonfigurationswerte (`constants.h`).

Jeder dieser Teile ist darauf ausgelegt, eine klare und spezifische Aufgabe zu erfüllen, wobei die verschiedenen Dateien zusammenarbeiten, um eine vollständige Funktionalität zu bieten.

## Tests

Keine spezifischen Testinformationen sind im gegebenen Kontext verfügbar. Es wird empfohlen, die Funktionalitäten manuell zu testen, indem verschiedene Kombinationen von Kommandozeilenargumenten und Konfigurationsdateiinhalten verwendet werden, um sicherzustellen, dass alle erwarteten Konfigurationen korrekt erkannt und verarbeitet werden.

## Sonstiges

Die Verarbeitung der Konfigurationsdatei unterstützt einfache Schlüssel-Wert-Paare und ignoriert Kommentare und Leerzeilen. Fehler bei ungültigen Formaten oder unbekannten Schlüsseln werden gemeldet, um die Integrität der Konfiguration sicherzustellen.
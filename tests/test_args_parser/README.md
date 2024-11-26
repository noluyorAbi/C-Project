# Args Parser Modul

Dieses Projekt enthält ein C-Modul zur Verarbeitung von Kommandozeilenargumenten (`args_parser.c`) und eine umfassende Testsuite (`test_args_parser.c`), um die Funktionalität zu validieren. Das Modul überprüft Benutzer-Eingaben und stellt sicher, dass diese korrekt sind.

## Inhaltsverzeichnis

- [Übersicht](#übersicht)
- [Funktionen](#funktionen)
- [Verwendung](#verwendung)
  - [Kompilieren](#kompilieren)
  - [Ausführen](#ausführen)
- [Tests](#tests)
  - [Testfälle](#testfälle)
  - [Tests ausführen](#tests-ausführen)
- [Beitragen](#beitragen)
- [Lizenz](#lizenz)

---

## Übersicht

Das `args_parser`-Modul verarbeitet Kommandozeilenargumente und extrahiert die Konfigurationsparameter für ein Spiel. Es validiert die Eingaben basierend auf den folgenden erwarteten Argumenten:

- `-g <GAME-ID>`: Eine 13-stellige numerische Spiel-ID.
- `-p <1|2>`: Die Spielerzahl (entweder `1` oder `2`).

## Funktionen

- Validierung der Kommandozeilenargumente.
- Überprüfung, ob die Spiel-ID aus genau 13 Ziffern besteht.
- Akzeptiert nur Spielerzahlen `1` oder `2`.
- Gibt Fehlermeldungen bei ungültigen oder fehlenden Argumenten aus.
- Testsuite mit Standard- und Randfalltests.

---

## Verwendung

### Kompilieren

Verwenden Sie einen C-Compiler wie GCC, um das Modul und die Testsuite zu kompilieren:

1. **Kompilieren des Hauptmoduls:**
   ```bash
   gcc -o args_parser args_parser.c


## Testfälle

Die Testsuite deckt folgende Szenarien ab:

	1.	Gültige Eingaben:
	•	Korrekte GAME-ID und SPIELERZAHL.
	2.	Ungültige Eingaben:
	•	Fehlende Argumente.
	•	Ungültige GAME-ID (falsche Länge, Nicht-Ziffern).
	•	Ungültige SPIELERZAHL (nicht 1 oder 2).
	3.	Randfälle:
	•	Zusätzliche Argumente.
	•	Parameter in falscher Reihenfolge.
	•	Nicht-numerische Werte für SPIELERZAHL.
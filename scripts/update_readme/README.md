# Ordnername: update_readme

## Inhalt des Ordners

Dieser Ordner enthält das Bash-Skript `update_readme.sh` sowie eine `README.md`-Datei. Diese Dateien dienen zur Automatisierung der Aktualisierung von Projektdokumentationen.

### Dateien

- **update_readme.sh**: Ein Shell-Skript, das den Inhalt der `README.md`-Datei mit der aktuellen Verzeichnisstruktur des Projekts aktualisiert.

- **README.md**: Die Dokumentation des Skripts selbst sowie dessen Gebrauchsanweisung.

## Funktionalität

Das Hauptziel des `update_readme.sh`-Skripts besteht darin, die `README.md`-Datei eines Projekts automatisch mit der neuesten Verzeichnisstruktur zu aktualisieren. Dies geschieht durch den Einsatz des `tree`-Befehls, der die Verzeichnisstruktur generiert. Das Skript bietet folgende Hauptfunktionen:

1. **Terminalfreundliche Ausgabe**: Informative Nachrichten, Fehler- und Update-Meldungen werden farblich hervorgehoben, um die Lesbarkeit zu verbessern.
   
2. **Still-Modus**: Mit dem `-q` oder `--quiet` Parameter können nicht-essentielle Nachrichten unterdrückt werden.

3. **Automatische Installation von `tree`**: Das Skript überprüft, ob der `tree`-Befehl vorhanden ist, und versucht ihn, falls nötig, mit Hilfe eines unterstützten Paketmanagers (wie `apt-get`, `yum`, `dnf`, `pacman` oder `brew`) zu installieren.

4. **Benutzerdefinierte Ausschlüsse**: Bestimmte Dateien und Verzeichnisse können vom `tree`-Output ausgeschlossen werden, um unerwünschte Elemente in der Struktur zu vermeiden.

5. **Aktualisierung der `README.md`**: Das Skript sucht nach vordefinierten Markern in der `README.md` und ersetzt den Inhalt zwischen diesen mit der neuesten Verzeichnisstruktur.

6. **Robuste Fehlerbehandlung**: Das Skript gibt informative Fehlermeldungen aus und beendet sich mit einem Fehlercode, wenn Voraussetzungen nicht erfüllt sind, beispielsweise wenn die `README.md` fehlt oder Marker nicht gefunden werden.

7. **Temporäre Dateiverwaltung**: Temporäre Dateien, die während der Ausführung erzeugt werden, werden am Ende gelöscht, selbst wenn das Skript unterbrochen wird.

## Abhängigkeiten

Das Skript benötigt den `tree`-Befehl zur Erstellung der Verzeichnisstruktur. Sollte dieser nicht installiert sein, versucht das Skript, diesen selbst zu installieren. Ein unterstützter Paketmanager wie `apt-get`, `yum`, `dnf`, `pacman` oder `brew` ist Voraussetzung für die automatische Installation.

## Verwendung

Zur Ausführung des Skripts nutzen Sie folgendes Kommando im Terminal:

bash
./update_readme.sh [OPTIONS]

### Optionen

- `-q, --quiet`: Unterdrückt nicht-essentielle Informationsnachrichten.
- `-h, --help`: Zeigt die Verwendung und verfügbaren Optionen des Skripts an.

## Interne Struktur

Das Skript navigiert zunächst zum Projektstammverzeichnis, überprüft und installiert bei Bedarf den `tree`-Befehl, generiert die Verzeichnisstruktur und aktualisiert die `README.md`-Datei zwischen den Markern `<!-- project-structure-start -->` und `<!-- project-structure-end -->`.

## Sonstiges

Das Skript behandelt Fehlermeldungen präzise und gibt bei Problemen deutliche Hinweise aus. Beispielhafte Fehlermeldungen umfassen das Fehlen der `README.md` oder der benötigten Marker darin. Sollten diese Probleme auftreten, ist eine manuelle Prüfung und Korrektur nötig, um eine erfolgreiche Ausführung zu gewährleisten.

Bitte beachten Sie, dass diese Dokumentation auf den bereitgestellten Informationen basiert und für weitergehende Änderungen oder Anpassungen des Skripts spezifische Kenntnisse der Bash-Programmierung erforderlich sein können.
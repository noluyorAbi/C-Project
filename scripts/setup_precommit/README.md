# setup_precommit

## Inhalt des Ordners

Der Ordner `setup_precommit` enthält ein Skript zur Einrichtung eines Git-`pre-commit`-Hooks sowie eine zugehörige README-Datei. Der Zweck dieses Ordners ist es, sicherzustellen, dass alle Codeeinreichungen in einem Git-Repository vor dem Commit bestimmten Qualitätsstandards entsprechen und dass die Dokumentation der Projektstruktur auf dem neuesten Stand gehalten wird.

## Funktionalität

Das Hauptskript `setup_pre_commit.sh` dient dazu, einen `pre-commit`-Hook im Git-Repository zu konfigurieren. Dieser Hook wird vor jedem Commit automatisch ausgeführt und führt folgende Aufgaben aus:

1. **Codeformatierung**: Durch den Aufruf von `make format` wird sichergestellt, dass alle Quellcodedateien gemäß den im Projekt festgelegten Stilregeln formatiert sind.
2. **Aktualisierung der `README.md`**: Der Aufruf von `make update_readme` stellt sicher, dass der Abschnitt zur Projektstruktur in der `README.md` basierend auf der neuesten Struktur des Projekts aktualisiert wird.
3. **Staging der aktualisierten `README.md`**: Die Änderungen an der `README.md` werden automatisch für den Commit vorbereitet.

## Abhängigkeiten

- Eine gültige `Makefile` im Hauptverzeichnis des Repositories mit den Zielen:
  - `format`: Zur Gewährleistung der Codeformatierung.
  - `update_readme`: Zur Aktualisierung der Projektstruktur in der `README.md`.
- Git muss im Repository initialisiert sein (`git init`).

## Verwendung

Um das Skript zu verwenden, folgen Sie diesen Schritten:

1. **Speicherung des Skripts**: Speichern Sie `setup_pre_commit.sh` im Verzeichnis `scripts/setupprecommit/` Ihres Repositories.
2. **Ausführbarkeitsrechte setzen**: Führen Sie den Befehl `chmod +x scripts/setupprecommit/setup_pre_commit.sh` aus, um das Skript ausführbar zu machen.
3. **Skript ausführen**: Führen Sie das Skript mit `scripts/setupprecommit/setup_pre_commit.sh` aus, um den `pre-commit`-Hook einzurichten.
4. **Überprüfung des Hooks**: Überprüfen Sie, ob der `pre-commit`-Hook in `.git/hooks/` vorhanden ist, indem Sie `ls .git/hooks/pre-commit` ausführen.

## Interne Struktur

Das Skript `setup_pre_commit.sh` besteht aus mehreren Schritten:

- Es wechselt in das Hauptverzeichnis des Git-Repositories.
- Es überprüft, ob ein `.git`-Verzeichnis existiert, um sicherzustellen, dass das Skript im Root-Verzeichnis des Repositories ausgeführt wird.
- Der `pre-commit`-Hook wird mit spezifischem Inhalt in die Datei `.git/hooks/pre-commit` geschrieben.
- Die Ausführbarkeitsrechte für den Hook werden gesetzt, um sicherzustellen, dass Git den Hook ausführen kann.

## Sonstiges

- Der `pre-commit`-Hook ist lokal auf die Umgebung jedes Entwicklers beschränkt und wird nicht automatisch mit anderen geteilt. Für eine gemeinsame Konfiguration in einem Team wird die Verwendung von Tools wie [pre-commit](https://pre-commit.com/) empfohlen.
- Sollten bereits andere Funktionen im `pre-commit`-Hook vorhanden sein, überschreibt dieses Skript diese. Es wird empfohlen, die bestehenden Funktionen zu ergänzen, anstatt sie zu überschreiben.
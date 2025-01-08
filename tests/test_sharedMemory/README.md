# test_sharedMemory

## Inhalt des Ordners

Der Ordner `test_sharedMemory` enthält zwei Hauptdateien, die für die Überprüfung der Shared-Memory-Funktionalität verantwortlich sind:

- `test_sharedMemory.bash`: Ein Shell-Skript, das für das Kompilieren, Ausführen und Protokollieren der Testergebnisse der Shared-Memory-Funktionalität verwendet wird.
- `test_sharedMemory.c`: Ein C-Quellcode, der die Testfälle zur Überprüfung der Shared-Memory-Operationen implementiert.

## Funktionalität

Der Zweck des Ordners `test_sharedMemory` ist es, die Shared-Memory-Funktionen zu testen, die die Erstellung, den Zugriff und die Verwaltung eines gemeinsamen Speicherbereichs für mehrere Prozesse ermöglichen. Dies ist besonders nützlich zur Verwaltung von Spielerdaten in einem Multiplayer-Spiel.

### test_sharedMemory.bash

- **Kompilierung**: Das Skript stellt sicher, dass der Testcode `test_sharedMemory.c` korrekt kompiliert wird. Es überprüft, ob notwendige Dateien vorhanden sind, und führt die Kompilierung mit GCC durch.
- **Ausführung der Tests**: Nach erfolgreicher Kompilierung wird das Testprogramm ausgeführt, und seine Ausgaben werden in Protokolldateien gespeichert.
- **Fehlerbehandlung**: Das Skript enthält Mechanismen zur Fehlererkennung, um sicherzustellen, dass die Tests unter stabilen Bedingungen ausgeführt werden, indem alle verbliebenen Shared-Memory-Segmente vor und nach den Tests bereinigt werden.
- **Protokollierung**: Alle Testergebnisse und mögliche Fehler werden in separaten Log-Dateien (`test_sharedMemory.out` und `test_sharedMemory.err`) gespeichert, um die Analyse zu erleichtern.
- **Benutzereingaben**: Am Ende des Tests fragt das Skript den Benutzer, ob die erstellten Log-Dateien gelöscht werden sollen, um den Speicherplatz zu verwalten.

### test_sharedMemory.c

- **Testfälle**: Die C-Datei enthält mehrere Testfälle zur Prüfung der korrekten Funktion der Shared-Memory-Operationen:
  - **Erstellung und Anhängen von Shared Memory**: Verifiziert, dass Shared-Memory-Segmente für verschiedene Spielerzahlen korrekt erstellt und angehängt werden können.
  - **Initialisierung**: Überprüft die korrekte Initialisierung von Shared-Memory-Strukturen mit gültigen und ungültigen Parametern.
  - **Fehlerbehandlung**: Testet Szenarien mit ungültigen Eingaben und überprüft, dass Fehler korrekt erkannt werden.
  - **Stresstest**: Führt mehrfaches Erstellen und Löschen von Shared-Memory-Segmenten durch, um die Systemstabilität zu überprüfen.

## Abhängigkeiten

- **Compiler**: Das Skript benötigt den GCC-Compiler, um die C-Quellcodedatei zu kompilieren.
- **Header-Datei**: `shared_memory.h` muss im entsprechenden Modulverzeichnis vorhanden sein, da es die Deklarationen für die Shared-Memory-Funktionen enthält.
- **Betriebssystemtools**: Das Skript verwendet Unix-Kommandos wie `ipcs` und `ipcrm` zur Verwaltung von Shared-Memory-Segmenten.

## Verwendung

1. **Voraussetzungen**: Stellen Sie sicher, dass alle erforderlichen Header- und Quellcodedateien vorhanden sind, und dass der GCC-Compiler installiert ist.
2. **Ausführung des Skripts**: Starten Sie das Skript `test_sharedMemory.bash` in einem Terminal, um den Kompiliervorgang zu starten und die Tests durchzuführen.
3. **Ergebnisse analysieren**: Nach Abschluss der Tests finden Sie die Protokolle in den angegebenen Log-Verzeichnissen. Diese enthalten detaillierte Informationen über jeden Testfall und mögliche Fehler.
4. **Bereinigung**: Befolgen Sie die Anweisungen des Skripts, um die Log-Dateien zu löschen, wenn sie nicht mehr benötigt werden.

## Tests

Die Tests decken verschiedene Aspekte der Shared-Memory-Funktionalität ab, einschließlich der Fehlerbehandlung und der Stabilität bei wiederholten Operationen. Nach Abschluss jedes Tests wird eine Zusammenfassung der bestandenen und fehlgeschlagenen Tests ausgegeben, um eine schnelle Bewertung der Ergebnisse zu ermöglichen.
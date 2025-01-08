# Shared Memory Modul

## Inhalt des Ordners

Der Ordner 'shared_memory' enthält zwei zentrale Dateien:

1. **shared_memory.c**: Diese Datei enthält die Implementierung der Funktionalitäten zur Verwaltung von Shared Memory in einem System, einschließlich der Erstellung, Anbindung, Loslösung, Entfernung und Initialisierung von Shared Memory Segmenten.
2. **shared_memory.h**: Dies ist die Header-Datei, die die Schnittstelle für die Nutzung der in `shared_memory.c` definierten Funktionen bereitstellt. Sie enthält Definitionen und Deklarationen, die in der Implementierung verwendet werden.

## Funktionalität

Das Modul ermöglicht es, Speicherbereiche im Shared Memory für den Einsatz in Multi-Prozess-Anwendungen zu erstellen und zu verwalten. Es wird hauptsächlich für die Speicherung von Spiel- und Spielerinformationen eingesetzt. Zu den Funktionen gehören:

- **Erstellen eines Shared Memory Segments**: Initialisiert einen Speicherbereich im Shared Memory für eine bestimmte Anzahl von Spielern. Hierbei werden Zugriffsrechte festgelegt.
- **Anbinden an ein Shared Memory Segment**: Ermöglicht Prozessen den Zugriff auf den zuvor erstellten Speicherbereich.
- **Loslösung von einem Shared Memory Segment**: Trennt die Verbindung eines Prozesses zu einem Shared Memory Segment.
- **Entfernen eines Shared Memory Segments**: Markiert einen Speicherbereich zur Löschung, sobald er nicht mehr von Prozessen genutzt wird.
- **Initialisierung des Shared Memory**: Bereitet das Speichersegment für die Verwendung vor, indem Spiel- und Spielerinformationen darin gespeichert werden.
- **Aufräumen des Shared Memory**: Trennung und Entfernung des Speicherbereichs zur Freigabe von Ressourcen.

## Abhängigkeiten

Dieses Modul setzt voraus, dass das System die Standardbibliotheken `errno.h`, `stdio.h`, `stdlib.h`, `string.h`, `sys/ipc.h`, `sys/shm.h`, und `unistd.h` unterstützt, um die Shared Memory Funktionalitäten bereitzustellen.

## Verwendung

Um das Modul zu verwenden, wird die Header-Datei `shared_memory.h` in das Projekt eingebunden. Anschließend können die bereitgestellten Funktionen zur Verwaltung von Shared Memory Segmenten aufgerufen werden. Die Initialisierung erfolgt durch die Angabe der Spieleranzahl, des Spielnamens, sowie der Prozess-IDs für spezifische Abläufe.

## Interne Struktur

- **Datentypen**: 
  - `Player`: Struktur zur Speicherung von Spielerinformationen wie Spielernummer, Spielername und Registrierungsstatus.
  - `SharedMemory`: Flexible Struktur zur Speicherung von Spiel- und Spielerinformationen sowie Prozess-IDs.
  
- **Fehlercodes**: 
  - `SHM_SUCCESS`: Operation erfolgreich.
  - `SHM_ERROR_CREATION`, `SHM_ERROR_ATTACH`, `SHM_ERROR_DETACH`, `SHM_ERROR_REMOVE`: Fehlermeldungen für diverse Fehlerfälle.

## Tests

Es liegen keine spezifischen Informationen zu Testdateien oder Testframeworks innerhalb der bereitgestellten Daten vor.

## Sonstiges

Hinweise innerhalb der `shared_memory.c` Datei deuten auf ungeklärte Fragen hinsichtlich der initialen Nutzung des Shared Memory Segments für das Laden von Spieldaten hin. Weitere Dokumentation oder Kommentare könnten zur Klärung dieses Aspektes erforderlich sein.
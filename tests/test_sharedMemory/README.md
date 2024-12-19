# Inhalt

- **`test_sharedMemory/`**: Integrationstest für die Shared-Memory-Funktionen im Modul `shared_memory.c`.

### `test_sharedMemory/`

#### Zweck

`test_sharedMemory.c` ist ein Integrationstest, der die Shared-Memory-Funktionen überprüft. Diese Funktionen ermöglichen die Erstellung, den Zugriff und die Verwaltung eines gemeinsamen Speicherbereichs für mehrere Prozesse, insbesondere zur Verwaltung von Spielerdaten in einem Multiplayer-Spiel.

#### Funktionsweise

Der Test validiert die korrekte Funktionalität von Shared-Memory-Operationen durch:

1. **Erstellung und Anhängen von Shared Memory:**
   - Testet die Funktion `createSharedMemory` für verschiedene Spieleranzahlen, einschließlich Grenzwerte wie `0`, `1`, `100` und `101` Spieler.
   - Überprüft das erfolgreiche Anhängen (`attachSharedMemory`), Trennen (`detachSharedMemory`) und Entfernen (`removeSharedMemory`) des Shared Memory-Segments.

2. **Initialisierung des Shared Memory:**
   - Verwendet `initSharedMemory`, um die Shared-Memory-Struktur mit gültigen und ungültigen Parametern zu initialisieren.
   - Überprüft die korrekte Initialisierung von Feldern wie `gameName`, `playerCount`, `playerNumber`, `thinkerPID` und `connectorPID`.
   - Testet Szenarien mit `NULL`-Spielnamen und zu langen Spielnamen.

3. **Fehlerbehandlung:**
   - Testet die Reaktion des Systems auf ungültige Eingaben, wie z.B. eine ungültige Anzahl von Spielern oder das Anhängen eines ungültigen Shared Memory-Segments.
   - Stellt sicher, dass Fehler korrekt erkannt und behandelt werden, ohne dass es zu Abstürzen kommt.

4. **Mehrfache Erstellung und Löschung:**
   - Führt zehn Iterationen durch, in denen Shared Memory-Segmente wiederholt erstellt, angehängt, getrennt und entfernt werden.
   - Überprüft die Stabilität des Systems und das Fehlen von Speicherlecks bei wiederholten Operationen.

#### Ablauf des Tests

1. **Initialisierung:**
   - Shared Memory wird für verschiedene Spieleranzahlen erstellt (z. B. `2`, `0`, `100`, `101`, `-1` Spieler).

2. **Dateninitialisierung und Validierung:**
   - Felder wie `gameName`, `playerCount`, `playerNumber`, `thinkerPID` und `connectorPID` werden initialisiert und überprüft.
   - Überprüft die Spielerinformationen im Shared Memory, einschließlich `playerNumber`, `playerName` und `isRegistered`.

3. **Fehlerbehandlung:**
   - Szenarien wie das Anhängen an ungültige Speicherbereiche (`shmat` mit ungültigen IDs) und das Trennen eines ungültigen Zeigers (`shmdt` mit `NULL`-Zeigern) werden getestet und korrekt gehandhabt.

4. **Grenzwerte:**
   - Der Test evaluiert Grenzfälle wie:
     - **0 Spieler**: Überprüft, ob die Erstellung des Shared Memory korrekt fehlschlägt.
     - **1 Spieler**: Minimal gültiger Wert.
     - **100 Spieler**: Maximale gültige Anzahl von Spielern.
     - **101 Spieler** und **-1 Spieler**: Überprüft, ob die Erstellung des Shared Memory korrekt fehlschlägt.

5. **Abschluss:**
   - Shared Memory wird getrennt und entfernt.
   - Der erfolgreiche Abschluss des Tests wird ausgegeben, zusammen mit einer Zusammenfassung der bestandenen und fehlgeschlagenen Tests.

#### Beispielhafte Ausführung

Beim Ausführen des Tests (`./test_sharedMemory`) sollte eine Ausgabe ähnlich der folgenden erscheinen:

```text
Starting Shared Memory Tests

[Test] SharedMemory erstellen und anhängen für 2 Spieler
[INFO] SharedMemory erstellen und anhängen: Shared memory created with shmid: 3670018
[INFO] SharedMemory erstellen und anhängen: Shared memory attached at address: 0x100a70000
[INFO] SharedMemory erstellen und anhängen: Shared memory detached successfully.
[INFO] SharedMemory erstellen und anhängen: Shared memory removed successfully.
[PASS] SharedMemory erstellen und anhängen: Shared memory erstellt, angehängt, getrennt und entfernt.

[Test] SharedMemory initialisieren für 2 Spieler
[INFO] SharedMemory initialisieren: Verifying initialization...
[INFO] Game name initialized correctly.
[INFO] Player number initialized correctly.
[INFO] Player count initialized correctly.
[INFO] Thinker PID initialized correctly.
[INFO] Connector PID initialized correctly.
[INFO] All initialization verifications passed.
[PASS] SharedMemory initialisieren: Shared memory initialized and verified successfully.
[INFO] SharedMemory initialisieren: Shared memory detached successfully.
[INFO] SharedMemory initialisieren: Shared memory removed successfully.
[PASS] SharedMemory initialisieren: Cleanup erfolgreich.

[Test] SharedMemory num_players = 0
[PASS] SharedMemory num_players = 0: Shared Memory Erstellung korrekt fehlgeschlagen.

[Test] SharedMemory num_players = 1
[INFO] SharedMemory num_players = 1: Shared memory attached at address: 0x100a74000
[INFO] SharedMemory num_players = 1: Shared memory detached successfully.
[INFO] SharedMemory num_players = 1: Shared memory removed successfully.
[PASS] SharedMemory num_players = 1: Shared Memory erstellt, angehängt, getrennt und entfernt.

[Test] SharedMemory num_players = 100
[INFO] SharedMemory num_players = 100: Shared memory attached at address: 0x100a74000
[INFO] SharedMemory num_players = 100: Shared memory detached successfully.
[INFO] SharedMemory num_players = 100: Shared memory removed successfully.
[PASS] SharedMemory num_players = 100: Shared Memory erstellt, angehängt, getrennt und entfernt.

[Test] SharedMemory num_players = 101
[PASS] SharedMemory num_players = 101: Shared Memory Erstellung korrekt fehlgeschlagen.

[Test] SharedMemory num_players = -1
[PASS] SharedMemory num_players = -1: Shared Memory Erstellung korrekt fehlgeschlagen.

[Test] SharedMemory initialisieren mit NULL game_name
[PASS] SharedMemory initialisieren mit NULL game_name: Shared Memory Initialization korrekt fehlgeschlagen mit NULL game_name.

[Test] SharedMemory initialisieren mit zu langem game_name
[PASS] SharedMemory initialisieren mit zu langem game_name: Shared Memory Initialization korrekt fehlgeschlagen mit zu langem game_name.

[Test] Multiple SharedMemory Erstellung und Löschung

[Multiple SharedMemory Erstellung und Löschung] Iteration 1/10
[INFO] Multiple SharedMemory Erstellung und Löschung: Iteration 1: Shared Memory created with shmid: 53215232
[INFO] Multiple SharedMemory Erstellung und Löschung: Iteration 1: Shared memory attached at address: 0x100a74000
[INFO] Multiple SharedMemory Erstellung und Löschung: Iteration 1: Shared memory detached successfully.
[INFO] Multiple SharedMemory Erstellung und Löschung: Iteration 1: Shared memory removed successfully.
[PASS] Multiple SharedMemory Erstellung und Löschung: Shared Memory erstellt, angehängt, getrennt und entfernt.

...

[Multiple SharedMemory Erstellung und Löschung] Iteration 10/10
[INFO] Multiple SharedMemory Erstellung und Löschung: Iteration 10: Shared Memory created with shmid: 53805056
[INFO] Multiple SharedMemory Erstellung und Löschung: Iteration 10: Shared memory attached at address: 0x100a74000
[INFO] Multiple SharedMemory Erstellung und Löschung: Iteration 10: Shared memory detached successfully.
[INFO] Multiple SharedMemory Erstellung und Löschung: Iteration 10: Shared memory removed successfully.
[PASS] Multiple SharedMemory Erstellung und Löschung: Shared Memory erstellt, angehängt, getrennt und entfernt.
[PASS] Multiple SharedMemory Erstellung und Löschung: All multiple creation iterations passed successfully.

All tests completed
[SUMMARY] Bestanden: 21, Fehlgeschlagen: 0
[ERROR] Ungültige Anzahl von Spielern: 0. Muss zwischen 0 und 100 liegen.
[ERROR] Ungültige Anzahl von Spielern: 101. Muss zwischen 0 und 100 liegen.
[ERROR] Ungültige Anzahl von Spielern: -1. Muss zwischen 0 und 100 liegen.
[ERROR] Spielname darf nicht NULL sein.
[ERROR] Spielname zu lang: 265 Zeichen. Maximal erlaubt sind 255 Zeichen.
Bereinige verbleibende Shared-Memory-Segmente...
```

#### Ergebnisse

- **Bestanden: 21**
- **Fehlgeschlagen: 0**

##### Erfolgreiche Tests:
- **Shared Memory Erstellung und Verwaltung:**
  - Shared Memory wurde erfolgreich erstellt, angehängt, getrennt und entfernt für gültige Spielerzahlen (`1`, `2`, `100`).
  
- **Initialisierung des Shared Memory:**
  - Felder wie `gameName`, `playerCount`, `playerNumber`, `thinkerPID` und `connectorPID` wurden korrekt initialisiert.
  
- **Fehlerbehandlung:**
  - Ungültige Spielerzahlen (`0`, `101`, `-1`) führten korrekt zu Fehlern bei der Erstellung des Shared Memory.
  - Initialisierung mit `NULL`-Spielnamen und zu langen Spielnamen wurde korrekt abgelehnt.
  
- **Mehrfache Erstellung und Löschung:**
  - Zehn Iterationen der Erstellung und Löschung von Shared Memory-Segmenten wurden erfolgreich abgeschlossen, was auf die Stabilität und das Fehlen von Speicherlecks hinweist.

##### Fehlgeschlagene Tests:
- **Keine Tests fehlgeschlagen.** Alle getesteten Szenarien wurden erfolgreich bestanden, was die Robustheit und Korrektheit der implementierten Shared-Memory-Funktionen bestätigt.

#### Fazit

Der Test deckt die zentralen Funktionalitäten und möglichen Fehlerfälle des `shared_memory.c`-Moduls ab. Die Ergebnisse bestätigen die Zuverlässigkeit und Skalierbarkeit der implementierten Shared-Memory-Funktionen. Insbesondere wurde die korrekte Handhabung sowohl gültiger als auch ungültiger Eingaben sichergestellt, und die Stabilität des Systems bei wiederholten Operationen wurde bestätigt.

**Empfehlungen:**
- **Erweiterung der Testabdeckung:** Weitere Tests könnten parallele Zugriffe auf Shared Memory oder die Interaktion mit mehreren Prozessen umfassen, um die Robustheit unter realistischen Betriebsbedingungen weiter zu erhöhen.
- **Code-Review:** Ein formelles Code-Review könnte zusätzliche Sicherheit bieten und weitere Optimierungspotenziale aufdecken.
- **Automatisierung:** Integrieren Sie die Tests in eine kontinuierliche Integrationspipeline, um bei zukünftigen Änderungen automatisch die Integrität der Shared-Memory-Funktionen zu überprüfen.


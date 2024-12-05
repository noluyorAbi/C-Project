# Inhalt

- **`test_sharedMemory/`**: Integrationstest für die Shared-Memory-Funktionen im Modul `shared_memory.c`.

### `test_sharedMemory/`

#### Zweck

`test_sharedMemory.c` ist ein Integrationstest, der die Shared-Memory-Funktionen überprüft. Diese Funktionen ermöglichen die Erstellung, den Zugriff und die Verwaltung eines gemeinsamen Speicherbereichs für mehrere Prozesse, insbesondere für die Simulation von Spielerdaten in einem Multiplayer-Spiel.

#### Funktionsweise

Der Test validiert die korrekte Funktionalität von Shared-Memory-Operationen durch:

1. **Erstellung eines Shared-Memory-Segments:**
   - Testet die Funktion `createSharedMemory` für verschiedene Spieleranzahlen, einschließlich Grenzwerten wie `0 Spielern`.

2. **Anbindung an das Shared-Memory-Segment:**
   - Verwendet die Funktion `attachSharedMemory`, um den Speicherbereich zu mappen, und überprüft die erfolgreiche Initialisierung.

3. **Datenmanipulation im Shared Memory:**
   - Initialisiert Felder wie `gameName`, `playerCount`, und `playerNumber` und füllt die Spielerinformationen mit simulierten Werten.

4. **Trennung und Entfernung des Shared-Memory-Segments:**
   - Validiert die Funktionen `detachSharedMemory` und `removeSharedMemory`, um sicherzustellen, dass Ressourcen ordnungsgemäß freigegeben werden.

5. **Fehlerbehandlung:**
   - Testet Szenarien mit ungültigen Eingaben, z. B. beim Versuch, nicht existierende Speicherbereiche zu binden oder zu trennen.

#### Ablauf des Tests

1. **Initialisierung:**
   - Shared Memory wird für verschiedene Spieleranzahlen erstellt (z. B. `2`, `0`, `100` Spieler).

2. **Dateninitialisierung und Validierung:**
   - Felder wie `gameName`, `playerCount`, `playerNumber`, und die Informationen der Spieler werden initialisiert und überprüft.
   - Es wird sichergestellt, dass Spieler korrekt registriert sind (abwechselnd `Registered: Yes/No`).

3. **Fehlerbehandlung:**
   - Szenarien wie das Anhängen an ungültigen Speicher (z. B. `shmat` mit `-1`) und das Trennen eines ungültigen Zeigers werden getestet und korrekt gehandhabt.

4. **Grenzwerte:**
   - Der Test evaluiert Grenzfälle wie:
     - `0 Spieler`: Kein Spieler wird initialisiert.
     - `100 Spieler`: Überprüfung der Skalierbarkeit.

5. **Abschluss:**
   - Shared Memory wird getrennt und entfernt, und der erfolgreiche Abschluss des Tests wird ausgegeben.

#### Beispielhafte Ausführung

Beim Ausführen des Tests (`./test_sharedMemory`) sollte eine Ausgabe ähnlich der folgenden erscheinen:

```text
[Test] Creating and initializing shared memory for 2 players
Player 1: Player1, Registered: Yes
Player 2: Player2, Registered: No

[Test] Error handling
shmat failed: Invalid argument
Correctly handled invalid shared memory attach
shmdt failed: Invalid argument
Correctly handled invalid shared memory detach

[Test] Boundary conditions

[Test] Creating and initializing shared memory for 0 players

[Test] Creating and initializing shared memory for 100 players
Player 1: Player1, Registered: Yes
Player 2: Player2, Registered: No
...
Player 100: Player100, Registered: No

All tests completed
```

Ergebnisse

	•	Erfolgreiche Tests:
	•	Spielerinformationen werden korrekt initialisiert.
	•	Fehler werden sicher und ohne Abstürze behandelt.
	•	Grenzwerte:
	•	Keine Probleme bei 0 Spielern oder 100 Spielern.
	•	Fehlerbehandlung:
	•	Ungültige Operationen wie das Anhängen eines ungültigen Speichers werden korrekt abgefangen.

Fazit

Der Test deckt die zentralen Funktionalitäten und mögliche Fehlerfälle des shared_memory.c-Moduls ab. Die Ergebnisse bestätigen die Zuverlässigkeit und Skalierbarkeit der implementierten Shared-Memory-Funktionen.


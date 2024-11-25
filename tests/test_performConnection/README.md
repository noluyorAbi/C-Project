# Tests

Dieses Verzeichnis enthält Testdateien für das Projekt. Hier werden Unit-Tests, Integrationstests und andere Testmethoden abgelegt, um die Funktionalität der einzelnen Module und des gesamten Projekts zu überprüfen.

## Inhalt

- **`test_performConnection/`**: Integrationstest für die `performConnection`-Funktion der TCP-Kommunikationsmodule.

### `test_performConnection/`

#### Zweck

`test_performConnection.c` ist ein Integrationstest, der die `performConnection`-Funktion überprüft. Diese Funktion ist verantwortlich für die Prolog-Phase der Kommunikation mit dem Gameserver, einschließlich des Austauschs von Nachrichten gemäß dem definierten Kommunikationsprotokoll.

#### Funktionsweise

Der Test simuliert einen Gameserver, der erwartete Nachrichten sendet und auf Nachrichten des Clients reagiert. Dies wird erreicht durch:

1. **Erstellung eines Socket-Paares:**

   - Verwendet die Funktion `socketpair()`, um zwei miteinander verbundene Sockets zu erzeugen. Einer dieser Sockets wird vom Mock-Server genutzt, der andere vom Client (`performConnection`).

2. **Start eines Mock-Servers in einem separaten Thread:**

   - Ein neuer Thread wird mittels `pthread_create()` gestartet, der die Mock-Server-Funktion `mock_server()` ausführt.
   - Der Mock-Server sendet vordefinierte Nachrichten an den Client und empfängt Nachrichten vom Client, um das Kommunikationsprotokoll zu simulieren.

3. **Ausführung der `performConnection`-Funktion:**

   - Der Client-Teil des Tests ruft `performConnection()` mit dem entsprechenden Socket-Dateideskriptor auf.
   - Diese Funktion führt die Prolog-Phase durch, sendet Nachrichten an den Mock-Server und verarbeitet die empfangenen Antworten.

4. **Überprüfung der Ergebnisse:**
   - Der Test überprüft, ob `performConnection()` korrekt auf die vom Mock-Server gesendeten Nachrichten reagiert und die erwarteten Ausgaben produziert.
   - Nach Abschluss des Tests werden die Sockets geschlossen und der Mock-Server-Thread beendet.

#### Ablauf des Tests

1. **Initialisierung:**

   - Ein Socket-Paar wird erstellt, um eine bidirektionale Kommunikation zwischen dem Mock-Server und dem Client zu ermöglichen.

2. **Mock-Server-Aktionen:**

   - Sendet eine Begrüßungsnachricht (`+ MNM Gameserver\n`) an den Client.
   - Empfängt die Client-Version (`VERSION 2.42\n`) und sendet eine Bestätigung (`+ Client version accepted\n`).
   - Empfängt die Game-ID (`ID my-game-id\n`) und sendet den Spieltyp (`+ PLAYING NMMorris\n`).
   - Empfängt den PLAYER-Befehl (`PLAYER\n`) und weist dem Client einen Spieler zu (`+ YOU 1 Uli READY\n`), sendet die Gesamtanzahl der Spieler (`+ TOTAL 2\n`) sowie Details zu anderen Spielern (`+ PLAYER 2 Bob NOT_READY\n` und `+ ENDPLAYERS\n`).

3. **Client-Aktionen (`performConnection`):**

   - Führt die empfangenen Nachrichten aus und sendet die entsprechenden Antworten gemäß dem Protokoll.
   - Gibt Ausgaben auf `stdout` für erfolgreiche Aktionen und auf `stderr` für Fehlermeldungen aus.

4. **Abschluss:**
   - Schließt die Sockets und wartet auf die Beendigung des Mock-Server-Threads.
   - Gibt eine Erfolgsmeldung (`Test erfolgreich abgeschlossen.`) aus, wenn alle Schritte ohne Fehler durchgeführt wurden.

#### Beispielhafte Ausführung

Beim Ausführen des Tests (`make test`) sollte eine Ausgabe ähnlich der folgenden erscheinen:

# test_performConnection

## Inhalt des Ordners

Der Ordner `test_performConnection` enthält einen Integrationstest, der die Funktionalität der `performConnection`-Funktion aus den TCP-Kommunikationsmodulen testet. Insbesondere umfasst dieser Ordner die folgende Datei:

- `test_performConnection.c`: Diese Datei implementiert den Integrationstest.

## Funktionalität

Die Datei `test_performConnection.c` dient als Integrationstest für die `performConnection`-Funktion. Ziel dieses Tests ist es, die korrekte Durchführung der Prolog-Phase der Kommunikation zwischen einem Client und einem Gameserver gemäß einem festgelegten Protokoll sicherzustellen.

## Abhängigkeiten

Der Test erfordert die folgende Abhängigkeit:

- `performConnection.h`: Diese Header-Datei befindet sich im Verzeichnis `../../modules/tcp_performConnection/` und definiert die Schnittstelle für die Funktion `performConnection`, die im Test verwendet wird.

Zusätzlich nutzt der Test Standardbibliotheken wie `<arpa/inet.h>`, `<netdb.h>`, `<netinet/in.h>`, `<pthread.h>`, `<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<sys/socket.h>`, und `<unistd.h>` für Socket-Programmierung und Thread-Verwaltung.

## Verwendung

Die Ausführung des Tests erfordert das Kompilieren und Starten der `test_performConnection.c` Datei. Dies kann typischerweise über ein Build-System wie `make` erreicht werden, indem der Befehl `make test` im Hauptverzeichnis des Projekts ausgeführt wird. Der Test gibt detaillierte Protokolle über den Nachrichtenaustausch zwischen dem Mock-Server und dem Client aus, wodurch die Funktionsweise der `performConnection`-Funktion nachvollziehbar wird.

## Interne Struktur

### Ablauf des Tests

1. **Initialisierung:**
   - Ein Socket-Paar wird mit `socketpair()` erstellt, um eine direkte Kommunikation zwischen dem Mock-Server und dem Client zu ermöglichen.

2. **Mock-Server-Aktionen:**
   - Der Mock-Server, implementiert in einem separaten Thread, sendet eine Begrüßungsnachricht und erwartet, dass der Client darauf reagiert.
   - Er überprüft die Client-Version, empfängt die Game-ID, und simuliert den Austausch von Spielerinformationen.

3. **Client-Aktionen (`performConnection`):**
   - Der Client (getestet durch die `performConnection`-Funktion) reagiert auf die Nachrichten des Mock-Servers und agiert gemäß dem Protokoll.

4. **Abschluss:**
   - Am Ende des Tests werden die Sockets geschlossen und der Server-Thread wird beendet. Eine Erfolgsmeldung wird ausgegeben, falls alle Protokollschritte korrekt durchgeführt wurden.

## Tests

Der Integrationstest `test_performConnection.c` überprüft die Funktionalität der `performConnection`-Funktion unter realistischen Kommunikationsbedingungen. Durch den Einsatz eines Mock-Servers wird das Verhalten eines tatsächlichen Gameservers simuliert, wodurch sichergestellt wird, dass der Client ordnungsgemäß Nachrichten sendet und empfängt.

Der Test validiert, ob:
- Der Client auf eine korrekt formatierte Begrüßung reagiert.
- Die Client-Version akzeptiert wird und eine Bestätigung gesendet wird.
- Die Game-ID korrekt verarbeitet wird und der Spieltyp bestätigt wird.
- Spielerinformationen korrekt gesendet und empfangen werden.

Am Ende des Tests wird erwartet, dass eine Meldung `Test erfolgreich abgeschlossen.` erscheint, was bedeutet, dass der Integrationstest ohne Fehler durchlaufen wurde.

## Sonstiges

Es wird empfohlen, den Test regelmäßig durchzuführen, insbesondere nach Änderungen an der `performConnection`-Funktion oder verwandten Modulen, um sicherzustellen, dass die Kommunikation korrekt abläuft und keine neuen Fehler eingeführt wurden.
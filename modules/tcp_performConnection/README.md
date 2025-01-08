# tcp_performConnection

## Inhalt des Ordners

Der Ordner `tcp_performConnection` enthält mehrere Header- und Quellcodedateien, die die Funktionalität zur Erstellung und Verwaltung von TCP-Verbindungen sowie zur Kommunikation mit einem Spielserver bereitstellen. Die enthaltenen Dateien sind:

- `performConnection.h`
- `tcp_connection.h`
- `gameplay.h`
- `tcp_connection.c`
- `performConnection.c`
- `gameplay.c`

## Funktionalität

Dieser Modulordner stellt die notwendige Logik zur Verfügung, um eine TCP-Verbindung zu einem Spielserver herzustellen und die Protokollkommunikation zu managen. Dies umfasst das Senden und Empfangen von Nachrichten, die Handhabung von Spielphasen und das Verwalten der Verbindung bis zur Beendigung des Spiels.

### Wichtige Funktionen

1. **createConnection**: Baut eine Verbindung zum Spielserver auf und führt das Kommunikationsprotokoll aus. Die Verbindung wird über die IP-Adresse und den Port hergestellt, die in der Hostadresse definiert sind.

2. **performConnection**: Führt die Prozedur der Verbindung gemäß dem Kommunikationsprotokoll aus. Es empfängt Begrüßungsnachrichten, sendet Client-Versionen und Spiel-ID und verwaltet den Übergang zwischen den verschiedenen Spielphasen.

3. **sendMessage und receiveMessage**: Diese Funktionen sind für das Senden und Empfangen von Nachrichten über den Socket verantwortlich. Sie implementieren grundlegende Fehlerbehandlungen und stellen sicher, dass Nachrichten korrekt übertragen werden.

4. **handleWait, handleMove, handleGameover**: Diese Funktionen sind dafür verantwortlich, die jeweilige Spielphase zu managen. Sie empfangen Nachrichten, aktualisieren den Spielstatus und reagieren auf spezifische Befehle des Spielservers.

## Abhängigkeiten

Die Implementierung basiert auf UNIX-spezifischen Bibliotheken und Funktionen, um Netzwerk- und Interprozesskommunikation zu ermöglichen, darunter:

- `<sys/socket.h>`
- `<netdb.h>`
- `<unistd.h>`
- `<arpa/inet.h>`

Weitere Abhängigkeiten umfassen Standard-C-Bibliotheken wie `<stdio.h>`, `<stdlib.h>`, `<string.h>`, sowie `<signal.h>` zur Signalbehandlung.

## Verwendung

Um diese Implementierung zu verwenden, müssen entsprechende Verbindungen zu einem Spielserver mit der spezifischen IP und Portadresse (`HOSTNAME` und `PORT` in `tcp_connection.c`) konfiguriert werden. Eine typischerweise aufgerufene Funktion ist `createConnection`, die die Verbindung aufbaut und die Kommunikation initiiert.

## Interne Struktur

- **Header-Dateien (`.h`)**: Definieren die Schnittstellen für die jeweilige Funktionalität, die in den dazugehörigen `.c`-Dateien implementiert ist. Jede Header-Datei erklärt die Funktionalität der Methoden, die in der Implementierung bereitgestellt werden.
  
- **Quellcodedateien (`.c`)**: Enthalten die eigentliche Implementierung der Funktionen, die in den Header-Dateien deklariert sind. Sie verwalten die Sockets, führen die Spieleprotokolle aus und bearbeiten die verschiedenen Spielphasen.

## Tests

Es liegen keine expliziten Informationen zu Tests vor, basierend auf den vorhandenen Datei- und Inhaltsdetails. Es ist jedoch wichtig, dass Implementierungen, die Netzwerkkommunikation involvieren, gründlich getestet werden, insbesondere in Bezug auf Robustheit und Fehlermanagement.

## Sonstiges

Eine ordnungsgemäße Signalbehandlung ist in die Implementierung eingebaut, um reibungslose Kommunikation zwischen verschiedenen Prozessen zu ermöglichen. Das Erstellen und Verwalten von Verbindungen basiert auf den aktuellen Internet-Protokollen, wobei sowohl IPv4 als auch IPv6 unterstützt werden. Es ist wichtig, sicherzustellen, dass der Spielserver, zu dem die Verbindung aufgebaut wird, den erwarteten Kommunikationsstandard unterstützt.
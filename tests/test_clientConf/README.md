# Modul/Ordnername: test_clientConf

## Inhalt des Ordners
Der Ordner `test_clientConf` enthält ein Skript mit dem Namen `test_clientConf.sh`. Dieses Skript wird verwendet, um die Konfigurationsdatei `client.conf` sowie die Argumentenverarbeitung des Programms `sysprak-client` zu testen.

## Funktionalität
Das Skript `test_clientConf.sh` ist darauf ausgelegt, verschiedene Szenarien im Zusammenhang mit der Verwendung und Verarbeitung von Konfigurationsdateien und Kommandozeilenargumenten zu testen. Es simuliert sowohl gültige als auch ungültige Eingaben, um die Robustheit und Fehlerbehandlung des Programms `sysprak-client` zu überprüfen.

### Wichtige Aufgaben des Skripts:
- Erstellung von verschiedenen Testkonfigurationsdateien mit unterschiedlichen Inhaltsszenarien.
- Ausführung des Programms `sysprak-client` mit verschiedenen Konfigurationsdateien und Argumenten.
- Überprüfung der Ausgabe und des Exit-Codes des Programms im Vergleich zu den erwarteten Werten.
- Zusammenfassung der Testergebnisse und Entscheidung, ob erstellte Dateien erhalten oder gelöscht werden sollen.

## Abhängigkeiten
- `bash`: Das Skript ist ein Bash-Skript und benötigt daher eine Bash-Umgebung zur Ausführung.
- `sysprak-client`: Ein ausführbares Programm, das sich im Verzeichnis `../../bin/` relativ zum Speicherort des Skripts befinden muss.

## Verwendung
Das Skript wird ausgeführt, um automatisierte Tests zu implementieren, welche die Robustheit und Korrektheit der Konfigurationsverarbeitung des `sysprak-client` sicherstellen. Dazu werden mehrere Szenarien definiert und jeweils geprüft, ob der `sysprak-client` sich wie erwartet verhält.

1. Vergewissern Sie sich, dass das ausführbare Programm `sysprak-client` im Pfad `../../bin/` vorhanden und ausführbar ist.
2. Führen Sie das Skript `test_clientConf.sh` aus, um alle definierten Testszenarien zu prüfen.

## Interne Struktur
Das Skript `test_clientConf.sh` verwendet eine Reihe von definierten Szenarien, die unterschiedliche Konfigurationen und Argumentensätze repräsentieren. Dazu zählen:

- Gültige und ungültige `GAME-ID`s.
- Fehlerhafte oder unvollständige Konfigurationsdateien.
- Überprüfung von unbekannten Parametern in der Konfiguration und den Kommandozeilenargumenten.
- Verarbeitung von Kommentaren, zusätzlichen Whitespaces und Großschreibung in Konfigurationsschlüsseln.

Jedes Szenario ist als Funktion `run_test` im Skript implementiert, die den Test durchführt und die Ergebnisse bewertet.

## Tests
Das Skript führt mehrere Tests durch, indem es das `sysprak-client`-Programm mit verschiedenen Kombinationen aus Konfigurationsdateien und Argumenten aufruft. Nach jedem Test wird die tatsächliche Ausgabe mit der erwarteten verglichen und ein Bericht generiert. Am Ende des Skripts wird eine Zusammenfassung der bestandenen und nicht bestandenen Tests angezeigt.

## Sonstiges
Nach dem Abschluss der Tests besteht die Möglichkeit, alle erstellten Dateien und Testprotokolle zu löschen. Dies erfolgt interaktiv über eine Benutzereingabe. Hierbei wird darauf geachtet, dass nur Dateien gelöscht werden, die vom Skript erzeugt wurden.
# C-Project

Hier ist ein allgemeines **README**, das ihr als Grundlage verwenden könnt, und das während der Entwicklung angepasst und erweitert werden kann.

---

# Neunermühle-Client für das Systempraktikum

## Projektübersicht

Willkommen im Systempraktikum der **Ludwig-Maximilians-Universität München**. Im Rahmen dieses Praktikums entwickeln wir einen **Client für das strategische Brettspiel Neunermühle** in der Programmiersprache **C**. Ziel ist es, die Logik des Spiels zu implementieren und den Client über eine TCP-Verbindung mit einem zentralen Gameserver zu verbinden. Der Gameserver überwacht das Spielgeschehen und stellt sicher, dass die Regeln eingehalten werden, während unser Client als aktiver Mitspieler agiert.

Das Projekt wird in mehreren Schritten im Verlauf des Praktikums entwickelt, und jeder Schritt führt uns näher zur fertigen Version des Clients.

## Projektanforderungen

- **Programmiersprache**: C
- **Netzwerkprotokoll**: TCP zur Kommunikation mit dem Gameserver
- **Systemzugriff**: Nutzung von Shared Memory zur Speicherung von Spielfeldinformationen
- **Zugriff auf den Gameserver**: Nur über das Münchener Wissenschaftsnetzwerk (MWN) möglich

## Entwicklungsphasen

Das Projekt gliedert sich in mehrere Phasen, die durch Übungsblätter begleitet werden. In jeder Phase erweitern wir den Funktionsumfang des Clients schrittweise, sodass am Ende ein funktionsfähiger Neunermühle-Client entsteht.

### Hauptfunktionen

1. **Verbindung zum Gameserver**: Herstellung einer TCP-Verbindung und Kommunikation mit dem Server zur Durchführung der Spielzüge.
2. **Spielelogik**: Implementierung der Spielregeln und -strategie für Neunermühle.
3. **Signalverarbeitung**: Reagieren auf Server-Signale und Spielerinteraktionen.
4. **Shared Memory**: Speichern und Abrufen des Spielfeldzustands.

## Zugriff auf Gameserver und Webinterface

Das **Webinterface** ist unter folgender Adresse verfügbar:
[http://sysprak.priv.lab.nm.ifi.lmu.de](http://sysprak.priv.lab.nm.ifi.lmu.de)

Um das Webinterface und den Gameserver aus dem MWN zu erreichen, könnt ihr eine SSH-Portweiterleitung zu einem CIP-Rechner nutzen:

```bash
ssh -L localhost:PORT:sysprak.priv.lab.nm.ifi.lmu.de:PORT username@cip.ifi.lmu.de
```

### Funktionen des Webinterfaces

Das Webinterface ermöglicht das Erstellen von Partien, in denen der Client gegen andere Spieler antreten oder gegen sich selbst getestet werden kann. Zudem kann man als Beobachter in eine Partie einsteigen, um Einblicke in den Spielablauf zu erhalten und die korrekte Funktion des Clients zu überprüfen.

## Spielregeln und Ressourcen

Die Spielregeln für Neunermühle und weitere Hilfen zur Entwicklung des Clients findet ihr unter folgendem Link:
[http://sysprak.priv.lab.nm.ifi.lmu.de/help/](http://sysprak.priv.lab.nm.ifi.lmu.de/help/)

---

## Installation und Ausführung

1. **Projekt klonen**:

   ```bash
   git clone <repository-url>
   cd neunermuehle-client
   ```

2. **Kompilieren des Clients**:

   ```bash
   make
   ```

3. **Ausführung**:
   ```bash
   ./neunermuehle_client
   ```

## Mitwirkende

Dieses Projekt wird von Studierenden der LMU im Rahmen des Systempraktikums entwickelt.

## Lizenz

Dies ist ein universitätsinternes Projekt im Rahmen des Systempraktikums der LMU München.

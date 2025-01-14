<p align="center">
    <img src="https://upload.wikimedia.org/wikipedia/commons/1/18/C_Programming_Language.svg" align="center" width="30%">
</p>
<h1 align="center">Neunermühle-Client</h1>
<p align="center"><em>Client für das Systempraktikum der LMU München</em></p>

<p align="center">
	<img src="https://img.shields.io/github/license/noluyorAbi/C-Project?style=for-the-badge&logo=opensourceinitiative&logoColor=white&color=0080ff" alt="license">
	<img src="https://img.shields.io/github/last-commit/noluyorAbi/C-Project?style=for-the-badge&logo=git&logoColor=white&color=0080ff" alt="last-commit">
	<img src="https://img.shields.io/github/languages/top/noluyorAbi/C-Project?style=for-the-badge&color=0080ff" alt="repo-top-language">
	<img src="https://img.shields.io/github/languages/count/noluyorAbi/C-Project?style=for-the-badge&color=0080ff" alt="repo-language-count">
</p>
<p align="center"><!-- default option, no dependency badges. -->
  <!-- default option, no dependency badges. -->

 <a href="https://github.com/noluyorAbi/C-Project/actions/workflows/ci-format.yaml">
    <img src="https://github.com/noluyorAbi/C-Project/actions/workflows/ci-format.yaml/badge.svg" alt="CI - Format Check">
  </a>

<a href="https://github.com/noluyorAbi/C-Project/actions/workflows/flawfinder.yml">
  <img src="https://github.com/noluyorAbi/C-Project/actions/workflows/flawfinder.yml/badge.svg" alt="flawfinder">
</a>

<a href="https://codescene.io/projects/61375">
<img src="https://codescene.io/projects/61375/status-badges/code-health" alt="Code Health">
</a>

</p>

<p align="center">
	<!-- default option, no dependency badges. -->
</p>

<br/>

<h4 align="center">Made with ♥ by:</h4>

<p align="center">
  <a href="https://github.com/noluyorAbi/C-Project/graphs/contributors">
    <img src="https://contrib.rocks/image?repo=noluyorAbi/C-Project" />
  </a>
</p>

<br/>

## Inhaltsverzeichnis

- [Inhaltsverzeichnis](#inhaltsverzeichnis)
- [Überblick](#überblick)
- [Funktionen](#funktionen)
- [Projektstruktur](#projektstruktur)
  - [Übersicht der Verzeichnisse und Dateien](#übersicht-der-verzeichnisse-und-dateien)
- [Erste Schritte](#erste-schritte)
  - [Voraussetzungen](#voraussetzungen)
  - [Installation](#installation)
  - [Benutzung](#benutzung)
- [Zugriff auf Gameserver und Webinterface](#zugriff-auf-gameserver-und-webinterface)
  - [Funktionen des Webinterfaces](#funktionen-des-webinterfaces)
- [Spielregeln und Ressourcen](#spielregeln-und-ressourcen)
- [Projekt-Roadmap](#projekt-roadmap)
- [Beitragende](#beitragende)
- [Richtlinien für Beiträge](#richtlinien-für-beiträge)
- [Lizenz](#lizenz)
- [Danksagungen](#danksagungen)

---

## Überblick

Willkommen im Systempraktikum der **Ludwig-Maximilians-Universität München**. Im Rahmen dieses Praktikums entwickeln wir einen **Client für das strategische Brettspiel Neunermühle** in der Programmiersprache **C**. Ziel ist es, die Logik des Spiels zu implementieren und den Client über eine TCP-Verbindung mit einem zentralen Gameserver zu verbinden. Der Gameserver überwacht das Spielgeschehen und stellt sicher, dass die Regeln eingehalten werden, während unser Client als aktiver Mitspieler agiert.

Das Projekt wird in mehreren Schritten im Verlauf des Praktikums entwickelt, und jeder Schritt führt uns näher zur fertigen Version des Clients.

---

## Funktionen

|     |        Feature         | Zusammenfassung                                                                                                  |
| :-- | :--------------------: | :--------------------------------------------------------------------------------------------------------------- |
| ⚙️  | **Programmiersprache** | C                                                                                                                |
| 🔌  | **Netzwerkprotokoll**  | TCP zur Kommunikation mit dem Gameserver                                                                         |
| 🗃️  |   **Systemzugriff**    | Nutzung von Shared Memory zur Speicherung von Spielfeldinformationen                                             |
| 🌐  | **Gameserver-Zugriff** | Nur über das Münchener Wissenschaftsnetzwerk (MWN) möglich                                                       |
| 🧩  |  **Hauptfunktionen**   | <ul><li>Verbindung zum Gameserver</li><li>Spielelogik</li><li>Signalverarbeitung</li><li>Shared Memory</li></ul> |

---

## Projektstruktur

<!-- project-structure-start -->
```sh
.
├── LICENSE
├── README.md
├── bin
│   └── sysprak-client
├── client.conf
├── compile_commands.json
├── lib
│   └── README.md
├── main.c
├── makefile
├── modules
│   ├── README.md
│   ├── args_parser
│   │   ├── README.md
│   │   ├── args_parser.c
│   │   ├── args_parser.h
│   │   ├── config.c
│   │   ├── config.h
│   │   └── constants.h
│   ├── shared_memory
│   │   ├── README.md
│   │   ├── shared_memory.c
│   │   └── shared_memory.h
│   ├── tcp_performConnection
│   │   ├── README.md
│   │   ├── gameplay.c
│   │   ├── gameplay.h
│   │   ├── performConnection.c
│   │   ├── performConnection.h
│   │   ├── tcp_connection.c
│   │   └── tcp_connection.h
│   └── think
│       ├── README.md
│       ├── mcts.c
│       ├── mcts.h
│       ├── think.c
│       ├── think.h
│       ├── util.c
│       └── util.h
├── scripts
│   ├── Readme.md
│   ├── setup_precommit
│   │   ├── README.md
│   │   └── setup_pre_commit.sh
│   └── update_readme
│       ├── README.md
│       └── update_readme.sh
└── tests
    ├── README.md
    ├── test_clientConf
    │   ├── README.md
    │   └── test_clientConf.sh
    ├── test_performConnection
    │   ├── README.md
    │   └── test_performConnection.c
    └── test_sharedMemory
        ├── README.md
        ├── test_sharedMemory.bash
        └── test_sharedMemory.c

15 directories, 45 files
```
<!-- project-structure-end -->

### Übersicht der Verzeichnisse und Dateien

1. **`README.md`**: Enthält die Projektdokumentation, wie Ziel, Funktionen und Aufbau des Projekts.
2. **`compile_commands.json`**: Nützlich für die Integration mit Tools wie Clang, um die Kompilierungsschritte zu verfolgen.
3. **`docs/`**: Alle projektbezogenen Dokumentationen, strukturiert und separat gehalten.
4. **`lib/`**: Bibliotheken oder externe Abhängigkeiten, die das Projekt verwendet.
5. **`main.c`**: Zentrale Einstiegsdatei, in der das Programm startet.
6. **`makefile`**: Automatisiert den Build-Prozess und erleichtert das Kompilieren.
7. **`modules/`**: Organisiert die Funktionalitäten des Projekts in wiederverwendbare Module:
   - **`args_parser`**: Verarbeitung von Kommandozeilenargumenten.
   - **`tcp_performConnection`**: Netzwerkmodule zur Verwaltung von TCP-Verbindungen.
8. **`scripts/`**: Praktische Automatisierungsskripte, z. B. für Pre-Commit-Hooks oder Dokumentationsaktualisierungen.
9. **`tests/`**: Verzeichnis für Tests, die sicherstellen, dass der Code wie erwartet funktioniert.

---

## Erste Schritte

### Voraussetzungen

Bevor Sie beginnen, stellen Sie sicher, dass folgende Voraussetzungen erfüllt sind:

- **Programmiersprache**: C
- **Compiler**: Ein C-Compiler wie GCC oder Clang
- **Make**: Zur Verwendung des Makefiles
- **Netzwerkzugriff**: Zugang zum Münchener Wissenschaftsnetzwerk (MWN)

### Installation

1. **Projekt klonen**:

_Github:_

```sh
git clone https://github.com/noluyorAbi/C-Project
```

_Gitlab_ (Mirror-Repository von Github)

```sh
git clone https://gitlab.lrz.de/sysprakws2425/gruppe18/c-project-mirror
```

2. **Kompilieren des Clients**:

   ```sh
   make
   ```

   Dies erstellt die ausführbare Datei und legt sie im `bin/` Verzeichnis ab.

### Benutzung

Um den Client zu starten, verwenden Sie:

```sh
make run
```

---

## Zugriff auf Gameserver und Webinterface

Das **Webinterface** ist unter folgender Adresse verfügbar:

[http://sysprak.priv.lab.nm.ifi.lmu.de](http://sysprak.priv.lab.nm.ifi.lmu.de)

Um das Webinterface und den Gameserver aus dem MWN zu erreichen, können Sie eine SSH-Portweiterleitung zu einem CIP-Rechner nutzen:

```sh
ssh -L localhost:PORT:sysprak.priv.lab.nm.ifi.lmu.de:PORT username@cip.ifi.lmu.de
```

### Funktionen des Webinterfaces

Das Webinterface ermöglicht das Erstellen von Partien, in denen der Client gegen andere Spieler antreten oder gegen sich selbst getestet werden kann. Zudem kann man als Beobachter in eine Partie einsteigen, um Einblicke in den Spielablauf zu erhalten und die korrekte Funktion des Clients zu überprüfen.

---

## Spielregeln und Ressourcen

Die Spielregeln für Neunermühle und weitere Hilfen zur Entwicklung des Clients finden Sie unter folgendem Link:

[http://sysprak.priv.lab.nm.ifi.lmu.de/help/](http://sysprak.priv.lab.nm.ifi.lmu.de/help/)

---

## Projekt-Roadmap

- [x] **Phase 1**: Verbindung zum Gameserver herstellen
- [ ] **Phase 2**: Spielelogik implementieren
- [ ] **Phase 3**: Signalverarbeitung hinzufügen
- [ ] **Phase 4**: Shared Memory nutzen

---

## Beitragende

Dieses Projekt wird von Studierenden der LMU im Rahmen des Systempraktikums entwickelt.

- **💬 [Diskussionen beitreten](https://github.com/noluyorAbi/C-Project/discussions)**: Teilen Sie Ihre Ideen, Feedback oder stellen Sie Fragen.
- **🐛 [Probleme melden](https://github.com/noluyorAbi/C-Project/issues)**: Reichen Sie gefundene Bugs ein oder stellen Sie Feature-Anfragen.
- **💡 [Pull Requests einreichen](https://github.com/noluyorAbi/C-Project/pulls)**: Überprüfen Sie offene PRs oder reichen Sie eigene PRs ein.

## Richtlinien für Beiträge

⚠️ **Wichtiger Hinweis:**
Da dieses Projekt ein modulinternes Projekt ist, können leider keine Beiträge von Außenstehenden berücksichtigt werden.

---

## Lizenz

Dieses Projekt steht unter der [MIT License](https://choosealicense.com/licenses/mit/). Weitere Details finden Sie in der [LICENSE](https://github.com/noluyorAbi/neunermuehle-client/blob/main/LICENSE) Datei.

---

## Danksagungen

- Wir danken der Ludwig-Maximilians-Universität München für die Bereitstellung dieses Praktikums.
- Besonderer Dank gilt den Betreuern und Professoren für ihre Unterstützung.

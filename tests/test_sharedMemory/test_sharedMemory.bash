#!/bin/bash

# test_shared_memory.sh
# Skript zum Testen der Shared Memory Funktionalität.

# Definieren Sie Farbvariablen für die Ausgabe
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Pfade setzen
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEST_PROGRAM="$SCRIPT_DIR/../../build/test_sharedMemory" # Angepasster Pfad zum Testprogramm
MODULE_DIR="$SCRIPT_DIR/../../modules/shared_memory" # Pfad zu den Moduldateien
LOG_DIR="$SCRIPT_DIR/logs"

# Sicherstellen, dass das Log-Verzeichnis existiert
mkdir -p "$LOG_DIR"

# Sicherstellen, dass das build-Verzeichnis existiert
mkdir -p "$(dirname "$TEST_PROGRAM")"

# Header-Datei für shared_memory einbinden, falls noch nicht geschehen
if [ ! -f "$MODULE_DIR/shared_memory.h" ]; then
  echo -e "${RED}Fehler: Header-Datei shared_memory.h fehlt.${NC}"
  exit 1
fi

# Kompilierbefehl mit expliziter Einbindung der Header-Datei
COMPILE_CMD="gcc -o \"$TEST_PROGRAM\" \"$SCRIPT_DIR/test_sharedMemory.c\" \"$MODULE_DIR/shared_memory.c\" -Wall -I\"$MODULE_DIR\""

# Testprogramm kompilieren, falls es nicht existiert oder die Quelldateien neuer sind
if [ ! -x "$TEST_PROGRAM" ] || [ "$SCRIPT_DIR/test_sharedMemory.c" -nt "$TEST_PROGRAM" ] || [ "$MODULE_DIR/shared_memory.c" -nt "$TEST_PROGRAM" ]; then
  echo "Kompiliere Testprogramm..."
  eval "$COMPILE_CMD"
  if [ $? -ne 0 ]; then
    echo -e "${RED}Fehler beim Kompilieren des Testprogramms.${NC}"
    exit 1
  fi
fi

# Funktion zur Bereinigung von verbleibenden Shared-Memory-Segmenten
cleanup_after_test() {
    echo "Bereinige verbleibende Shared-Memory-Segmente..."
    # Überprüfen Sie, ob Shared-Memory-Segmente vorhanden sind und entfernen Sie diese
    ipcs -m | grep "$USER" | awk '{print $2}' | while read -r shmid; do
        ipcrm -m "$shmid"
        printf "Entferne Shared Memory Segment: %s\n" "$shmid"
    done
}

# Führen Sie die Bereinigung vor den Tests durch, um sicherzustellen, dass keine alten Segmente vorhanden sind
cleanup_after_test

# Trap einrichten, um bei Abbruch des Skripts (z.B. durch Ctrl+C) eine Bereinigung durchzuführen
trap cleanup_after_test EXIT

# Programm ausführen und Logs erfassen
echo "===== Führe Shared Memory Tests aus ====="
"$TEST_PROGRAM" > "$LOG_DIR/test_sharedMemory.out" 2> "$LOG_DIR/test_sharedMemory.err"
EXIT_CODE=$?

# Zähler für Pass und Fail initialisieren
PASS_COUNT=0
FAIL_COUNT=0

# Pass- und Fail-Zähler aus dem Log extrahieren
PASS_COUNT=$(grep -c "\[PASS\]" "$LOG_DIR/test_sharedMemory.out")
FAIL_COUNT=$(grep -c "\[FAIL\]" "$LOG_DIR/test_sharedMemory.out")

# Zusammenfassung anzeigen
echo -e "\n===== Testzusammenfassung ====="
echo -e "${GREEN}Bestanden: $PASS_COUNT${NC}"
echo -e "${RED}Fehlgeschlagen: $FAIL_COUNT${NC}"

# Detaillierte Fehlermeldungen anzeigen, falls vorhanden
if [ "$FAIL_COUNT" -ne 0 ]; then
  echo -e "\n${RED}Einige Tests sind fehlgeschlagen. Details siehe unten:${NC}"
  echo "---- Inhalt von test_sharedMemory.out ----"
  grep "\[FAIL\]" "$LOG_DIR/test_sharedMemory.out"
  echo "---- Inhalt von test_sharedMemory.err ----"
  cat "$LOG_DIR/test_sharedMemory.err"
fi

# Optional: Gesamten Log anzeigen
echo -e "\n---- Gesamter Test-Log ----"
 cat "$LOG_DIR/test_sharedMemory.out"
 cat "$LOG_DIR/test_sharedMemory.err"

# Aufräumen von nicht benötigten Logdateien (optional)
# rm -f "$LOG_DIR/test_sharedMemory.out" "$LOG_DIR/test_sharedMemory.err"

# Rückgabe des Gesamtstatus
if [ "$FAIL_COUNT" -ne 0 ]; then
    exit 1
else
    exit 0
fi

#!/bin/bash

# test_clientConf.sh
# Skript zum Testen der client.conf und der Argumentverarbeitung

# Definiere Farbvariablen
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Pfade festlegen
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROGRAM="$SCRIPT_DIR/../../bin/sysprak-client"  # Korrigierter Pfad zum ausführbaren Programm
CONFIG_DIR="$SCRIPT_DIR"  # Konfigurationsdateien befinden sich im aktuellen Verzeichnis
LOG_DIR="$SCRIPT_DIR/logs"

# Sicherstellen, dass die Verzeichnisse existieren
mkdir -p "$LOG_DIR"

# Funktion zum Testen eines Szenarios
run_test() {
    local test_name="$1"
    local config_file="$2"         # Pfad zur Konfigurationsdatei, leer für Standard
    local args="$3"                # Zusätzliche Argumente
    local expected_output="$4"     # Erwartete Ausgabe
    local expected_exit_code="$5"  # Erwarteter Exit-Code

    printf "===== Test: %s =====\n" "$test_name"

    # Führe das Programm aus
    if [ -z "$config_file" ]; then
        # Keine Konfigurationsdatei angegeben, benutze Standard
        set -- $args
        "$PROGRAM" "$@" > "$LOG_DIR/$test_name.out" 2> "$LOG_DIR/$test_name.err"
    else
        # Konfigurationsdatei angegeben
        set -- $args
        "$PROGRAM" "$@" -c "$config_file" > "$LOG_DIR/$test_name.out" 2> "$LOG_DIR/$test_name.err"
    fi
    local exit_code=$?

    # Überprüfe den Exit-Code
    if [ "$exit_code" -ne "$expected_exit_code" ]; then
        printf "${RED}FAIL:${NC} Erwarteter Exit-Code %d, aber erhalten %d\n" "$expected_exit_code" "$exit_code"
        # Ausgabe der Fehlermeldungen zur Fehleranalyse
        printf "---- Inhalt von %s.err ----\n" "$test_name"
        cat "$LOG_DIR/$test_name.err"
        printf "-----------------------------------\n\n"
        FAIL_COUNT=$((FAIL_COUNT + 1))
        return
    fi

    # Überprüfe die Ausgabe in stdout und stderr
    if grep -q "$expected_output" "$LOG_DIR/$test_name.out" || grep -q "$expected_output" "$LOG_DIR/$test_name.err"; then
        printf "${GREEN}PASS:${NC} Erwartete Ausgabe gefunden.\n\n"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        printf "${RED}FAIL:${NC} Erwartete Ausgabe '%s' nicht gefunden.\n" "$expected_output"
        # Ausgabe der aktuellen Ausgaben zur Fehleranalyse
        printf "---- Inhalt von %s.out ----\n" "$test_name"
        cat "$LOG_DIR/$test_name.out"
        printf "---- Inhalt von %s.err ----\n" "$test_name"
        cat "$LOG_DIR/$test_name.err"
        printf "-----------------------------------\n\n"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
}

# Sicherstellen, dass das ausführbare Programm existiert und ausführbar ist
if [ ! -x "$PROGRAM" ]; then
    printf "${RED}Fehler:${NC} Programm '%s' existiert nicht oder ist nicht ausführbar.\n" "$PROGRAM"
    exit 1
fi

# Initialisieren der Zähler
PASS_COUNT=0
FAIL_COUNT=0

# Szenario 0: Erstellen der Standard-Konfigurationsdatei für den Test
cat > "$CONFIG_DIR/client.conf" <<EOL
Hostname = sysprak.priv.lab.nm.ifi.lmu.de
PortNumber = 1357
GameKindName = NMMorris
EOL

# Szenario 1: Gültige client.conf und alle Argumente korrekt
cat > "$CONFIG_DIR/valid.conf" <<EOL
Hostname = sysprak.priv.lab.nm.ifi.lmu.de
PortNumber = 1357
GameKindName = NMMorris
EOL

run_test "Gültige Konfiguration" "$CONFIG_DIR/valid.conf" "-g 1234567890123 -p 1" "Hostname: sysprak.priv.lab.nm.ifi.lmu.de" 0

# Szenario 2: Fehlende client.conf
run_test "Fehlende Konfigurationsdatei" "/nonexistent/client.conf" "-g 1234567890123" "Error opening configuration file" 1

# Szenario 3: Ungültige GAME-ID (zu kurz)
run_test "Ungültige GAME-ID (zu kurz)" "$CONFIG_DIR/valid.conf" "-g 12345" "Fehler: GAME-ID muss genau 13 Stellen lang sein." 1

# Szenario 4: Ungültige GAME-ID (nicht numerisch)
run_test "Ungültige GAME-ID (nicht numerisch)" "$CONFIG_DIR/valid.conf" "-g 1234567890abc" "Fehler: GAME-ID muss genau 13 Stellen lang sein." 1

# Szenario 5: Ungültige Spielerzahl
run_test "Ungültige Spielerzahl" "$CONFIG_DIR/valid.conf" "-g 1234567890123 -p 3" "Fehler: Spielerzahl muss entweder 1 oder 2 sein." 1

# Szenario 6: Fehlender GAME-ID Parameter
run_test "Fehlender GAME-ID Parameter" "$CONFIG_DIR/valid.conf" "-p 1" "Fehler: Fehlendes oder ungültiges Pflichtargument -g <GAME-ID>." 1

# Szenario 7: Gültige Konfiguration mit Standardwerten (ohne -p)
run_test "Gültige Konfiguration mit Standardwerten" "$CONFIG_DIR/valid.conf" "-g 1234567890123" "Spielerzahl: 1" 0

# Szenario 8: Verwendung der Standard-Konfigurationsdatei ohne Angabe von -c
run_test "Verwendung der Standard-Konfigurationsdatei" "" "-g 9876543210987 -p 2" "Hostname: sysprak.priv.lab.nm.ifi.lmu.de" 0

# Zusammenfassung
printf "===== Testzusammenfassung =====\n"
printf "${GREEN}Bestanden: %d${NC}\n" "$PASS_COUNT"
printf "${RED}Fehlgeschlagen: %d${NC}\n" "$FAIL_COUNT"

# Gesamtstatus zurückgeben
if [ "$FAIL_COUNT" -ne 0 ]; then
    exit 1
else
    exit 0
fi

#!/bin/bash

# test_clientConf.sh
# Script for testing the client.conf file and argument handling

# Define color variables
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Set paths
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROGRAM="$SCRIPT_DIR/../../bin/sysprak-client"  # Adjusted path to the executable program
CONFIG_DIR="$SCRIPT_DIR"  # Configuration files are located in the current directory
LOG_DIR="$SCRIPT_DIR/logs"

# Ensure directories exist
mkdir -p "$LOG_DIR"

# Function to test a scenario
run_test() {
    local test_name="$1"
    local config_file="$2"         # Path to the configuration file, empty for default
    local args="$3"                # Additional arguments
    local expected_output="$4"     # Expected output
    local expected_exit_code="$5"  # Expected exit code

    printf "===== Test: %s =====\n" "$test_name"

    # Run the program
    if [ -z "$config_file" ]; then
        # No configuration file specified, use default
        set -- $args
        "$PROGRAM" "$@" > "$LOG_DIR/$test_name.out" 2> "$LOG_DIR/$test_name.err"
    else
        # Configuration file specified
        set -- $args
        "$PROGRAM" "$@" -c "$config_file" > "$LOG_DIR/$test_name.out" 2> "$LOG_DIR/$test_name.err"
    fi
    local exit_code=$?

    # Check the exit code
    if [ "$exit_code" -ne "$expected_exit_code" ]; then
        printf "${RED}FAIL:${NC} Erwarteter Exit-Code %d, aber erhalten %d\n" "$expected_exit_code" "$exit_code"
        # Output error messages for analysis
        printf "---- Inhalt von %s.err ----\n" "$test_name"
        cat "$LOG_DIR/$test_name.err"
        printf "-----------------------------------\n\n"
        FAIL_COUNT=$((FAIL_COUNT + 1))
        return
    fi

    # Check the output in stdout and stderr
    if grep -q "$expected_output" "$LOG_DIR/$test_name.out" || grep -q "$expected_output" "$LOG_DIR/$test_name.err"; then
        printf "${GREEN}PASS:${NC} Erwartete Ausgabe gefunden.\n\n"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        printf "${RED}FAIL:${NC} Erwartete Ausgabe '%s' nicht gefunden.\n" "$expected_output"
        # Output current outputs for analysis
        printf "---- Inhalt von %s.out ----\n" "$test_name"
        cat "$LOG_DIR/$test_name.out"
        printf "---- Inhalt von %s.err ----\n" "$test_name"
        cat "$LOG_DIR/$test_name.err"
        printf "-----------------------------------\n\n"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
}

# Ensure the executable program exists and is executable
if [ ! -x "$PROGRAM" ]; then
    printf "${RED}Fehler:${NC} Programm '%s' existiert nicht oder ist nicht ausführbar.\n" "$PROGRAM"
    exit 1
fi

# Initialize counters
PASS_COUNT=0
FAIL_COUNT=0

# Scenario 0: Create the default configuration file for testing
cat > "$CONFIG_DIR/client.conf" <<EOL
Hostname = sysprak.priv.lab.nm.ifi.lmu.de
PortNumber = 1357
GameKindName = NMMorris
EOL

# Scenario 1: Valid client.conf and all arguments correct
cat > "$CONFIG_DIR/valid.conf" <<EOL
Hostname = sysprak.priv.lab.nm.ifi.lmu.de
PortNumber = 1357
GameKindName = NMMorris
EOL

run_test "Gültige Konfiguration" "$CONFIG_DIR/valid.conf" "-g 1234567890123 -p 1" "Hostname: sysprak.priv.lab.nm.ifi.lmu.de" 0

# Scenario 2: Missing client.conf
run_test "Fehlende Konfigurationsdatei" "/nonexistent/client.conf" "-g 1234567890123" "Error opening configuration file" 1

# Scenario 3: Invalid GAME-ID (too short)
run_test "Ungültige GAME-ID (zu kurz)" "$CONFIG_DIR/valid.conf" "-g 12345" "Fehler: GAME-ID muss genau 13 Stellen lang sein." 1

# Scenario 4: Invalid GAME-ID (non-numeric)
run_test "Ungültige GAME-ID (nicht numerisch)" "$CONFIG_DIR/valid.conf" "-g 1234567890abc" "Fehler: GAME-ID muss genau 13 Stellen lang sein." 1

# Scenario 5: Invalid player number
run_test "Ungültige Spielerzahl" "$CONFIG_DIR/valid.conf" "-g 1234567890123 -p 3" "Fehler: Spielerzahl muss entweder 1 oder 2 sein." 1

# Scenario 6: Missing GAME-ID parameter
run_test "Fehlender GAME-ID Parameter" "$CONFIG_DIR/valid.conf" "-p 1" "Fehler: Fehlendes oder ungültiges Pflichtargument -g <GAME-ID>." 1

# Scenario 7: Valid configuration with default values (without -p)
run_test "Gültige Konfiguration mit Standardwerten" "$CONFIG_DIR/valid.conf" "-g 1234567890123" "Spielerzahl: 1" 0

# Scenario 8: Using the default configuration file without specifying -c
run_test "Verwendung der Standard-Konfigurationsdatei" "" "-g 9876543210987 -p 2" "Hostname: sysprak.priv.lab.nm.ifi.lmu.de" 0

# Summary
printf "===== Testzusammenfassung =====\n"
printf "${GREEN}Bestanden: %d${NC}\n" "$PASS_COUNT"
printf "${RED}Fehlgeschlagen: %d${NC}\n" "$FAIL_COUNT"

# Return overall status
if [ "$FAIL_COUNT" -ne 0 ]; then
    exit 1
else
    exit 0
fi
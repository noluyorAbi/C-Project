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
    local config_file="$2"         # Path to the configuration file, "NONE" to not specify
    local args="$3"                # Additional arguments
    local expected_output="$4"     # Expected output
    local expected_exit_code="$5"  # Expected exit code

    printf "===== Test: %s =====\n" "$test_name"

    # Run the program
    if [ "$config_file" == "NONE" ]; then
        # Do not specify a configuration file
        set -- $args
        "$PROGRAM" "$@" > "$LOG_DIR/$test_name.out" 2> "$LOG_DIR/$test_name.err"
    elif [ "$config_file" == "EMPTY" ]; then
        # Specify an empty configuration file
        "$PROGRAM" $args > "$LOG_DIR/$test_name.out" 2> "$LOG_DIR/$test_name.err"
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
        # Corrected printf call with -- before the expected_output
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
run_test "Fehlende Konfigurationsdatei" "/nonexistent/client.conf" "-g 1234567890123" "Fehler beim Öffnen der Konfigurationsdatei" 1

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
run_test "Verwendung der Standard-Konfigurationsdatei" "NONE" "-g 9876543210987 -p 2" "Hostname: sysprak.priv.lab.nm.ifi.lmu.de" 0

# ---------------------------------------------------
# Additional scenarios for extended test coverage
# ---------------------------------------------------

# Scenario 9: Malformed config file (missing '=')
cat > "$CONFIG_DIR/malformed_no_equals.conf" <<EOL
Hostname sysprak.priv.lab.nm.ifi.lmu.de
PortNumber 1357
GameKindName = NMMorris
EOL

run_test "Malformed Konfigurationsdatei ohne '=' " "$CONFIG_DIR/malformed_no_equals.conf" "-g 1234567890123" "Ungültiges Format in Zeile" 1

# Scenario 10: Malformed config file (invalid key-value pair)
cat > "$CONFIG_DIR/malformed_invalid_format.conf" <<EOL
Hostname= sysprak.priv.lab.nm.ifi.lmu.de
PortNumber=1357=extra
GameKindName = NMMorris
EOL

run_test "Malformed Konfigurationsdatei mit ungültigem Format" "$CONFIG_DIR/malformed_invalid_format.conf" "-g 1234567890123" "Ungültiges Format in Zeile" 1

# Scenario 11: Unknown configuration parameter
cat > "$CONFIG_DIR/unknown_param.conf" <<EOL
Hostname = sysprak.priv.lab.nm.ifi.lmu.de
PortNumber = 1357
GameKindName = NMMorris
UnknownParam = Wert
EOL

run_test "Unbekannter Parameter in Konfigurationsdatei" "$CONFIG_DIR/unknown_param.conf" "-g 1234567890123" "Unbekannter Konfigurationsparameter" 1

# Scenario 12: Unknown command line argument
run_test "Unbekanntes Kommandozeilenargument" "$CONFIG_DIR/valid.conf" "-g 1234567890123 -x" "Unbekannter Parameter" 1

# Scenario 13: GAME-ID with exactly 13 characters but includes letters
run_test "GAME-ID mit 13 Zeichen inklusive Buchstaben" "$CONFIG_DIR/valid.conf" "-g ABCDEFGHIJKLM" "Fehler: GAME-ID muss genau 13 Stellen lang sein." 1

# Scenario 14: Spielerzahl 0
run_test "Spielerzahl 0" "$CONFIG_DIR/valid.conf" "-g 1234567890123 -p 0" "Fehler: Spielerzahl muss entweder 1 oder 2 sein." 1

# Scenario 15: Spielerzahl 3
run_test "Spielerzahl 3" "$CONFIG_DIR/valid.conf" "-g 1234567890123 -p 3" "Fehler: Spielerzahl muss entweder 1 oder 2 sein." 1

# Scenario 16: Missing required parameter GameKindName
cat > "$CONFIG_DIR/missing_required_param.conf" <<EOL
Hostname = sysprak.priv.lab.nm.ifi.lmu.de
PortNumber = 1357
EOL

run_test "Konfigurationsdatei ohne erforderlichen Parameter GameKindName" "$CONFIG_DIR/missing_required_param.conf" "-g 1234567890123" "GameKindName ist in der Konfigurationsdatei nicht gesetzt." 1

# Scenario 17: Maximum length parameters
LONG_HOSTNAME=$(printf 'a%.0s' {1..255})  # 255 'a's
LONG_GAMEKINDNAME=$(printf 'b%.0s' {1..127})  # 127 'b's

cat > "$CONFIG_DIR/long_params.conf" <<EOL
Hostname = ${LONG_HOSTNAME}
PortNumber = 1357
GameKindName = ${LONG_GAMEKINDNAME}
EOL

run_test "Konfigurationsdatei mit maximal langen Hostname und GameKindName" "$CONFIG_DIR/long_params.conf" "-g 1234567890123" "Hostname: $LONG_HOSTNAME" 0

# Scenario 18: Configuration file with empty lines and comments
cat > "$CONFIG_DIR/commented_conf.conf" <<EOL
# This is a comment
Hostname = sysprak.priv.lab.nm.ifi.lmu.de

# Another comment
PortNumber = 1357
GameKindName = NMMorris
EOL

run_test "Konfigurationsdatei mit Kommentaren und leeren Zeilen" "$CONFIG_DIR/commented_conf.conf" "-g 1234567890123" "Hostname: sysprak.priv.lab.nm.ifi.lmu.de" 0

# Scenario 19: Configuration file with extra whitespace
cat > "$CONFIG_DIR/whitespace_conf.conf" <<EOL
Hostname    =    sysprak.priv.lab.nm.ifi.lmu.de
PortNumber=1357
GameKindName    =NMMorris
EOL

run_test "Konfigurationsdatei mit zusätzlichem Whitespace" "$CONFIG_DIR/whitespace_conf.conf" "-g 1234567890123" "Hostname: sysprak.priv.lab.nm.ifi.lmu.de" 0

# Scenario 20: Configuration file with uppercase keys
cat > "$CONFIG_DIR/uppercase_keys.conf" <<EOL
HOSTNAME = sysprak.priv.lab.nm.ifi.lmu.de
PORTNUMBER = 1357
GAMEKINDNAME = NMMorris
EOL

run_test "Konfigurationsdatei mit Großbuchstaben in Schlüsseln" "$CONFIG_DIR/uppercase_keys.conf" "-g 1234567890123" "Hostname: sysprak.priv.lab.nm.ifi.lmu.de" 0

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

#!/bin/bash

# test_shared_memory.sh
# Script to test the Shared Memory functionality.

# Define color variables for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Set paths
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEST_PROGRAM="$SCRIPT_DIR/../../build/test_sharedMemory" # Adjusted path to the test program
MODULE_DIR="$SCRIPT_DIR/../../modules/shared_memory" # Path to the module files
LOG_DIR="$SCRIPT_DIR/logs"

# Ensure the log directory exists
mkdir -p "$LOG_DIR"

# Ensure the build directory exists
mkdir -p "$(dirname "$TEST_PROGRAM")"

# Include header file for shared_memory if not already present
if [ ! -f "$MODULE_DIR/shared_memory.h" ]; then
  echo -e "${RED}Fehler: Header-Datei shared_memory.h fehlt.${NC}"
  exit 1
fi

# Compilation command with explicit inclusion of the header file
COMPILE_CMD="gcc -o \"$TEST_PROGRAM\" \"$SCRIPT_DIR/test_sharedMemory.c\" \"$MODULE_DIR/shared_memory.c\" -Wall -I\"$MODULE_DIR\""

# Compile the test program if it does not exist or if the source files are newer
if [ ! -x "$TEST_PROGRAM" ] || [ "$SCRIPT_DIR/test_sharedMemory.c" -nt "$TEST_PROGRAM" ] || [ "$MODULE_DIR/shared_memory.c" -nt "$TEST_PROGRAM" ]; then
  echo "Kompiliere Testprogramm..."
  eval "$COMPILE_CMD"
  if [ $? -ne 0 ]; then
    echo -e "${RED}Fehler beim Kompilieren des Testprogramms.${NC}"
    exit 1
  fi
fi

# Function to clean up remaining Shared Memory segments
cleanup_after_test() {
    echo "Bereinige verbleibende Shared-Memory-Segmente..."
    # Check if Shared Memory segments exist and remove them
    ipcs -m | grep "$USER" | awk '{print $2}' | while read -r shmid; do
        ipcrm -m "$shmid"
        printf "Entferne Shared Memory Segment: %s\n" "$shmid"
    done
}

# Perform cleanup before tests to ensure no old segments are present
cleanup_after_test

# Set up a trap to perform cleanup if the script is interrupted (e.g., via Ctrl+C)
trap cleanup_after_test EXIT

# Run the program and capture logs
echo "===== Führe Shared Memory Tests aus ====="
"$TEST_PROGRAM" > "$LOG_DIR/test_sharedMemory.out" 2> "$LOG_DIR/test_sharedMemory.err"
EXIT_CODE=$?

# Initialize counters for Pass and Fail
PASS_COUNT=0
FAIL_COUNT=0

# Extract Pass and Fail counts from the log
PASS_COUNT=$(grep -c "\[PASS\]" "$LOG_DIR/test_sharedMemory.out")
FAIL_COUNT=$(grep -c "\[FAIL\]" "$LOG_DIR/test_sharedMemory.out")

# Display summary
echo -e "\n===== Testzusammenfassung ====="
echo -e "${GREEN}Bestanden: $PASS_COUNT${NC}"
echo -e "${RED}Fehlgeschlagen: $FAIL_COUNT${NC}"

# Display detailed error messages if any
if [ "$FAIL_COUNT" -ne 0 ]; then
  echo -e "\n${RED}Einige Tests sind fehlgeschlagen. Details siehe unten:${NC}"
  echo "---- Inhalt von test_sharedMemory.out ----"
  grep "\[FAIL\]" "$LOG_DIR/test_sharedMemory.out"
  echo "---- Inhalt von test_sharedMemory.err ----"
  cat "$LOG_DIR/test_sharedMemory.err"
fi

# Optional: Display the entire log
echo -e "\n---- Gesamter Test-Log ----"
 cat "$LOG_DIR/test_sharedMemory.out"
 cat "$LOG_DIR/test_sharedMemory.err"

# Optional: Clean up unnecessary log files
# rm -f "$LOG_DIR/test_sharedMemory.out" "$LOG_DIR/test_sharedMemory.err"

# Return the overall status
if [ "$FAIL_COUNT" -ne 0 ]; then
    exit 1
else
    exit 0
fi

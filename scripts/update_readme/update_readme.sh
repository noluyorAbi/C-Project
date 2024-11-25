#!/bin/bash

# Beendet das Skript sofort, wenn ein Befehl mit einem Fehlerstatus endet
set -e

# Farben definieren, falls das Skript an ein Terminal ausgegeben wird
if [ -t 1 ]; then
    COLOR_INFO="\033[34m[INFO]\033[0m"
    COLOR_ERROR="\033[31m[ERROR]\033[0m"
    COLOR_UPDATE="\033[32m[UPDATE]\033[0m"
    COLOR_NONE="\033[0m"
else
    COLOR_INFO=""
    COLOR_ERROR=""
    COLOR_UPDATE=""
    COLOR_NONE=""
fi

# Standardmäßig ist der stille Modus deaktiviert
QUIET=false

# Funktion zur Anzeige von Informationsmeldungen
echo_info() {
    if [ "$QUIET" = false ]; then
        printf "${COLOR_INFO} %s ${COLOR_NONE}\n" "$1"
    fi
}

# Funktion zur Anzeige von Fehlermeldungen und Skript beenden
echo_error() {
    printf "${COLOR_ERROR} %s ${COLOR_NONE}\n" "$1" >&2
    exit 1
}

# Funktion zur Anzeige von Hilfe
show_help() {
    cat << EOF
Usage: $(basename "$0") [OPTIONS]

Options:
  -q, --quiet    Suppress informational messages
  -h, --help     Display this help message
EOF
}

# Überprüfen der Argumente
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -q|--quiet) QUIET=true ;;
        -h|--help) show_help; exit 0 ;;
        *) echo_error "Unbekannte Option: $1" ;;
    esac
    shift
done

# Bestimme das Verzeichnis des Skripts
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Navigiere zum Projektstammverzeichnis (angenommen, scripts/update_readme/ ist direkt unter dem Projektstamm)
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"
cd "$PROJECT_ROOT"
echo_info "Navigated to project root: $PROJECT_ROOT"

# Überprüfe, ob 'tree' installiert ist, und installiere es bei Bedarf
install_tree() {
    echo_info "'tree' not found. Attempting to install..."
    if command -v apt-get &> /dev/null; then
        sudo apt-get update
        sudo apt-get install -y tree
    elif command -v yum &> /dev/null; then
        sudo yum install -y tree
    elif command -v dnf &> /dev/null; then
        sudo dnf install -y tree
    elif command -v pacman &> /dev/null; then
        sudo pacman -Syu --noconfirm tree
    elif command -v brew &> /dev/null; then
        brew install tree
    else
        echo_error "No supported package manager found. Please install 'tree' manually."
    fi
}

if ! command -v tree &> /dev/null; then
    install_tree
else
    echo_info "'tree' is already installed."
fi

# Definiere Ausschlüsse für das 'tree'-Kommando
EXCLUDES=".git|node_modules|build|.DS_Store|*.patch|*.d|.idea|.vscode|*.o|*.ko|*.obj|*.elf|*.ilk|*.map|*.exp|*.gch|*.pch|*.lib|*.a|*.la|*.lo|*.dll|*.so|*.so.*|*.dylib|*.exe|*.out|*.app|*.i*86|*.x86_64|*.hex|*.dSYM|*.su|*.idb|*.pdb|*.mod*|*.cmd|.tmp_versions|modules.order|Module.symvers|Mkfile.old|dkms.conf"

# Generiere die Projektstruktur und speichere sie in einer temporären Datei
echo_info "Generating project structure..."
TEMP_TREE_FILE="$(mktemp)"
trap 'rm -f "$TEMP_TREE_FILE"' EXIT
tree -I "$EXCLUDES" > "$TEMP_TREE_FILE"

# Definiere Marker in README.md
START_MARKER="<!-- project-structure-start -->"
END_MARKER="<!-- project-structure-end -->"

# Überprüfe, ob README.md existiert
README_FILE="README.md"
if [ ! -f "$README_FILE" ]; then
    echo_error "README.md not found in project root."
fi

# Überprüfe, ob beide Marker vorhanden sind
if ! grep -F -q "$START_MARKER" "$README_FILE" || ! grep -F -q "$END_MARKER" "$README_FILE"; then
    echo_error "Markers '$START_MARKER' and/or '$END_MARKER' not found in $README_FILE."
fi

# Aktualisiere README.md mit der neuesten Projektstruktur
echo_info "Updating README.md with the latest project structure..."

awk -v start="$START_MARKER" -v end="$END_MARKER" -v tree_file="$TEMP_TREE_FILE" '
    BEGIN { inside = 0 }
    $0 ~ start {
        print $0
        print "```sh"
        while ((getline line < tree_file) > 0) {
            print line
        }
        print "```"
        inside = 1
        next
    }
    $0 ~ end {
        print $0
        inside = 0
        next
    }
    {
        if (!inside) {
            print $0
        }
    }
' "$README_FILE" > "$README_FILE.tmp"

# Ersetze die originale README.md mit der aktualisierten Version
mv "$README_FILE.tmp" "$README_FILE"

echo_info "README.md has been successfully updated."

# Optional: Zeige den aktualisierten Abschnitt an
if [ "$QUIET" = false ]; then
    printf "${COLOR_UPDATE}Displaying the updated project structure section in README.md:${COLOR_NONE}\n"
    awk "/$START_MARKER/,/$END_MARKER/" "$README_FILE"
fi

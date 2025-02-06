# Compiler und Flags
CC = gcc
CFLAGS = -Wall -Werror -Imodules -Imodules/args_parser -Imodules/tcp_performConnection -fsanitize=address -fno-omit-frame-pointer

# Verzeichnisse
BIN_DIR = bin
BUILD_DIR = build
SRC_DIR = .
MODULES_DIR = modules
LIB_DIR = lib
TESTS_DIR = tests

# Ziele
TARGET = $(BIN_DIR)/sysprak-client
LIBRARY = $(LIB_DIR)/libsysprak.a

# Mock-Daten
MOCK_ARGS = -g 0hnmtqyocqhej

# Quellen und Objektdateien
LIB_SRC = $(shell find $(MODULES_DIR) -name '*.c')
LIB_OBJ = $(LIB_SRC:%.c=$(BUILD_DIR)/%.o)

MAIN_SRC = main.c
MAIN_OBJ = $(BUILD_DIR)/main.o

TEST_SRC = $(shell find $(TESTS_DIR) -type f -name '*.c')
TEST_OBJ = $(TEST_SRC:%.c=$(BUILD_DIR)/%.o)

CLANG_FORMAT = clang-format

# Standardziel
.DEFAULT_GOAL := all

# Phony-Ziele
.PHONY: all run clean format tidy test update_readme

all: $(TARGET) update_readme

# Kompilieren von Bibliotheksobjektdateien
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Kompilieren von main.o mit korrektem Header-Pfad
$(BUILD_DIR)/main.o: main.c modules/tcp_performConnection/performConnection.h 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Erstellen der statischen Bibliothek
$(LIBRARY): $(LIB_OBJ)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

# Linken des Hauptprogramms mit der Bibliothek
$(TARGET): $(MAIN_OBJ) $(LIBRARY)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Formatieren der Quellcode-Dateien
format:
	@echo "\n\033[1;35mFormatiere Quellcode-Dateien...\033[0m\n"
	@formatted_count=0; \
	for file in $(shell find $(SRC_DIR) $(MODULES_DIR) -name '*.c' -o -name '*.h'); do \
		if ! $(CLANG_FORMAT) --dry-run --Werror $$file 2>/dev/null; then \
			$(CLANG_FORMAT) -i $$file; \
			echo "\033[1;33m  Formatiert:\033[0m $$file"; \
			formatted_count=$$((formatted_count + 1)); \
		else \
			echo "\033[1;32m  Bereits formatiert:\033[0m $$file"; \
		fi; \
	done; \
	echo; \
	echo "\033[1;36mFormatierung abgeschlossen. Gesamtzahl der formatierten Dateien: $$formatted_count.\033[0m"

# Clean-Ziel - entfernt Binärdateien und Build-Dateien
clean:
	@echo "Bereinige..."
	@for dir in $(BIN_DIR) $(BUILD_DIR) $(LIB_DIR); do \
		if [ -d "$$dir" ]; then \
			echo "Bereinige $$dir..."; \
			find $$dir -type f ! -name "README.md" -exec rm -f {} +; \
			find $$dir -type d -empty -delete; \
		fi; \
	done


# Clang-Tidy-Ziel - Analysiert alle Quellcode-Dateien mit clang-tidy
tidy:
	@echo "Führe clang-tidy auf Quellcode-Dateien aus..."
	@for file in $(LIB_SRC) $(MAIN_SRC); do \
		echo "Analysiere $$file..."; \
		clang-tidy -p . $$file || { echo "clang-tidy schlug bei $$file fehl"; exit 1; }; \
	done
	@echo "clang-tidy-Analyse erfolgreich abgeschlossen."

# Ausführen des Hauptprogramms mit Mock-Daten
run: $(TARGET)
	echo "\n"
	./$(TARGET) $(MOCK_ARGS)

# Kompilieren und Ausführen von Tests
test: $(LIBRARY) $(TEST_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/sysprak-client-test $(TEST_OBJ) -L$(LIB_DIR) -lsysprak -lpthread
	@echo "\n\n"
	@./$(BIN_DIR)/sysprak-client-test

# Phony Target für Update des README.md
update_readme:
#	@printf "\n\033[1;35mAktualisiere die Projektstruktur in README.md...\033[0m\n"
	@./scripts/update_readme/update_readme.sh > /dev/null
#	@printf "\033[1;36mUpdate abgeschlossen. Die Projektstruktur wurde in die README.md übernommen.\033[0m\n"

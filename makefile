# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Werror -Imodules -Imodules/args_parser -Imodules/tcp_performConnection

# Directories
BUILD_DIR   = build
MODULES_DIR = modules
LIB_DIR     = lib
TESTS_DIR   = tests

# Targets
TARGET    = sysprak-client
LIBRARY   = $(LIB_DIR)/libsysprak.a

# Mock Data
MOCK_ARGS = -g 0hnmtqyocqhej

# Sources and Object Files
LIB_SRC = $(shell find $(MODULES_DIR) -name '*.c')
LIB_OBJ = $(LIB_SRC:%.c=$(BUILD_DIR)/%.o)

MAIN_SRC = main.c
MAIN_OBJ = $(BUILD_DIR)/main.o

TEST_SRC = $(shell find $(TESTS_DIR) -type f -name '*.c')
TEST_OBJ = $(TEST_SRC:%.c=$(BUILD_DIR)/%.o)

CLANG_FORMAT = clang-format

# Default Target
.DEFAULT_GOAL := all

# Phony Targets
.PHONY: all run clean format tidy test update_readme

all: $(TARGET) update_readme

# Build object files for the library
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Build main.o (explicit dependency on a header)
$(BUILD_DIR)/main.o: main.c modules/tcp_performConnection/performConnection.h 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Build the static library
$(LIBRARY): $(LIB_OBJ)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

# Link the main program with the library, placing the executable in the root
$(TARGET): $(MAIN_OBJ) $(LIBRARY)
	$(CC) $(CFLAGS) -o $@ $(MAIN_OBJ) $(LIBRARY)

# Format source code files
format:
	@echo "\n\033[1;35mFormatiere Quellcode-Dateien...\033[0m\n"
	@formatted_count=0; \
	for file in $(shell find . $(MODULES_DIR) -name '*.c' -o -name '*.h'); do \
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

# Clean: remove build, lib and binaries
clean:
	@echo "Bereinige..."
	@for dir in $(BUILD_DIR) $(LIB_DIR); do \
		if [ -d "$$dir" ]; then \
			echo "Bereinige $$dir..."; \
			find $$dir -type f ! -name "README.md" -exec rm -f {} +; \
			find $$dir -type d -empty -delete; \
		fi; \
	done
	@rm -f $(TARGET) sysprak-client-test

# Run clang-tidy on source files
tidy:
	@echo "Führe clang-tidy auf Quellcode-Dateien aus..."
	@for file in $(LIB_SRC) $(MAIN_SRC); do \
		echo "Analysiere $$file..."; \
		clang-tidy -p . $$file || { echo "clang-tidy schlug bei $$file fehl"; exit 1; }; \
	done
	@echo "clang-tidy-Analyse erfolgreich abgeschlossen."

# Run the main program with mock arguments
run: $(TARGET)
	@echo "\n"
	@./$(TARGET) $(MOCK_ARGS)

# Build and run tests
test: $(LIBRARY) $(TEST_OBJ)
	$(CC) $(CFLAGS) -o sysprak-client-test $(TEST_OBJ) $(LIBRARY) -lpthread
	@echo "\n\n"
	@./sysprak-client-test

# Update README.md with the project structure
update_readme:
	@./scripts/update_readme/update_readme.sh > /dev/null

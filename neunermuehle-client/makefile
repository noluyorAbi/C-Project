# Compiler and Flags
CC = clang
CFLAGS = -Wall -Werror -Imodules -Imodules/args_parser

# Directories
BIN_DIR = bin
BUILD_DIR = build
SRC_DIR = .
MODULES_DIR = modules/args_parser

# Target name
TARGET = $(BIN_DIR)/sysprak-client

# Mock data
MOCK_ARGS = -g 1234567890123 -p 1

# Source and object files
SRC = $(shell find $(SRC_DIR) $(MODULES_DIR) -name '*.c' -o -name '*.h')
OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)
CLANG_FORMAT = clang-format

# Default target
.DEFAULT_GOAL := all

# Phony targets
.PHONY: all run clean format tidy

# Format the source files
format:
	@echo "\n\033[1;35mReformating source files...\033[0m\n"
	@formatted_count=0; \
	for file in $(shell find $(SRC_DIR) $(MODULES_DIR) -name '*.c' -o -name '*.h'); do \
		if ! $(CLANG_FORMAT) --dry-run --Werror $$file 2>/dev/null; then \
			$(CLANG_FORMAT) -i $$file; \
			echo "\033[1;33m  Reformatted:\033[0m $$file"; \
			formatted_count=$$((formatted_count + 1)); \
		else \
			echo "\033[1;32m  Already formatted:\033[0m $$file"; \
		fi; \
	done; \
	echo; \
	echo "\033[1;36mFormatting complete. Total files reformatted: $$formatted_count.\033[0m"

# Compile the main program
all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^
	@clang-format -i $(SRC)

# Compile object files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	@clang-format -i $(SRC)

# Run the program with mock data
run: $(TARGET)
	./$(TARGET) $(MOCK_ARGS)
	@clang-format -i $(SRC)

# Clean target - removes binaries and build files
clean:
	@echo "Cleaning up..."
	@rm -rf $(BIN_DIR) $(BUILD_DIR)
	@clang-format -i $(SRC)

# Clang-Tidy target - Analyze all source files with clang-tidy
tidy:
	@echo "Running clang-tidy on source files..."
	@for file in $(SRC); do \
		echo "Analyzing $$file..."; \
		clang-tidy -p . $$file || { echo "clang-tidy failed on $$file"; exit 1; }; \
	done
	@echo "clang-tidy analysis completed successfully."

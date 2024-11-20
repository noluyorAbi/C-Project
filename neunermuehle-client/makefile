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
SRC = $(SRC_DIR)/main.c $(MODULES_DIR)/args_parser.c
OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)

# Default target
.DEFAULT_GOAL := all

# Phony targets
.PHONY: all run clean format tidy

# Format the source files
format:
	@echo "Formatting source files..."
	@clang-format -i $(SRC)

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

# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Werror -g -Imodules -Imodules/tcp_performConnection -Imodules/thinker_and_connector -Imodules/args_parser

# Directories
BUILD_DIR = build
SRC_DIR = .
MODULES_DIR = modules
LIB_DIR = lib
BIN_DIR = bin

# Source Files
MAIN_SRC = main.c
CONNECTOR_SRC = modules/thinker_and_connector/connector.c
PERFORM_CONN_SRC = modules/tcp_performConnection/performConnection.c
ARGS_PARSER_SRC = modules/args_parser/args_parser.c
CONFIG_SRC = modules/args_parser/config.c

# Object Files
MAIN_OBJ = $(BUILD_DIR)/main.o
CONNECTOR_OBJ = $(BUILD_DIR)/modules/thinker_and_connector/connector.o
PERFORM_CONN_OBJ = $(BUILD_DIR)/modules/tcp_performConnection/performConnection.o
ARGS_PARSER_OBJ = $(BUILD_DIR)/modules/args_parser/args_parser.o
CONFIG_OBJ = $(BUILD_DIR)/modules/args_parser/config.o

# Targets
TARGET = $(BIN_DIR)/sysprak-client

all: $(TARGET)

$(TARGET): $(MAIN_OBJ) $(CONNECTOR_OBJ) $(PERFORM_CONN_OBJ) $(ARGS_PARSER_OBJ) $(CONFIG_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
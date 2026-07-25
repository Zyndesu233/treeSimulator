CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -I./src
LDFLAGS ?=

APP := tree-simulator
BUILD_DIR := build
BIN_DIR := bin

APP_SOURCES := \
	src/common/log.c \
	src/core/forest.c \
	src/output/tree_printer.c \
	src/repl/commands.c \
	src/repl/input.c \
	src/repl/repl.c \
	src/trees/avl_tree.c \
	src/trees/btree.c \
	src/trees/binary_tree.c \
	src/trees/binary_search_tree.c \
	src/trees/red_black_tree.c \
	src/trees/splay_tree.c

MAIN_SOURCE := src/main.c
TEST_SOURCE := tests/test_main.c

APP_OBJECTS := $(APP_SOURCES:src/%.c=$(BUILD_DIR)/src/%.o)
MAIN_OBJECT := $(BUILD_DIR)/src/main.o
TEST_OBJECT := $(BUILD_DIR)/tests/test_main.o

.PHONY: all clean test run

all: $(BIN_DIR)/$(APP)

$(BIN_DIR)/$(APP): $(APP_OBJECTS) $(MAIN_OBJECT)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/tests_runner: $(APP_OBJECTS) $(TEST_OBJECT)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/src/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/tests/%.o: tests/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(BUILD_DIR)/tests_runner $(BIN_DIR)/$(APP)
	./$(BUILD_DIR)/tests_runner
	sh ./scripts/run_simulation_test.sh examples/basic_bst.commands examples/basic_bst.expected

run: $(BIN_DIR)/$(APP)
	./$(BIN_DIR)/$(APP)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

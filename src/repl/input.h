#ifndef TREE_SIMULATOR_INPUT_H
#define TREE_SIMULATOR_INPUT_H

#include <stdbool.h>
#include <stddef.h>

#include "trees/binary_tree.h"

#define INPUT_MAX_TOKENS 128

typedef enum repl_command {
    REPL_COMMAND_HELP,
    REPL_COMMAND_NEW,
    REPL_COMMAND_INSERT,
    REPL_COMMAND_INSERT_MANY,
    REPL_COMMAND_DELETE,
    REPL_COMMAND_FIND,
    REPL_COMMAND_PRINT,
    REPL_COMMAND_DUMP,
    REPL_COMMAND_QUIT,
    REPL_COMMAND_EMPTY,
    REPL_COMMAND_UNKNOWN
} repl_command;

size_t input_tokenize(char *line, char *tokens[], size_t max_tokens);
bool input_parse_int(const char *text, int *value);
bool input_parse_size(const char *text, size_t *value);
tree_type input_parse_tree_type(const char *text);
repl_command input_parse_command(const char *text);
const char *input_command_name(repl_command command);

#endif

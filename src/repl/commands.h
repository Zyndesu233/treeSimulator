#ifndef TREE_SIMULATOR_COMMANDS_H
#define TREE_SIMULATOR_COMMANDS_H

#include <stdio.h>

#include "core/forest.h"

typedef enum command_result {
    COMMAND_RESULT_CONTINUE,
    COMMAND_RESULT_QUIT
} command_result;

typedef struct command_context {
    forest *trees;
    FILE *out;
} command_context;

command_result command_execute_line(command_context *context, char *line);
void command_print_help(FILE *out);

#endif

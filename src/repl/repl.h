#ifndef TREE_SIMULATOR_REPL_H
#define TREE_SIMULATOR_REPL_H

#include <stdbool.h>
#include <stdio.h>

#include "core/forest.h"

int repl_run(forest *trees, FILE *input, FILE *out, bool interactive);
int simulate_file(forest *trees, const char *path, FILE *out);
void repl_print_usage(FILE *out, const char *program_name);

#endif

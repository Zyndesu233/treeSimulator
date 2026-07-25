#include "repl/repl.h"

#include <stdio.h>

#include "common/log.h"
#include "common/version.h"
#include "repl/commands.h"

#define REPL_LINE_SIZE 512

static void print_banner(FILE *out) {
    fprintf(out, "        /\\\n");
    fprintf(out, "       /  \\\n");
    fprintf(out, "      /____\\\n");
    fprintf(out, "        ||\n");
    fprintf(out, "  %s\n", TREE_SIMULATOR_NAME);
    fprintf(out, "  Version: %s\n", TREE_SIMULATOR_VERSION);
    fprintf(out, "  Trees: bst, avl, splay, rbt, btree (in progress)\n");
    fprintf(out, "  Type help for commands.\n\n");
}

int repl_run(forest *trees, FILE *input, FILE *out, bool interactive) {
    char line[REPL_LINE_SIZE];
    command_context context = {
        .trees = trees,
        .out = out,
    };

    if (interactive) {
        print_banner(out);
    }

    while (true) {
        if (interactive) {
            fprintf(out, "> ");
            fflush(out);
        }

        if (fgets(line, sizeof(line), input) == NULL) {
            break;
        }

        if (command_execute_line(&context, line) == COMMAND_RESULT_QUIT) {
            break;
        }
    }

    return 0;
}

int simulate_file(forest *trees, const char *path, FILE *out) {
    FILE *input = fopen(path, "r");
    if (input == NULL) {
        log_error(out, "Could not open input file: %s", path);
        return 1;
    }

    int status = repl_run(trees, input, out, false);
    fclose(input);
    return status;
}

void repl_print_usage(FILE *out, const char *program_name) {
    fprintf(out, "Usage:\n");
    fprintf(out, "  %s                 start REPL mode\n", program_name);
    fprintf(out, "  %s repl|-r          start REPL mode\n", program_name);
    fprintf(out, "  %s simulate|-s FILE run commands from FILE\n", program_name);
}

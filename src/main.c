#include <stdio.h>
#include <string.h>

#include "core/forest.h"
#include "repl/repl.h"

int main(int argc, char **argv) {
    forest trees;
    forest_init(&trees);

    int status = 0;
    if (argc == 1 || strcmp(argv[1], "repl") == 0 || strcmp(argv[1], "-r") == 0) {
        status = repl_run(&trees, stdin, stdout, true);
    } else if ((strcmp(argv[1], "simulate") == 0 || strcmp(argv[1], "-s") == 0) && argc == 3) {
        status = simulate_file(&trees, argv[2], stdout);
    } else {
        repl_print_usage(stderr, argv[0]);
        status = 1;
    }

    forest_free(&trees);
    return status;
}

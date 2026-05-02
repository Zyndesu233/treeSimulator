#include <stdio.h>
#include <string.h>
#include "Interpreter.h"
#include "Simulator.h"
#include "Util.h"

void printUsage() {
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        printError("No subcommand is provided\n");
        printUsage();
        return 1;
    }

    Forest forest = {0};

    if (strcmp(argv[1], "interpret") == 0) {
        interpret(&forest);
        return 0;
    }

    if (strcmp(argv[1], "simulate") == 0) {
        simulate(&forest, argv[2]);
        return 0;
    }

    printWarning("Unknown subcommand\n");
    printUsage();
    return 1;
}
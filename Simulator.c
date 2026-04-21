#include "Simulator.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Commands.h"
#include "Util.h"

int main() {
    printf(
        "  /\\   _______              _____ _                 _       _             \n"
        " /**\\ |__   __|            / ____(_)               | |     | |            \n"
        "/****\\   | |_ __ ___  ___ | (___  _ _ __ ___  _   _| | __ _| |_ ___  _ __ \n"
        "  ||     | | '__/ _ \\/ _ \\ \\___ \\| | '_ ` _ \\| | | | |/ _` | __/ _ \\| '__|\n"
        "  ||     | | | |  __/  __/ ____) | | | | | | | |_| | | (_| | || (_) | |   \n"
        "  ||     |_|_|  \\___|\\___||_____/|_|_| |_| |_|\\__,_|_|\\__,_|\\__\\___/|_|   \n"
    );
    printf("\n");
    printf("Author: K1tsune233\n");
    printf("Version: 5.0.0\n");
    printf("\n");

    Forest forest = {0};

    char line[100];
    while (true) {
        printf("> ");
        fgets(line, 100, stdin);
        if (strcmp(line, "\n") == 0) continue;
        char* command = strtok(line, " \n");

        assert(COMMAND_NUMBER == 11);
        switch (string2command(command)) {
            case QUIT:
                printf("[INFO] Exit.\n");
                exit(EXIT_SUCCESS);
                break;
            case HELP:
                runHelp();
                break;
            case INSERT:
                runInsert(&forest);
                break;
            case INSERT_MANY:
                runInsertMany(&forest);
                break;
            case PRINT:
                runPrint(&forest);
                break;
            case DELETE:
                runDelete(&forest);
                break;
            case TRAVERSAL:
                runTraversal(&forest);
                break;
            case NEW:
                runNew(&forest);
                break;
            case DUMP_FOREST:
                runDumpForest(&forest);
                break;
            case SEARCH:
                runSearch(&forest);
                break;
            case LOAD_TREE:
                runLoadTree(&forest);
                break;
            default:
                printError("Unknown command.\n");
                printf("Use [h]elp command to display command list.\n");
                break;
        }
        printf("\n");
    }
}
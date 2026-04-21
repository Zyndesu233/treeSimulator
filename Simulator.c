#include "Simulator.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Commands.h"
#include "Util.h"

int main(int argc, char** argv) {
    if (argc <= 1) {
        printError("No subcommand is provided\n");
    }
    Forest forest = {0};
    char line[100];

    if (strcmp(argv[1], "simulate") == 0) {
        printWelcomeMsg("5.1.0");

        while (true) {
            printf("> ");
            fgets(line, 100, stdin);
            if (strcmp(line, "\n") == 0) continue;
            char* command = strtok(line, " \n");

            assert(COMMAND_NUMBER == 11);
            switch (string2command(command)) {
                case QUIT: {
                    printInfo("Exit.\n");
                    exit(EXIT_SUCCESS);
                    break;
                }
                case HELP: {
                    char* manualPage = strtok(NULL, " \n");
                    runHelp(manualPage);
                    break;
                }
                case INSERT: {
                    size_t index;
                    int val;
                    if (!readAndParseSubcommandToSize(&index, INSERT) || !readAndParseSubcommandToInt(&val, INSERT)) {
                        break;
                    }
                    if (index >= forest.count) {
                        printError("Tree with index %d is not found.\n", index);
                        break;
                    }
                    runInsert(&forest, index, val);
                    break;
                }
                case INSERT_MANY: {
                    size_t index;
                    int count, vals[MAX_INT_INPUT_CNT];
                    if (!readAndParseSubcommandToSize(&index, INSERT) ||
                        !readAndParseSubcommandToInt(&count, INSERT) ||
                        !readAndParseSubcommandsToInts(count, vals, INSERT_MANY)) {
                        break;
                    }
                    if (index >= forest.count) {
                        printError("Tree with index %d is not found.\n", index);
                        break;
                    }
                    runInsertMany(&forest, index, count, vals);
                    break;
                }
                case PRINT: {
                    size_t index;
                    if (!readAndParseSubcommandToSize(&index, INSERT)) {
                        break;
                    }
                    if (index >= forest.count) {
                        printError("Tree with index %d is not found.\n", index);
                        break;
                    }
                    runPrint(&forest, index, stdout);
                    break;
                }
                case DELETE:
                    runDelete(&forest);
                    break;
                case TRAVERSAL:
                    runTraversal(&forest);
                    break;
                case NEW: {
                    TreeType treeType;
                    if (!readAndParseSubcommandToTreeType(&treeType, NEW))
                        break;
                    runNew(&forest, treeType);
                    break;
                }
                case DUMP_FOREST: {
                    runDumpForest(&forest);
                    break;
                }
                case SEARCH: {
                    size_t index;
                    int val;
                    if (!readAndParseSubcommandToSize(&index, SEARCH) || !readAndParseSubcommandToInt(&val, SEARCH))
                        break;
                    if (index >= forest.count) {
                        printError("Tree with index %d is not found\n", index);
                        break;
                    }
                    runSearch(&forest, index, val);
                    break;
                }
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
    } else if (strcmp(argv[1], "compile") == 0) {
        printInfo("Parsing %s\n", argv[2]);
        todo("Compile mode\n");
    } else {
        printWarning("Unknown subcommand\n");
    }
}
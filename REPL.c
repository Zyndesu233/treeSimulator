#include "REPL.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Commands.h"
#include "Util.h"

void interpret(ForestPtr forestPtr) {
    char inputBuffer[100];
    printWelcomeMsg("5.1.0");

    while (true) {
        printf("> ");
        fgets(inputBuffer, 100, stdin);
        if (strcmp(inputBuffer, "\n") == 0) continue;
        char* command = strtok(inputBuffer, " \n");

        assert(COMMAND_NUMBER == 10);
        switch (string2command(command)) {
            case QUIT: {
                printInfo("Exit.\n");
                return;
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
                if (index >= forestPtr->count) {
                    printError("Tree with index %d is not found.\n", index);
                    break;
                }
                runInsert(forestPtr, index, val);
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
                if (index >= forestPtr->count) {
                    printError("Tree with index %d is not found.\n", index);
                    break;
                }
                runInsertMany(forestPtr, index, count, vals);
                break;
            }
            case PRINT: {
                size_t index;
                if (!readAndParseSubcommandToSize(&index, INSERT)) {
                    break;
                }
                if (index >= forestPtr->count) {
                    printError("Tree with index %d is not found.\n", index);
                    break;
                }
                runPrint(forestPtr, index, stdout);
                break;
            }
            case DELETE:
                runDelete(forestPtr);
                break;
            case TRAVERSAL:
                runTraversal(forestPtr);
                break;
            case NEW: {
                TreeType treeType;
                if (!readAndParseSubcommandToTreeType(&treeType, NEW))
                    break;
                runNew(forestPtr, treeType);
                break;
            }
            case DUMP_FOREST: {
                runDumpForest(forestPtr);
                break;
            }
            case SEARCH: {
                size_t index;
                int val;
                if (!readAndParseSubcommandToSize(&index, SEARCH) || !readAndParseSubcommandToInt(&val, SEARCH))
                    break;
                if (index >= forestPtr->count) {
                    printError("Tree with index %d is not found\n", index);
                    break;
                }
                runSearch(forestPtr, index, val);
                break;
            }
            default:
                printError("Unknown command.\n");
                printf("Use [h]elp command to display command list.\n");
                break;
        }
        printf("\n");
    }
}
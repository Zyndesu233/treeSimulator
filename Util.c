#include "Util.h"
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**
 * @name string2command
 * Convert string to command
 * @param {char*} command The command in string
 * @return {Command} The corresponding command
 */

Command string2command(char* command) {
    assert(COMMAND_NUMBER == 11 && "HAVE NOT EXHAUST ALL COMMAND");
    if (strcmp(command, "quit") == 0 || strcmp(command, "q") == 0)
        return QUIT;
    else if (strcmp(command, "help") == 0 || strcmp(command, "h") == 0)
        return HELP;
    else if (strcmp(command, "insert") == 0 || strcmp(command, "i") == 0)
        return INSERT;
    else if (strcmp(command, "insert_many") == 0 || strcmp(command, "im") == 0)
        return INSERT_MANY;
    else if (strcmp(command, "print") == 0 || strcmp(command, "p") == 0)
        return PRINT;
    else if (strcmp(command, "delete") == 0 || strcmp(command, "d") == 0)
        return DELETE;
    else if (strcmp(command, "traversal") == 0 || strcmp(command, "t") == 0)
        return TRAVERSAL;
    else if (strcmp(command, "new") == 0 || strcmp(command, "n") == 0)
        return NEW;
    else if (strcmp(command, "dump_trees") == 0 || strcmp(command, "dt") == 0)
        return DUMP_TREES;
    else if (strcmp(command, "search") == 0 || strcmp(command, "s") == 0)
        return SEARCH;
    else if (strcmp(command, "load_tree") == 0 || strcmp(command, "lt") == 0)
        return LOAD_TREE;
    return UNKNOWN_COMMAND;
}

/**
 * @name command2string
 * Convert command to string
 * @param {Command} command The command
 * @return {char*} The corresponding string
 */
char* command2string(Command command) {
    assert(COMMAND_NUMBER == 11 && "HAVE NOT EXHAUST ALL COMMAND");
    switch (command) {
        case QUIT:
            return "quit";
        case HELP:
            return "help";
        case INSERT:
            return "insert";
        case INSERT_MANY:
            return "insert_many";
        case PRINT:
            return "print";
        case DELETE:
            return "delete";
        case TRAVERSAL:
            return "traversal";
        case NEW:
            return "new";
        case DUMP_TREES:
            return "dump_trees";
        case SEARCH:
            return "search";
        case LOAD_TREE:
            return "load_tree";
        default:
            return "unknown";
    }
}

Subcommand hashSubcommand(char* subcommand) {
    assert(SUBCOMMAND_NUMBER == 3);
    if (strcmp(subcommand, "in-order") == 0 || strcmp(subcommand, "in") == 0)
        return IN_ORDER;
    else if (strcmp(subcommand, "pre-order") == 0 || strcmp(subcommand, "pre") == 0)
        return PRE_ORDER;
    else if (strcmp(subcommand, "post-order") == 0 || strcmp(subcommand, "post") == 0)
        return POST_ORDER;
    return UNKNOWN_SUBCOMMAND;
}

/**
 * @name string2treetype
 * Convert string to tree type
 * @param {char*} treeType The tree type
 * @return {TreeType} The corresponding tree type
 */
TreeType string2treetype(char* treeType) {
    assert(TREE_TYPE_NUMBER == 4);
    if (strcmp(treeType, "binary_search_tree") == 0 || strcmp(treeType, "bst") == 0)
        return BST;
    else if (strcmp(treeType, "avl_tree") == 0 || strcmp(treeType, "avl") == 0)
        return AVL;
    else if (strcmp(treeType, "splay_tree") == 0 || strcmp(treeType, "spl") == 0)
        return SPL;
    else if (strcmp(treeType, "red_black_tree") == 0 || strcmp(treeType, "rbt") == 0)
        return RBT;
    return UNKNOWN_TREE_TYPE;
}

/**
 * @name treetype2string
 * Convert string to tree type
 * @param {TreeType} treeType The tree type
 * @return {char*} The corresponding string
 */
char* treetype2string(TreeType treeType) {
    assert(TREE_TYPE_NUMBER == 4);
    switch (treeType) {
        case BST:
            return "binary search tree";
        case AVL:
            return "AVL tree";
        case SPL:
            return "splay tree";
        case RBT:
            return "red black tree";
        default:
            assert(false && "[ERROR] UNREACHABLE");
            break;
    }
    return "";
}

/**
 * @name printCommandFormat
 * Print the corresponding command format to the terminal.
 * @param {Command} command The command name
 */
void printCommandFormat(Command command) {
    assert(COMMAND_NUMBER == 11 && "HAVE NOT EXHAUST ALL COMMAND");
    switch (command) {
        case QUIT:
            printf("[q]uit\n");
            break;
        case HELP:
            printf("[h]elp (<command:str>)\n");
            break;
        case INSERT:
            printf("[i]nsert <index:int> <value:int>\n");
            break;
        case INSERT_MANY:
            printf("[i]nsert_[m]any <index:int> <count:int> (<value:int> ...)\n");
            break;
        case PRINT:
            printf("[p]rint <index:int>\n");
            break;
        case DELETE:
            printf("[d]elete <index:int> <value:int>\n");
            break;
        case TRAVERSAL:
            printf("[t]raversal <index:int> <type:str>\n");
            break;
        case NEW:
            printf("[n]ew <index:int> <tree_type:str>\n");
            break;
        case DUMP_TREES:
            printf("[d]ump_[t]rees\n");
            break;
        case SEARCH:
            printf("[s]earch <index:int> <value:int>\n");
            break;
        case LOAD_TREE:
            printf("[l]oad_[t]ree <index:int> <type:str> <node_count:int> (<tree_format> ...)\n");
            break;
        default:
            assert(false && "UNREACHABLE");
            break;
    }
}

/**
 * @name todo
 * Place holder for function to be implemented.
 * Print warning message in terminal.
 * The program is NOT halt.
 * @param {char*} fn The name of the function to be implemented
 */

void todo(char* fn) {
    printf("[WARNING] %s has not be implemented.\n", fn);
    printf("Pull requests are welcomed!");
}

/**
 * @name printInfo
 * Print info message like "[INFO] <message_format>" in terminal.
 * @param {const char*} format The format of the message
 */
void printInfo(const char* message_format, ...) {
    va_list args;
    va_start(args, message_format);
    printf("[INFO] ");
    vprintf(message_format, args);
    va_end(args);
}

/**
 * @name printWarning
 * Print warnning message like "[WARNING] <message_format>\n" in yellow in terminal.
 * @param {const char*} format The format of the message
 */
void printWarning(const char* message_format, ...) {
    USE_YELLOW_FONT_COLOR;
    va_list args;
    va_start(args, message_format);
    printf("[Warning] ");
    vprintf(message_format, args);
    va_end(args);
    USE_DEFAULT_FONT_COLOR;
}

/**
 * @name printWarning
 * Print error message like "[ERROR] <message_format>\n" in yellow in terminal.
 * @param {const char*} format The format of the message
 */
void printError(const char* message_format, ...) {
    USE_RED_FONT_COLOR;
    va_list args;
    va_start(args, message_format);
    printf("[ERROR] ");
    vprintf(message_format, args);
    va_end(args);
    USE_DEFAULT_FONT_COLOR;
}

/**
 * @name readSubcommand
 * This function reads 1 subcommands from terminal.
 * Existence check is made, but no further checking is done.
 * Print error message in terminal if failed.
 * @param {char**} ret The return address of subcommand read
 * @param {Command} caller The command name of the caller
 * @return {bool} Success or not
 */
bool readSubcommand(char** ret, Command caller) {
    *ret = strtok(NULL, " \n");
    if (!*ret) {
        printError("Insufficient argument.");
        printCommandFormat(caller);
        return false;
    }
    return true;
}

/**
 * @name readAndParseSubcommandToInt
 * This function reads 1 subcommand from terminal, parse it to integer and saves it.
 * Existence check and type check are made, but no further checking is done.
 * Print error message in terminal if failed.
 * @param {int*} ret The return address of parsed interger
 * @param {Command} caller The command name of the caller
 * @return {bool} Success or not
 */

bool readAndParseSubcommandToInt(int* ret, Command caller) {
    char* subcommand = strtok(NULL, " \n");
    if (!subcommand) {
        printError("Insufficient argument.");
        printCommandFormat(caller);
        return false;
    }
    if (!sscanf(subcommand, "%d", ret)) {
        printError("Invalid argument.");
        printCommandFormat(caller);
        return false;
    }
    return true;
}

/**
 * @name readAndParseSubcommandsToInts
 * This function reads multiple subcommands from terminal, parse them to integer and saves them.
 * Existence check and type check are made, but no further checking is done.
 * Print error message in terminal if failed.
 * @param {int} cnt The number of subcommands to read
 * @param {int[]} ret The return array of parsed interger
 * @param {Command} caller The command name of the caller
 * @return {bool} Success or not
 */
bool readAndParseSubcommandsToInts(int cnt, int ret[], Command caller) {
    if (cnt > MAX_INT_INPUT_CNT) {
        printError("Reading too many inputs.");
        printf("The MAX_INT_INPUT_CNT is set to be %d.\n", MAX_INT_INPUT_CNT);
        return false;
    }
    for (int i = 0; i < cnt; i++) {
        char* subcommand = strtok(NULL, " \n");
        if (!subcommand) {
            printError("Insufficient argument.");
            printCommandFormat(caller);
            return false;
        }
        if (sscanf(subcommand, "%d", &ret[i]) != 1) {
            printError("Invalid argument (not an integer).");
            printCommandFormat(caller);
            return false;
        }
    }
    return true;
}

/**
 * @name readAndParseSubcommandToInt
 * This function reads 1 subcommands from terminal, parse it to TreeType and saves it.
 * Existence check and type check are made, but no further checking is done.
 * Print error message in terminal if failed.
 * @param {TreeType*} ret The return address of parsed TreeType
 * @param {Command} caller The command name of the caller
 * @return {bool} Success or not
 */
bool readAndParseSubcommandToTreeType(TreeType* ret, Command caller) {
    char* subcommand = strtok(NULL, " \n");
    if (!subcommand) {
        printError("Insufficient argument.");
        printCommandFormat(caller);
        return false;
    }
    TreeType treeType = string2treetype(subcommand);
    if (treeType == UNKNOWN_TREE_TYPE) {
        printError("Invalid argument.");
        printCommandFormat(caller);
        printf("Tree type can only be [b]inary_[s]earch_[t]ree, [avl]_tree or [spl]ay_tree.\n");
        return false;
    }
    *ret = treeType;
    return true;
}
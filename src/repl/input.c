#include "repl/input.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

size_t input_tokenize(char *line, char *tokens[], size_t max_tokens) {
    size_t count = 0;
    char *cursor = line;

    while (*cursor != '\0') {
        while (isspace((unsigned char)*cursor)) {
            cursor++;
        }

        if (*cursor == '\0' || *cursor == '#') {
            break;
        }

        if (count == max_tokens) {
            break;
        }

        tokens[count++] = cursor;
        while (*cursor != '\0' && !isspace((unsigned char)*cursor)) {
            cursor++;
        }

        if (*cursor == '\0') {
            break;
        }
        *cursor = '\0';
        cursor++;
    }

    return count;
}

bool input_parse_int(const char *text, int *value) {
    if (text == NULL || *text == '\0') {
        return false;
    }

    errno = 0;
    char *end = NULL;
    long parsed = strtol(text, &end, 10);
    if (errno != 0 || end == text || *end != '\0' || parsed < INT_MIN || parsed > INT_MAX) {
        return false;
    }

    if (value != NULL) {
        *value = (int)parsed;
    }
    return true;
}

bool input_parse_size(const char *text, size_t *value) {
    if (text == NULL || *text == '\0' || *text == '-') {
        return false;
    }

    errno = 0;
    char *end = NULL;
    unsigned long parsed = strtoul(text, &end, 10);
    if (errno != 0 || end == text || *end != '\0') {
        return false;
    }

    if (value != NULL) {
        *value = (size_t)parsed;
    }
    return true;
}

tree_type input_parse_tree_type(const char *text) {
    return binary_tree_parse_type(text);
}

repl_command input_parse_command(const char *text) {
    if (text == NULL) {
        return REPL_COMMAND_EMPTY;
    }
    if (strcmp(text, "help") == 0 || strcmp(text, "h") == 0) {
        return REPL_COMMAND_HELP;
    }
    if (strcmp(text, "new") == 0 || strcmp(text, "n") == 0) {
        return REPL_COMMAND_NEW;
    }
    if (strcmp(text, "insert") == 0 || strcmp(text, "i") == 0) {
        return REPL_COMMAND_INSERT;
    }
    if (strcmp(text, "insert-many") == 0 || strcmp(text, "insert_many") == 0 || strcmp(text, "im") == 0) {
        return REPL_COMMAND_INSERT_MANY;
    }
    if (strcmp(text, "delete") == 0 || strcmp(text, "d") == 0) {
        return REPL_COMMAND_DELETE;
    }
    if (strcmp(text, "find") == 0 || strcmp(text, "f") == 0 || strcmp(text, "search") == 0 || strcmp(text, "s") == 0) {
        return REPL_COMMAND_FIND;
    }
    if (strcmp(text, "print") == 0 || strcmp(text, "p") == 0) {
        return REPL_COMMAND_PRINT;
    }
    if (strcmp(text, "dump") == 0) {
        return REPL_COMMAND_DUMP;
    }
    if (strcmp(text, "quit") == 0 || strcmp(text, "q") == 0 || strcmp(text, "exit") == 0) {
        return REPL_COMMAND_QUIT;
    }
    return REPL_COMMAND_UNKNOWN;
}

const char *input_command_name(repl_command command) {
    switch (command) {
        case REPL_COMMAND_HELP:
            return "help";
        case REPL_COMMAND_NEW:
            return "new";
        case REPL_COMMAND_INSERT:
            return "insert";
        case REPL_COMMAND_INSERT_MANY:
            return "insert-many";
        case REPL_COMMAND_DELETE:
            return "delete";
        case REPL_COMMAND_FIND:
            return "find";
        case REPL_COMMAND_PRINT:
            return "print";
        case REPL_COMMAND_DUMP:
            return "dump";
        case REPL_COMMAND_QUIT:
            return "quit";
        case REPL_COMMAND_EMPTY:
            return "empty";
        default:
            return "unknown";
    }
}

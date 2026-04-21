#ifndef TYPE_H_
#define TYPE_H_
#include <stdlib.h>
typedef enum {
    QUIT = 0,
    HELP,
    INSERT,
    INSERT_MANY,
    PRINT,
    DELETE,
    TRAVERSAL,
    NEW,
    DUMP_FOREST,
    SEARCH,
    LOAD_TREE,
    COMMAND_NUMBER,
    UNKNOWN_COMMAND = 999
} Command;

typedef enum {
    IN_ORDER = 0,
    PRE_ORDER,
    POST_ORDER,
    SUBCOMMAND_NUMBER,
    UNKNOWN_SUBCOMMAND = 999
} Subcommand;

typedef enum {
    BST = 0,
    AVL,
    SPL,
    RBT,
    TREE_TYPE_NUMBER,
    UNKNOWN_TREE_TYPE = 999
} TreeType;

typedef struct {
    TreeType type;
    void* root;
} Tree;

typedef Tree* TreePtr;

typedef enum {
    YELLOW = 0,
    RED,
    WHITE,
    COLOR_NUMBER,
    DEFAULT
} FontColor;

typedef struct {
    TreePtr* items;
    size_t count;
    size_t capacity;
} Forest;

typedef Forest* ForestPtr;

#endif  // TYPE_H_
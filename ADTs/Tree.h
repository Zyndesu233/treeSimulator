#ifndef TREE_H_
#define TREE_H_

typedef enum {
    BST = 0,
    AVL,
    SPL,
    RBT,
    TREE_TYPE_NUMBER,
    UNKNOWN_TREE_TYPE = 999
} TreeType;

typedef void* (*tree_insert_fn)(void*, int);
typedef void (*tree_print_fn)(void*);

typedef struct {
    tree_insert_fn insert;
    tree_print_fn print;
} TreeOps;

typedef TreeOps* TreeOpsPtr;

typedef struct {
    TreeType type;
    void* root;
    const TreeOpsPtr ops;
} Tree;

typedef Tree* TreePtr;

TreePtr tree_init(TreeType type);
#endif
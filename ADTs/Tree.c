#include "Tree.h"
#include <assert.h>
#include <stdlib.h>
#include "AVLTree.h"
#include "BinarySearchTree.h"
#include "RedBlackTree.h"
#include "SplayTree.h"

static const TreeOps SPL_ops = {
    .insert = (void* (*)(void*, int))Splay_Insert,
    .print = NULL,
};

static const TreeOps RBT_ops = {
    .insert = (void* (*)(void*, int))Red_Black_Insert,
    .print = NULL,
};

TreePtr tree_init(TreeType type) {
    TreePtr tree = malloc(sizeof(tree));
    tree->type = type;
    tree->root = NULL;

    assert(TREE_TYPE_NUMBER == 4 && "HAVE NOT EXHAUST ALL TREE TYPES");
    switch (type) {
        case AVL:
            break;
        case BST:
            break;
        case SPL:
            tree->ops = &SPL_ops;
            break;
        case RBT:
            tree->ops = &RBT_ops;
            break;
        default:
            assert(false && "UNREACHABLE");
            break;
    }

    return tree;
}
#include "Tree.h"
#include <assert.h>
#include <stdlib.h>
#include "AVLTree.h"
#include "BinarySearchTree.h"
#include "RedBlackTree.h"
#include "SplayTree.h"

static void* BST_Insert(void* tree, int val) {
    return InsertNode((BinaryTreeADT) tree, NewTreeNode(val));
}

static TreeOps AVL_ops = {
    .insert = (void* (*)(void*, int)) AVL_Insert,
    .print = NULL,
};

static TreeOps BST_ops = {
    .insert = (void* (*)(void*, int))BST_Insert,
    .print = NULL,
};

static TreeOps SPL_ops = {
    .insert = (void* (*)(void*, int))Splay_Insert,
    .print = NULL,
};

static TreeOps RBT_ops = {
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
            tree->ops = &AVL_ops;
            break;
        case BST:
            tree->ops = &BST_ops;
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
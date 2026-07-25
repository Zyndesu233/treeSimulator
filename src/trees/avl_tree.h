#ifndef TREE_SIMULATOR_AVL_TREE_H
#define TREE_SIMULATOR_AVL_TREE_H

#include "trees/binary_tree.h"

typedef struct avl_tree {
    binary_tree base;
    int height;
} avl_tree;

const binary_tree_vtable *avl_tree_vtable(void);

#endif

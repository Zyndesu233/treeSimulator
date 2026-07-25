#ifndef TREE_SIMULATOR_SPLAY_TREE_H
#define TREE_SIMULATOR_SPLAY_TREE_H

#include "trees/binary_tree.h"

typedef struct splay_tree {
    binary_tree base;
    struct splay_tree *parent;
} splay_tree;

const binary_tree_vtable *splay_tree_vtable(void);

#endif

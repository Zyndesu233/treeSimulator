#ifndef TREE_SIMULATOR_RED_BLACK_TREE_H
#define TREE_SIMULATOR_RED_BLACK_TREE_H

#include <stdbool.h>

#include "trees/binary_tree.h"

typedef struct red_black_tree {
    binary_tree base;
    bool is_red;
    struct red_black_tree *parent;
} red_black_tree;

const binary_tree_vtable *red_black_tree_vtable(void);
bool red_black_tree_is_valid(const binary_tree *root, char *message, size_t message_size);

#endif

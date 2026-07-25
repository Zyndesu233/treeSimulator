#ifndef TREE_SIMULATOR_BTREE_H
#define TREE_SIMULATOR_BTREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "trees/binary_tree.h"

typedef struct btree_node {
    size_t key_count;
    int *keys;
    struct btree_node **children;
    bool is_leaf;
} btree_node;

typedef struct btree {
    size_t min_degree;
    btree_node *root;
} btree;

btree *btree_create(size_t min_degree);
void btree_free(btree *tree);
bool btree_insert(btree *tree, tree_value value);
bool btree_delete(btree *tree, tree_value value);
bool btree_find(const btree *tree, tree_value value);
bool btree_validate(const btree *tree, char *message, size_t message_size);
void btree_print(FILE *out, const btree *tree);

#endif

#ifndef TREE_SIMULATOR_FOREST_H
#define TREE_SIMULATOR_FOREST_H

#include <stdbool.h>
#include <stddef.h>

#include "trees/binary_tree.h"
#include "trees/btree.h"

typedef struct tree_handle {
    tree_type type;
    const binary_tree_vtable *methods;
    binary_tree *root;
    btree *btree_root;
    size_t btree_min_degree;
} tree_handle;

typedef struct forest {
    tree_handle *items;
    size_t count;
    size_t capacity;
} forest;

void forest_init(forest *forest_ptr);
void forest_free(forest *forest_ptr);
bool forest_create_tree(forest *forest_ptr, tree_type type, size_t *index);
bool forest_create_btree(forest *forest_ptr, size_t min_degree, size_t *index);
tree_handle *forest_get(forest *forest_ptr, size_t index);
const tree_handle *forest_get_const(const forest *forest_ptr, size_t index);
bool forest_insert(tree_handle *tree, tree_value value);
bool forest_delete(tree_handle *tree, tree_value value);
bool forest_find(tree_handle *tree, tree_value value);
bool forest_validate(const tree_handle *tree, char *message, size_t message_size);

#endif

#ifndef TREE_SIMULATOR_BINARY_TREE_H
#define TREE_SIMULATOR_BINARY_TREE_H

#include <stdbool.h>
#include <stddef.h>

typedef int tree_value;

typedef enum tree_type {
    TREE_TYPE_BST = 0,
    TREE_TYPE_AVL = 1,
    TREE_TYPE_SPLAY = 2,
    TREE_TYPE_RED_BLACK = 3,
    TREE_TYPE_BTREE = 4,
    TREE_TYPE_UNKNOWN = 999
} tree_type;

typedef struct binary_tree binary_tree;
typedef struct binary_tree_vtable binary_tree_vtable;

struct binary_tree {
    tree_value value;
    binary_tree *left_child;
    binary_tree *right_child;
    const binary_tree_vtable *methods;
};

struct binary_tree_vtable {
    const char *name;
    const char *short_name;
    binary_tree *(*insert)(binary_tree *root, tree_value value, bool *inserted);
    binary_tree *(*delete)(binary_tree *root, tree_value value, bool *deleted);
    binary_tree *(*find)(binary_tree *root, tree_value value, bool *found);
    void (*free)(binary_tree *root);
    bool (*validate)(const binary_tree *root, char *message, size_t message_size);
    void (*format_node)(const binary_tree *node, char *buffer, size_t buffer_size);
};

void binary_tree_init(binary_tree *node, tree_value value, const binary_tree_vtable *methods);
tree_type binary_tree_parse_type(const char *text);
const char *binary_tree_type_name(tree_type type);
const binary_tree_vtable *binary_tree_vtable_for_type(tree_type type);
void binary_tree_default_format_node(const binary_tree *node, char *buffer, size_t buffer_size);

#endif

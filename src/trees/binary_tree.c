#include "trees/binary_tree.h"

#include <stdio.h>
#include <string.h>

#include "trees/avl_tree.h"
#include "trees/binary_search_tree.h"
#include "trees/red_black_tree.h"
#include "trees/splay_tree.h"

void binary_tree_init(binary_tree *node, tree_value value, const binary_tree_vtable *methods) {
    node->value = value;
    node->left_child = NULL;
    node->right_child = NULL;
    node->methods = methods;
}

tree_type binary_tree_parse_type(const char *text) {
    if (text == NULL) {
        return TREE_TYPE_UNKNOWN;
    }
    if (strcmp(text, "bst") == 0 || strcmp(text, "binary-search-tree") == 0) {
        return TREE_TYPE_BST;
    }
    if (strcmp(text, "avl") == 0 || strcmp(text, "avl-tree") == 0) {
        return TREE_TYPE_AVL;
    }
    if (strcmp(text, "splay") == 0 || strcmp(text, "spl") == 0 || strcmp(text, "splay-tree") == 0) {
        return TREE_TYPE_SPLAY;
    }
    if (strcmp(text, "rbt") == 0 || strcmp(text, "red-black-tree") == 0) {
        return TREE_TYPE_RED_BLACK;
    }
    if (strcmp(text, "btree") == 0 || strcmp(text, "b-tree") == 0 || strcmp(text, "bt") == 0) {
        return TREE_TYPE_BTREE;
    }
    return TREE_TYPE_UNKNOWN;
}

const char *binary_tree_type_name(tree_type type) {
    switch (type) {
        case TREE_TYPE_BST:
            return "binary search tree";
        case TREE_TYPE_AVL:
            return "AVL tree";
        case TREE_TYPE_SPLAY:
            return "splay tree";
        case TREE_TYPE_RED_BLACK:
            return "red-black tree";
        case TREE_TYPE_BTREE:
            return "B-tree";
        default:
            return "unknown tree";
    }
}

const binary_tree_vtable *binary_tree_vtable_for_type(tree_type type) {
    switch (type) {
        case TREE_TYPE_BST:
            return binary_search_tree_vtable();
        case TREE_TYPE_AVL:
            return avl_tree_vtable();
        case TREE_TYPE_SPLAY:
            return splay_tree_vtable();
        case TREE_TYPE_RED_BLACK:
            return red_black_tree_vtable();
        default:
            return NULL;
    }
}

void binary_tree_default_format_node(const binary_tree *node, char *buffer, size_t buffer_size) {
    if (node == NULL || buffer_size == 0) {
        return;
    }
    snprintf(buffer, buffer_size, "%d", node->value);
}

#include "trees/binary_search_tree.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static binary_tree *binary_search_tree_new_node(tree_value value) {
    binary_tree *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    binary_tree_init(node, value, binary_search_tree_vtable());
    return node;
}

static binary_tree *binary_search_tree_insert_node(binary_tree *root, tree_value value, bool *inserted) {
    if (root == NULL) {
        binary_tree *node = binary_search_tree_new_node(value);
        if (inserted != NULL) {
            *inserted = node != NULL;
        }
        return node;
    }

    if (value == root->value) {
        if (inserted != NULL) {
            *inserted = false;
        }
        return root;
    }

    if (value < root->value) {
        root->left_child = binary_search_tree_insert_node(root->left_child, value, inserted);
    } else {
        root->right_child = binary_search_tree_insert_node(root->right_child, value, inserted);
    }
    return root;
}

static binary_tree *binary_search_tree_min_node(binary_tree *root) {
    binary_tree *current = root;
    while (current != NULL && current->left_child != NULL) {
        current = current->left_child;
    }
    return current;
}

static binary_tree *binary_search_tree_delete_node(binary_tree *root, tree_value value, bool *deleted) {
    if (root == NULL) {
        if (deleted != NULL) {
            *deleted = false;
        }
        return NULL;
    }

    if (value < root->value) {
        root->left_child = binary_search_tree_delete_node(root->left_child, value, deleted);
        return root;
    }
    if (value > root->value) {
        root->right_child = binary_search_tree_delete_node(root->right_child, value, deleted);
        return root;
    }

    if (deleted != NULL) {
        *deleted = true;
    }

    if (root->left_child == NULL) {
        binary_tree *next = root->right_child;
        free(root);
        return next;
    }
    if (root->right_child == NULL) {
        binary_tree *next = root->left_child;
        free(root);
        return next;
    }

    binary_tree *successor = binary_search_tree_min_node(root->right_child);
    bool ignored = false;
    root->value = successor->value;
    root->right_child = binary_search_tree_delete_node(root->right_child, successor->value, &ignored);
    return root;
}

static binary_tree *binary_search_tree_find_node(binary_tree *root, tree_value value, bool *found) {
    const binary_tree *current = root;
    bool exists = false;
    while (current != NULL) {
        if (value == current->value) {
            exists = true;
            break;
        }
        current = value < current->value ? current->left_child : current->right_child;
    }
    if (found != NULL) {
        *found = exists;
    }
    return root;
}

static void binary_search_tree_free_tree(binary_tree *root) {
    if (root == NULL) {
        return;
    }
    binary_search_tree_free_tree(root->left_child);
    binary_search_tree_free_tree(root->right_child);
    free(root);
}

static bool binary_search_tree_validate_range(const binary_tree *root, long long min, long long max) {
    if (root == NULL) {
        return true;
    }
    if ((long long)root->value <= min || (long long)root->value >= max) {
        return false;
    }
    return binary_search_tree_validate_range(root->left_child, min, root->value) &&
           binary_search_tree_validate_range(root->right_child, root->value, max);
}

static bool binary_search_tree_validate_tree(const binary_tree *root, char *message, size_t message_size) {
    bool valid = binary_search_tree_validate_range(root, (long long)INT_MIN - 1LL, (long long)INT_MAX + 1LL);
    if (message != NULL && message_size > 0) {
        snprintf(message, message_size, "%s", valid ? "ok" : "binary search tree ordering invariant failed");
    }
    return valid;
}

static const binary_tree_vtable BINARY_SEARCH_TREE_VTABLE = {
    .name = "binary search tree",
    .short_name = "bst",
    .insert = binary_search_tree_insert_node,
    .delete = binary_search_tree_delete_node,
    .find = binary_search_tree_find_node,
    .free = binary_search_tree_free_tree,
    .validate = binary_search_tree_validate_tree,
    .format_node = binary_tree_default_format_node,
};

const binary_tree_vtable *binary_search_tree_vtable(void) {
    return &BINARY_SEARCH_TREE_VTABLE;
}

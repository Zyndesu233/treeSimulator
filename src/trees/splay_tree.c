#include "trees/splay_tree.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static splay_tree *as_splay(binary_tree *node) {
    return (splay_tree *)node;
}

static const splay_tree *as_const_splay(const binary_tree *node) {
    return (const splay_tree *)node;
}

static splay_tree *left_of(splay_tree *node) {
    return node == NULL ? NULL : as_splay(node->base.left_child);
}

static splay_tree *right_of(splay_tree *node) {
    return node == NULL ? NULL : as_splay(node->base.right_child);
}

static const splay_tree *const_left_of(const splay_tree *node) {
    return node == NULL ? NULL : as_const_splay(node->base.left_child);
}

static const splay_tree *const_right_of(const splay_tree *node) {
    return node == NULL ? NULL : as_const_splay(node->base.right_child);
}

static splay_tree *splay_new_node(tree_value value) {
    splay_tree *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    binary_tree_init(&node->base, value, splay_tree_vtable());
    node->parent = NULL;
    return node;
}

static void rotate_left(splay_tree *pivot) {
    splay_tree *child = right_of(pivot);
    if (child == NULL) {
        return;
    }

    pivot->base.right_child = child->base.left_child;
    if (child->base.left_child != NULL) {
        as_splay(child->base.left_child)->parent = pivot;
    }

    child->parent = pivot->parent;
    if (pivot->parent != NULL) {
        if (pivot == left_of(pivot->parent)) {
            pivot->parent->base.left_child = &child->base;
        } else {
            pivot->parent->base.right_child = &child->base;
        }
    }

    child->base.left_child = &pivot->base;
    pivot->parent = child;
}

static void rotate_right(splay_tree *pivot) {
    splay_tree *child = left_of(pivot);
    if (child == NULL) {
        return;
    }

    pivot->base.left_child = child->base.right_child;
    if (child->base.right_child != NULL) {
        as_splay(child->base.right_child)->parent = pivot;
    }

    child->parent = pivot->parent;
    if (pivot->parent != NULL) {
        if (pivot == right_of(pivot->parent)) {
            pivot->parent->base.right_child = &child->base;
        } else {
            pivot->parent->base.left_child = &child->base;
        }
    }

    child->base.right_child = &pivot->base;
    pivot->parent = child;
}

static splay_tree *splay_to_root(splay_tree *node) {
    if (node == NULL) {
        return NULL;
    }

    while (node->parent != NULL) {
        splay_tree *parent = node->parent;
        splay_tree *grandparent = parent->parent;

        if (grandparent == NULL) {
            if (node == left_of(parent)) {
                rotate_right(parent);
            } else {
                rotate_left(parent);
            }
        } else if (node == left_of(parent) && parent == left_of(grandparent)) {
            rotate_right(grandparent);
            rotate_right(parent);
        } else if (node == right_of(parent) && parent == right_of(grandparent)) {
            rotate_left(grandparent);
            rotate_left(parent);
        } else if (node == right_of(parent) && parent == left_of(grandparent)) {
            rotate_left(parent);
            rotate_right(grandparent);
        } else {
            rotate_right(parent);
            rotate_left(grandparent);
        }
    }

    return node;
}

static binary_tree *splay_insert_node(binary_tree *root_node, tree_value value, bool *inserted) {
    if (root_node == NULL) {
        splay_tree *node = splay_new_node(value);
        if (inserted != NULL) {
            *inserted = node != NULL;
        }
        return node == NULL ? NULL : &node->base;
    }

    splay_tree *current = as_splay(root_node);
    splay_tree *parent = NULL;
    while (current != NULL) {
        parent = current;
        if (value == current->base.value) {
            if (inserted != NULL) {
                *inserted = false;
            }
            return &splay_to_root(current)->base;
        }
        current = value < current->base.value ? left_of(current) : right_of(current);
    }

    splay_tree *node = splay_new_node(value);
    if (node == NULL) {
        if (inserted != NULL) {
            *inserted = false;
        }
        return root_node;
    }

    node->parent = parent;
    if (value < parent->base.value) {
        parent->base.left_child = &node->base;
    } else {
        parent->base.right_child = &node->base;
    }

    if (inserted != NULL) {
        *inserted = true;
    }
    return &splay_to_root(node)->base;
}

static binary_tree *splay_find_node(binary_tree *root, tree_value value, bool *found) {
    splay_tree *current = as_splay(root);
    splay_tree *last = NULL;
    bool exists = false;

    while (current != NULL) {
        last = current;
        if (value == current->base.value) {
            exists = true;
            break;
        }
        current = value < current->base.value ? left_of(current) : right_of(current);
    }

    if (found != NULL) {
        *found = exists;
    }
    return last == NULL ? root : &splay_to_root(last)->base;
}

static splay_tree *maximum_node(splay_tree *root) {
    splay_tree *current = root;
    while (right_of(current) != NULL) {
        current = right_of(current);
    }
    return current;
}

static binary_tree *splay_delete_node(binary_tree *root_node, tree_value value, bool *deleted) {
    bool found = false;
    root_node = splay_find_node(root_node, value, &found);
    if (!found || root_node == NULL) {
        if (deleted != NULL) {
            *deleted = false;
        }
        return root_node;
    }

    splay_tree *root = as_splay(root_node);
    binary_tree *left_subtree = root->base.left_child;
    binary_tree *right_subtree = root->base.right_child;

    if (left_subtree != NULL) {
        as_splay(left_subtree)->parent = NULL;
    }
    if (right_subtree != NULL) {
        as_splay(right_subtree)->parent = NULL;
    }

    free(root);
    if (deleted != NULL) {
        *deleted = true;
    }

    if (left_subtree == NULL) {
        return right_subtree;
    }

    splay_tree *new_root = splay_to_root(maximum_node(as_splay(left_subtree)));
    new_root->base.right_child = right_subtree;
    if (right_subtree != NULL) {
        as_splay(right_subtree)->parent = new_root;
    }
    return &new_root->base;
}

static void splay_free_tree(binary_tree *root) {
    if (root == NULL) {
        return;
    }
    splay_free_tree(root->left_child);
    splay_free_tree(root->right_child);
    free(as_splay(root));
}

static void set_validation_message(char *message, size_t message_size, const char *text) {
    if (message != NULL && message_size > 0) {
        snprintf(message, message_size, "%s", text);
    }
}

static bool validate_subtree(
    const splay_tree *node,
    const splay_tree *expected_parent,
    long long min,
    long long max,
    char *message,
    size_t message_size
) {
    if (node == NULL) {
        return true;
    }

    if (node->parent != expected_parent) {
        set_validation_message(message, message_size, "splay parent link invariant failed");
        return false;
    }

    if ((long long)node->base.value <= min || (long long)node->base.value >= max) {
        set_validation_message(message, message_size, "splay ordering invariant failed");
        return false;
    }

    return validate_subtree(const_left_of(node), node, min, node->base.value, message, message_size) &&
           validate_subtree(const_right_of(node), node, node->base.value, max, message, message_size);
}

static bool splay_validate_tree(const binary_tree *root, char *message, size_t message_size) {
    bool valid = validate_subtree(
        as_const_splay(root),
        NULL,
        (long long)INT_MIN - 1LL,
        (long long)INT_MAX + 1LL,
        message,
        message_size
    );
    if (valid) {
        set_validation_message(message, message_size, "ok");
    }
    return valid;
}

static const binary_tree_vtable SPLAY_VTABLE = {
    .name = "splay tree",
    .short_name = "splay",
    .insert = splay_insert_node,
    .delete = splay_delete_node,
    .find = splay_find_node,
    .free = splay_free_tree,
    .validate = splay_validate_tree,
    .format_node = binary_tree_default_format_node,
};

const binary_tree_vtable *splay_tree_vtable(void) {
    return &SPLAY_VTABLE;
}

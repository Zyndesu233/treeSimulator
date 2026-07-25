#include "trees/red_black_tree.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define ANSI_RED "\033[31m"
#define ANSI_RESET "\033[0m"

static red_black_tree *as_rbt(binary_tree *node) {
    return (red_black_tree *)node;
}

static const red_black_tree *as_const_rbt(const binary_tree *node) {
    return (const red_black_tree *)node;
}

static red_black_tree *left_of(red_black_tree *node) {
    return node == NULL ? NULL : as_rbt(node->base.left_child);
}

static red_black_tree *right_of(red_black_tree *node) {
    return node == NULL ? NULL : as_rbt(node->base.right_child);
}

static const red_black_tree *const_left_of(const red_black_tree *node) {
    return node == NULL ? NULL : as_const_rbt(node->base.left_child);
}

static const red_black_tree *const_right_of(const red_black_tree *node) {
    return node == NULL ? NULL : as_const_rbt(node->base.right_child);
}

static bool is_red(const red_black_tree *node) {
    return node != NULL && node->is_red;
}

static bool is_black(const red_black_tree *node) {
    return node == NULL || !node->is_red;
}

static red_black_tree *rbt_new_node(tree_value value) {
    red_black_tree *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    binary_tree_init(&node->base, value, red_black_tree_vtable());
    node->is_red = true;
    node->parent = NULL;
    return node;
}

static void rotate_left(red_black_tree **root, red_black_tree *pivot) {
    red_black_tree *child = right_of(pivot);
    if (child == NULL) {
        return;
    }

    pivot->base.right_child = child->base.left_child;
    if (child->base.left_child != NULL) {
        as_rbt(child->base.left_child)->parent = pivot;
    }

    child->parent = pivot->parent;
    if (pivot->parent == NULL) {
        *root = child;
    } else if (pivot == left_of(pivot->parent)) {
        pivot->parent->base.left_child = &child->base;
    } else {
        pivot->parent->base.right_child = &child->base;
    }

    child->base.left_child = &pivot->base;
    pivot->parent = child;
}

static void rotate_right(red_black_tree **root, red_black_tree *pivot) {
    red_black_tree *child = left_of(pivot);
    if (child == NULL) {
        return;
    }

    pivot->base.left_child = child->base.right_child;
    if (child->base.right_child != NULL) {
        as_rbt(child->base.right_child)->parent = pivot;
    }

    child->parent = pivot->parent;
    if (pivot->parent == NULL) {
        *root = child;
    } else if (pivot == right_of(pivot->parent)) {
        pivot->parent->base.right_child = &child->base;
    } else {
        pivot->parent->base.left_child = &child->base;
    }

    child->base.right_child = &pivot->base;
    pivot->parent = child;
}

static void insert_fixup(red_black_tree **root, red_black_tree *node) {
    while (node->parent != NULL && node->parent->is_red) {
        red_black_tree *parent = node->parent;
        red_black_tree *grandparent = parent->parent;

        if (parent == left_of(grandparent)) {
            red_black_tree *uncle = right_of(grandparent);
            if (is_red(uncle)) {
                parent->is_red = false;
                uncle->is_red = false;
                grandparent->is_red = true;
                node = grandparent;
            } else {
                if (node == right_of(parent)) {
                    node = parent;
                    rotate_left(root, node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }
                parent->is_red = false;
                grandparent->is_red = true;
                rotate_right(root, grandparent);
            }
        } else {
            red_black_tree *uncle = left_of(grandparent);
            if (is_red(uncle)) {
                parent->is_red = false;
                uncle->is_red = false;
                grandparent->is_red = true;
                node = grandparent;
            } else {
                if (node == left_of(parent)) {
                    node = parent;
                    rotate_right(root, node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }
                parent->is_red = false;
                grandparent->is_red = true;
                rotate_left(root, grandparent);
            }
        }
    }
    (*root)->is_red = false;
    (*root)->parent = NULL;
}

static binary_tree *rbt_insert_node(binary_tree *root_node, tree_value value, bool *inserted) {
    red_black_tree *root = as_rbt(root_node);
    red_black_tree *parent = NULL;
    red_black_tree *current = root;

    while (current != NULL) {
        parent = current;
        if (value == current->base.value) {
            if (inserted != NULL) {
                *inserted = false;
            }
            return root_node;
        }
        current = value < current->base.value ? left_of(current) : right_of(current);
    }

    red_black_tree *node = rbt_new_node(value);
    if (node == NULL) {
        if (inserted != NULL) {
            *inserted = false;
        }
        return root_node;
    }

    node->parent = parent;
    if (parent == NULL) {
        root = node;
    } else if (value < parent->base.value) {
        parent->base.left_child = &node->base;
    } else {
        parent->base.right_child = &node->base;
    }

    insert_fixup(&root, node);
    if (inserted != NULL) {
        *inserted = true;
    }
    return &root->base;
}

static red_black_tree *rbt_find_node(red_black_tree *root, tree_value value) {
    red_black_tree *current = root;
    while (current != NULL) {
        if (value == current->base.value) {
            return current;
        }
        current = value < current->base.value ? left_of(current) : right_of(current);
    }
    return NULL;
}

static binary_tree *rbt_find_value(binary_tree *root, tree_value value, bool *found) {
    const red_black_tree *current = as_const_rbt(root);
    bool exists = false;
    while (current != NULL) {
        if (value == current->base.value) {
            exists = true;
            break;
        }
        current = value < current->base.value ? const_left_of(current) : const_right_of(current);
    }
    if (found != NULL) {
        *found = exists;
    }
    return root;
}

static red_black_tree *minimum_node(red_black_tree *root) {
    red_black_tree *current = root;
    while (left_of(current) != NULL) {
        current = left_of(current);
    }
    return current;
}

static void transplant(red_black_tree **root, red_black_tree *old_node, red_black_tree *new_node) {
    if (old_node->parent == NULL) {
        *root = new_node;
    } else if (old_node == left_of(old_node->parent)) {
        old_node->parent->base.left_child = new_node == NULL ? NULL : &new_node->base;
    } else {
        old_node->parent->base.right_child = new_node == NULL ? NULL : &new_node->base;
    }

    if (new_node != NULL) {
        new_node->parent = old_node->parent;
    }
}

static void delete_fixup(red_black_tree **root, red_black_tree *node, red_black_tree *parent) {
    while (node != *root && is_black(node)) {
        if (parent == NULL) {
            break;
        }

        if (node == left_of(parent)) {
            red_black_tree *sibling = right_of(parent);
            if (is_red(sibling)) {
                sibling->is_red = false;
                parent->is_red = true;
                rotate_left(root, parent);
                sibling = right_of(parent);
            }

            if (sibling == NULL) {
                node = parent;
                parent = node->parent;
                continue;
            }

            if (is_black(left_of(sibling)) && is_black(right_of(sibling))) {
                sibling->is_red = true;
                node = parent;
                parent = node->parent;
            } else {
                if (is_black(right_of(sibling))) {
                    red_black_tree *left_child = left_of(sibling);
                    if (left_child != NULL) {
                        left_child->is_red = false;
                    }
                    sibling->is_red = true;
                    rotate_right(root, sibling);
                    sibling = right_of(parent);
                }

                if (sibling != NULL) {
                    sibling->is_red = parent->is_red;
                    red_black_tree *right_child = right_of(sibling);
                    if (right_child != NULL) {
                        right_child->is_red = false;
                    }
                }
                parent->is_red = false;
                rotate_left(root, parent);
                node = *root;
                parent = NULL;
            }
        } else {
            red_black_tree *sibling = left_of(parent);
            if (is_red(sibling)) {
                sibling->is_red = false;
                parent->is_red = true;
                rotate_right(root, parent);
                sibling = left_of(parent);
            }

            if (sibling == NULL) {
                node = parent;
                parent = node->parent;
                continue;
            }

            if (is_black(right_of(sibling)) && is_black(left_of(sibling))) {
                sibling->is_red = true;
                node = parent;
                parent = node->parent;
            } else {
                if (is_black(left_of(sibling))) {
                    red_black_tree *right_child = right_of(sibling);
                    if (right_child != NULL) {
                        right_child->is_red = false;
                    }
                    sibling->is_red = true;
                    rotate_left(root, sibling);
                    sibling = left_of(parent);
                }

                if (sibling != NULL) {
                    sibling->is_red = parent->is_red;
                    red_black_tree *left_child = left_of(sibling);
                    if (left_child != NULL) {
                        left_child->is_red = false;
                    }
                }
                parent->is_red = false;
                rotate_right(root, parent);
                node = *root;
                parent = NULL;
            }
        }
    }

    if (node != NULL) {
        node->is_red = false;
    }
    if (*root != NULL) {
        (*root)->parent = NULL;
        (*root)->is_red = false;
    }
}

static binary_tree *rbt_delete_node(binary_tree *root_node, tree_value value, bool *deleted) {
    red_black_tree *root = as_rbt(root_node);
    red_black_tree *target = rbt_find_node(root, value);

    if (target == NULL) {
        if (deleted != NULL) {
            *deleted = false;
        }
        return root_node;
    }

    red_black_tree *moved = target;
    bool moved_was_red = moved->is_red;
    red_black_tree *fixup_node = NULL;
    red_black_tree *fixup_parent = NULL;

    if (left_of(target) == NULL) {
        fixup_node = right_of(target);
        fixup_parent = target->parent;
        transplant(&root, target, fixup_node);
    } else if (right_of(target) == NULL) {
        fixup_node = left_of(target);
        fixup_parent = target->parent;
        transplant(&root, target, fixup_node);
    } else {
        moved = minimum_node(right_of(target));
        moved_was_red = moved->is_red;
        fixup_node = right_of(moved);

        if (moved->parent == target) {
            fixup_parent = moved;
            if (fixup_node != NULL) {
                fixup_node->parent = moved;
            }
        } else {
            fixup_parent = moved->parent;
            transplant(&root, moved, fixup_node);
            moved->base.right_child = target->base.right_child;
            right_of(moved)->parent = moved;
        }

        transplant(&root, target, moved);
        moved->base.left_child = target->base.left_child;
        left_of(moved)->parent = moved;
        moved->is_red = target->is_red;
    }

    free(target);
    if (!moved_was_red) {
        delete_fixup(&root, fixup_node, fixup_parent);
    }

    if (root != NULL) {
        root->parent = NULL;
        root->is_red = false;
    }
    if (deleted != NULL) {
        *deleted = true;
    }
    return root == NULL ? NULL : &root->base;
}

static void rbt_free_tree(binary_tree *root) {
    if (root == NULL) {
        return;
    }
    rbt_free_tree(root->left_child);
    rbt_free_tree(root->right_child);
    free(as_rbt(root));
}

static void set_validation_message(char *message, size_t message_size, const char *text) {
    if (message != NULL && message_size > 0) {
        snprintf(message, message_size, "%s", text);
    }
}

static int validate_subtree(
    const red_black_tree *node,
    const red_black_tree *expected_parent,
    long long min,
    long long max,
    char *message,
    size_t message_size
) {
    if (node == NULL) {
        return 1;
    }

    if (node->parent != expected_parent) {
        set_validation_message(message, message_size, "red-black parent link invariant failed");
        return -1;
    }

    if ((long long)node->base.value <= min || (long long)node->base.value >= max) {
        set_validation_message(message, message_size, "red-black ordering invariant failed");
        return -1;
    }

    if (node->is_red && (is_red(const_left_of(node)) || is_red(const_right_of(node)))) {
        set_validation_message(message, message_size, "red node has a red child");
        return -1;
    }

    int left_height = validate_subtree(
        const_left_of(node),
        node,
        min,
        node->base.value,
        message,
        message_size
    );
    if (left_height < 0) {
        return -1;
    }

    int right_height = validate_subtree(
        const_right_of(node),
        node,
        node->base.value,
        max,
        message,
        message_size
    );
    if (right_height < 0) {
        return -1;
    }

    if (left_height != right_height) {
        set_validation_message(message, message_size, "red-black black-height invariant failed");
        return -1;
    }

    return left_height + (node->is_red ? 0 : 1);
}

bool red_black_tree_is_valid(const binary_tree *root, char *message, size_t message_size) {
    const red_black_tree *rbt_root = as_const_rbt(root);
    if (rbt_root == NULL) {
        set_validation_message(message, message_size, "ok");
        return true;
    }

    if (rbt_root->is_red) {
        set_validation_message(message, message_size, "red-black root is red");
        return false;
    }

    bool valid = validate_subtree(
        rbt_root,
        NULL,
        (long long)INT_MIN - 1LL,
        (long long)INT_MAX + 1LL,
        message,
        message_size
    ) > 0;
    if (valid) {
        set_validation_message(message, message_size, "ok");
    }
    return valid;
}

static void rbt_format_node(const binary_tree *node, char *buffer, size_t buffer_size) {
    const red_black_tree *rbt_node = as_const_rbt(node);
    if (rbt_node == NULL || buffer_size == 0) {
        return;
    }
    if (rbt_node->is_red) {
        snprintf(buffer, buffer_size, ANSI_RED "%d" ANSI_RESET, rbt_node->base.value);
    } else {
        snprintf(buffer, buffer_size, "%d", rbt_node->base.value);
    }
}

static const binary_tree_vtable RBT_VTABLE = {
    .name = "red-black tree",
    .short_name = "rbt",
    .insert = rbt_insert_node,
    .delete = rbt_delete_node,
    .find = rbt_find_value,
    .free = rbt_free_tree,
    .validate = red_black_tree_is_valid,
    .format_node = rbt_format_node,
};

const binary_tree_vtable *red_black_tree_vtable(void) {
    return &RBT_VTABLE;
}

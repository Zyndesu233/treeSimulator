#include "trees/avl_tree.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static avl_tree *as_avl(binary_tree *node) {
    return (avl_tree *)node;
}

static const avl_tree *as_const_avl(const binary_tree *node) {
    return (const avl_tree *)node;
}

static int height_of(const binary_tree *node) {
    return node == NULL ? 0 : as_const_avl(node)->height;
}

static int max_int(int left, int right) {
    return left > right ? left : right;
}

static void update_height(avl_tree *node) {
    node->height = 1 + max_int(height_of(node->base.left_child), height_of(node->base.right_child));
}

static int balance_factor(const avl_tree *node) {
    return height_of(node->base.left_child) - height_of(node->base.right_child);
}

static avl_tree *avl_new_node(tree_value value) {
    avl_tree *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    binary_tree_init(&node->base, value, avl_tree_vtable());
    node->height = 1;
    return node;
}

static avl_tree *rotate_right(avl_tree *root) {
    avl_tree *new_root = as_avl(root->base.left_child);
    binary_tree *moved_subtree = new_root->base.right_child;

    new_root->base.right_child = &root->base;
    root->base.left_child = moved_subtree;

    update_height(root);
    update_height(new_root);
    return new_root;
}

static avl_tree *rotate_left(avl_tree *root) {
    avl_tree *new_root = as_avl(root->base.right_child);
    binary_tree *moved_subtree = new_root->base.left_child;

    new_root->base.left_child = &root->base;
    root->base.right_child = moved_subtree;

    update_height(root);
    update_height(new_root);
    return new_root;
}

static avl_tree *rebalance(avl_tree *root) {
    if (root == NULL) {
        return NULL;
    }

    update_height(root);
    int balance = balance_factor(root);

    if (balance > 1) {
        avl_tree *left_child = as_avl(root->base.left_child);
        if (balance_factor(left_child) < 0) {
            root->base.left_child = &rotate_left(left_child)->base;
        }
        return rotate_right(root);
    }

    if (balance < -1) {
        avl_tree *right_child = as_avl(root->base.right_child);
        if (balance_factor(right_child) > 0) {
            root->base.right_child = &rotate_right(right_child)->base;
        }
        return rotate_left(root);
    }

    return root;
}

static binary_tree *avl_insert_node(binary_tree *root_node, tree_value value, bool *inserted) {
    if (root_node == NULL) {
        avl_tree *node = avl_new_node(value);
        if (inserted != NULL) {
            *inserted = node != NULL;
        }
        return node == NULL ? NULL : &node->base;
    }

    avl_tree *root = as_avl(root_node);
    if (value == root->base.value) {
        if (inserted != NULL) {
            *inserted = false;
        }
        return root_node;
    }

    if (value < root->base.value) {
        root->base.left_child = avl_insert_node(root->base.left_child, value, inserted);
    } else {
        root->base.right_child = avl_insert_node(root->base.right_child, value, inserted);
    }

    return &rebalance(root)->base;
}

static avl_tree *minimum_node(avl_tree *root) {
    avl_tree *current = root;
    while (current->base.left_child != NULL) {
        current = as_avl(current->base.left_child);
    }
    return current;
}

static binary_tree *avl_delete_node(binary_tree *root_node, tree_value value, bool *deleted) {
    if (root_node == NULL) {
        if (deleted != NULL) {
            *deleted = false;
        }
        return NULL;
    }

    avl_tree *root = as_avl(root_node);
    if (value < root->base.value) {
        root->base.left_child = avl_delete_node(root->base.left_child, value, deleted);
    } else if (value > root->base.value) {
        root->base.right_child = avl_delete_node(root->base.right_child, value, deleted);
    } else {
        if (deleted != NULL) {
            *deleted = true;
        }

        if (root->base.left_child == NULL || root->base.right_child == NULL) {
            binary_tree *replacement = root->base.left_child != NULL ? root->base.left_child : root->base.right_child;
            free(root);
            return replacement == NULL ? NULL : &rebalance(as_avl(replacement))->base;
        }

        avl_tree *successor = minimum_node(as_avl(root->base.right_child));
        bool ignored = false;
        root->base.value = successor->base.value;
        root->base.right_child = avl_delete_node(root->base.right_child, successor->base.value, &ignored);
    }

    return &rebalance(root)->base;
}

static binary_tree *avl_find_node(binary_tree *root, tree_value value, bool *found) {
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

static void avl_free_tree(binary_tree *root) {
    if (root == NULL) {
        return;
    }
    avl_free_tree(root->left_child);
    avl_free_tree(root->right_child);
    free(as_avl(root));
}

static void set_validation_message(char *message, size_t message_size, const char *text) {
    if (message != NULL && message_size > 0) {
        snprintf(message, message_size, "%s", text);
    }
}

static int validate_subtree(const avl_tree *node, long long min, long long max, char *message, size_t message_size) {
    if (node == NULL) {
        return 0;
    }

    if ((long long)node->base.value <= min || (long long)node->base.value >= max) {
        set_validation_message(message, message_size, "AVL ordering invariant failed");
        return -1;
    }

    int left_height = validate_subtree(as_const_avl(node->base.left_child), min, node->base.value, message, message_size);
    if (left_height < 0) {
        return -1;
    }

    int right_height = validate_subtree(as_const_avl(node->base.right_child), node->base.value, max, message, message_size);
    if (right_height < 0) {
        return -1;
    }

    int expected_height = 1 + max_int(left_height, right_height);
    if (node->height != expected_height) {
        set_validation_message(message, message_size, "AVL height metadata invariant failed");
        return -1;
    }

    int balance = left_height - right_height;
    if (balance < -1 || balance > 1) {
        set_validation_message(message, message_size, "AVL balance invariant failed");
        return -1;
    }

    return expected_height;
}

static bool avl_validate_tree(const binary_tree *root, char *message, size_t message_size) {
    bool valid = validate_subtree(
        as_const_avl(root),
        (long long)INT_MIN - 1LL,
        (long long)INT_MAX + 1LL,
        message,
        message_size
    ) >= 0;
    if (valid) {
        set_validation_message(message, message_size, "ok");
    }
    return valid;
}

static const binary_tree_vtable AVL_VTABLE = {
    .name = "AVL tree",
    .short_name = "avl",
    .insert = avl_insert_node,
    .delete = avl_delete_node,
    .find = avl_find_node,
    .free = avl_free_tree,
    .validate = avl_validate_tree,
    .format_node = binary_tree_default_format_node,
};

const binary_tree_vtable *avl_tree_vtable(void) {
    return &AVL_VTABLE;
}

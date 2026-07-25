#include "trees/btree.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static btree_node *btree_node_create(const btree *tree, bool is_leaf) {
    btree_node *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    const size_t max_keys = 2 * tree->min_degree - 1;
    const size_t max_children = 2 * tree->min_degree;
    node->keys = calloc(max_keys, sizeof(*node->keys));
    node->children = calloc(max_children, sizeof(*node->children));
    if (node->keys == NULL || node->children == NULL) {
        free(node->keys);
        free(node->children);
        free(node);
        return NULL;
    }

    node->key_count = 0;
    node->is_leaf = is_leaf;
    return node;
}

static void btree_node_free(btree_node *node) {
    if (node == NULL) {
        return;
    }

    if (!node->is_leaf) {
        for (size_t i = 0; i <= node->key_count; i++) {
            btree_node_free(node->children[i]);
        }
    }
    free(node->keys);
    free(node->children);
    free(node);
}

btree *btree_create(size_t min_degree) {
    if (min_degree < 2) {
        return NULL;
    }

    btree *tree = malloc(sizeof(*tree));
    if (tree == NULL) {
        return NULL;
    }

    tree->min_degree = min_degree;
    tree->root = btree_node_create(tree, true);
    if (tree->root == NULL) {
        free(tree);
        return NULL;
    }
    return tree;
}

void btree_free(btree *tree) {
    if (tree == NULL) {
        return;
    }
    btree_node_free(tree->root);
    free(tree);
}

static bool node_find(const btree_node *node, tree_value value) {
    size_t index = 0;
    while (index < node->key_count && value > node->keys[index]) {
        index++;
    }

    if (index < node->key_count && value == node->keys[index]) {
        return true;
    }
    if (node->is_leaf) {
        return false;
    }
    return node_find(node->children[index], value);
}

bool btree_find(const btree *tree, tree_value value) {
    if (tree == NULL || tree->root == NULL) {
        return false;
    }
    return node_find(tree->root, value);
}

static void split_child(const btree *tree, btree_node *parent, size_t child_index) {
    const size_t min_degree = tree->min_degree;
    btree_node *full_child = parent->children[child_index];
    btree_node *new_child = btree_node_create(tree, full_child->is_leaf);

    new_child->key_count = min_degree - 1;
    for (size_t j = 0; j < min_degree - 1; j++) {
        new_child->keys[j] = full_child->keys[j + min_degree];
    }

    if (!full_child->is_leaf) {
        for (size_t j = 0; j < min_degree; j++) {
            new_child->children[j] = full_child->children[j + min_degree];
        }
    }

    full_child->key_count = min_degree - 1;
    for (size_t j = parent->key_count + 1; j > child_index + 1; j--) {
        parent->children[j] = parent->children[j - 1];
    }
    parent->children[child_index + 1] = new_child;

    for (size_t j = parent->key_count; j > child_index; j--) {
        parent->keys[j] = parent->keys[j - 1];
    }
    parent->keys[child_index] = full_child->keys[min_degree - 1];
    parent->key_count++;
}

static void insert_nonfull(const btree *tree, btree_node *node, tree_value value) {
    size_t index = node->key_count;
    if (node->is_leaf) {
        while (index > 0 && value < node->keys[index - 1]) {
            node->keys[index] = node->keys[index - 1];
            index--;
        }
        node->keys[index] = value;
        node->key_count++;
        return;
    }

    while (index > 0 && value < node->keys[index - 1]) {
        index--;
    }

    if (node->children[index]->key_count == 2 * tree->min_degree - 1) {
        split_child(tree, node, index);
        if (value > node->keys[index]) {
            index++;
        }
    }
    insert_nonfull(tree, node->children[index], value);
}

bool btree_insert(btree *tree, tree_value value) {
    if (tree == NULL || btree_find(tree, value)) {
        return false;
    }

    btree_node *root = tree->root;
    if (root->key_count == 2 * tree->min_degree - 1) {
        btree_node *new_root = btree_node_create(tree, false);
        if (new_root == NULL) {
            return false;
        }
        new_root->children[0] = root;
        tree->root = new_root;
        split_child(tree, new_root, 0);
        insert_nonfull(tree, new_root, value);
    } else {
        insert_nonfull(tree, root, value);
    }
    return true;
}

static size_t find_key_index(const btree_node *node, tree_value value) {
    size_t index = 0;
    while (index < node->key_count && node->keys[index] < value) {
        index++;
    }
    return index;
}

static tree_value predecessor_value(btree_node *node) {
    btree_node *current = node;
    while (!current->is_leaf) {
        current = current->children[current->key_count];
    }
    return current->keys[current->key_count - 1];
}

static tree_value successor_value(btree_node *node) {
    btree_node *current = node;
    while (!current->is_leaf) {
        current = current->children[0];
    }
    return current->keys[0];
}

static void remove_from_leaf(btree_node *node, size_t key_index) {
    for (size_t i = key_index + 1; i < node->key_count; i++) {
        node->keys[i - 1] = node->keys[i];
    }
    node->key_count--;
}

static void merge_children(const btree *tree, btree_node *node, size_t key_index) {
    btree_node *child = node->children[key_index];
    btree_node *sibling = node->children[key_index + 1];
    const size_t min_degree = tree->min_degree;

    child->keys[min_degree - 1] = node->keys[key_index];
    for (size_t i = 0; i < sibling->key_count; i++) {
        child->keys[i + min_degree] = sibling->keys[i];
    }

    if (!child->is_leaf) {
        for (size_t i = 0; i <= sibling->key_count; i++) {
            child->children[i + min_degree] = sibling->children[i];
        }
    }

    for (size_t i = key_index + 1; i < node->key_count; i++) {
        node->keys[i - 1] = node->keys[i];
    }
    for (size_t i = key_index + 2; i <= node->key_count; i++) {
        node->children[i - 1] = node->children[i];
    }

    child->key_count += sibling->key_count + 1;
    node->key_count--;

    free(sibling->keys);
    free(sibling->children);
    free(sibling);
}

static void borrow_from_previous(btree_node *node, size_t child_index) {
    btree_node *child = node->children[child_index];
    btree_node *sibling = node->children[child_index - 1];

    for (size_t i = child->key_count; i > 0; i--) {
        child->keys[i] = child->keys[i - 1];
    }
    if (!child->is_leaf) {
        for (size_t i = child->key_count + 1; i > 0; i--) {
            child->children[i] = child->children[i - 1];
        }
    }

    child->keys[0] = node->keys[child_index - 1];
    if (!child->is_leaf) {
        child->children[0] = sibling->children[sibling->key_count];
    }

    node->keys[child_index - 1] = sibling->keys[sibling->key_count - 1];
    child->key_count++;
    sibling->key_count--;
}

static void borrow_from_next(btree_node *node, size_t child_index) {
    btree_node *child = node->children[child_index];
    btree_node *sibling = node->children[child_index + 1];

    child->keys[child->key_count] = node->keys[child_index];
    if (!child->is_leaf) {
        child->children[child->key_count + 1] = sibling->children[0];
    }

    node->keys[child_index] = sibling->keys[0];
    for (size_t i = 1; i < sibling->key_count; i++) {
        sibling->keys[i - 1] = sibling->keys[i];
    }
    if (!sibling->is_leaf) {
        for (size_t i = 1; i <= sibling->key_count; i++) {
            sibling->children[i - 1] = sibling->children[i];
        }
    }

    child->key_count++;
    sibling->key_count--;
}

static void fill_child(const btree *tree, btree_node *node, size_t child_index) {
    if (child_index > 0 && node->children[child_index - 1]->key_count >= tree->min_degree) {
        borrow_from_previous(node, child_index);
    } else if (child_index < node->key_count && node->children[child_index + 1]->key_count >= tree->min_degree) {
        borrow_from_next(node, child_index);
    } else if (child_index < node->key_count) {
        merge_children(tree, node, child_index);
    } else {
        merge_children(tree, node, child_index - 1);
    }
}

static bool remove_from_node(const btree *tree, btree_node *node, tree_value value);

static bool remove_from_internal(const btree *tree, btree_node *node, size_t key_index) {
    tree_value value = node->keys[key_index];
    btree_node *left_child = node->children[key_index];
    btree_node *right_child = node->children[key_index + 1];

    if (left_child->key_count >= tree->min_degree) {
        tree_value predecessor = predecessor_value(left_child);
        node->keys[key_index] = predecessor;
        return remove_from_node(tree, left_child, predecessor);
    }

    if (right_child->key_count >= tree->min_degree) {
        tree_value successor = successor_value(right_child);
        node->keys[key_index] = successor;
        return remove_from_node(tree, right_child, successor);
    }

    merge_children(tree, node, key_index);
    return remove_from_node(tree, left_child, value);
}

static bool remove_from_node(const btree *tree, btree_node *node, tree_value value) {
    size_t key_index = find_key_index(node, value);

    if (key_index < node->key_count && node->keys[key_index] == value) {
        if (node->is_leaf) {
            remove_from_leaf(node, key_index);
            return true;
        }
        return remove_from_internal(tree, node, key_index);
    }

    if (node->is_leaf) {
        return false;
    }

    bool descended_to_last_child = key_index == node->key_count;
    if (node->children[key_index]->key_count < tree->min_degree) {
        fill_child(tree, node, key_index);
    }

    if (descended_to_last_child && key_index > node->key_count) {
        return remove_from_node(tree, node->children[key_index - 1], value);
    }
    return remove_from_node(tree, node->children[key_index], value);
}

bool btree_delete(btree *tree, tree_value value) {
    if (tree == NULL || tree->root == NULL) {
        return false;
    }

    bool deleted = remove_from_node(tree, tree->root, value);
    if (!deleted) {
        return false;
    }

    if (tree->root->key_count == 0 && !tree->root->is_leaf) {
        btree_node *old_root = tree->root;
        tree->root = old_root->children[0];
        free(old_root->keys);
        free(old_root->children);
        free(old_root);
    }
    return true;
}

static void set_validation_message(char *message, size_t message_size, const char *text) {
    if (message != NULL && message_size > 0) {
        snprintf(message, message_size, "%s", text);
    }
}

static bool validate_node(
    const btree *tree,
    const btree_node *node,
    bool is_root,
    long long min,
    long long max,
    int depth,
    int *leaf_depth,
    char *message,
    size_t message_size
) {
    if (node == NULL) {
        set_validation_message(message, message_size, "B-tree child pointer invariant failed");
        return false;
    }

    const size_t max_keys = 2 * tree->min_degree - 1;
    if (node->key_count > max_keys || (!is_root && node->key_count < tree->min_degree - 1)) {
        set_validation_message(message, message_size, "B-tree key-count invariant failed");
        return false;
    }

    for (size_t i = 0; i < node->key_count; i++) {
        if ((long long)node->keys[i] <= min || (long long)node->keys[i] >= max) {
            set_validation_message(message, message_size, "B-tree ordering invariant failed");
            return false;
        }
        if (i > 0 && node->keys[i - 1] >= node->keys[i]) {
            set_validation_message(message, message_size, "B-tree sorted-key invariant failed");
            return false;
        }
    }

    if (node->is_leaf) {
        if (*leaf_depth == -1) {
            *leaf_depth = depth;
        } else if (*leaf_depth != depth) {
            set_validation_message(message, message_size, "B-tree leaf-depth invariant failed");
            return false;
        }
        return true;
    }

    for (size_t i = 0; i <= node->key_count; i++) {
        long long child_min = i == 0 ? min : node->keys[i - 1];
        long long child_max = i == node->key_count ? max : node->keys[i];
        if (!validate_node(tree, node->children[i], false, child_min, child_max, depth + 1, leaf_depth, message, message_size)) {
            return false;
        }
    }

    return true;
}

bool btree_validate(const btree *tree, char *message, size_t message_size) {
    if (tree == NULL || tree->min_degree < 2 || tree->root == NULL) {
        set_validation_message(message, message_size, "B-tree object invariant failed");
        return false;
    }

    int leaf_depth = -1;
    bool valid = validate_node(
        tree,
        tree->root,
        true,
        (long long)INT_MIN - 1LL,
        (long long)INT_MAX + 1LL,
        0,
        &leaf_depth,
        message,
        message_size
    );
    if (valid) {
        set_validation_message(message, message_size, "ok");
    }
    return valid;
}

static void print_keys(FILE *out, const btree_node *node) {
    fputc('[', out);
    for (size_t i = 0; i < node->key_count; i++) {
        if (i > 0) {
            fputc(' ', out);
        }
        fprintf(out, "%d", node->keys[i]);
    }
    fputc(']', out);
}

static void print_btree_node(FILE *out, const btree_node *node, int depth) {
    if (node == NULL) {
        return;
    }

    if (!node->is_leaf) {
        for (size_t i = node->key_count + 1; i > 0; i--) {
            print_btree_node(out, node->children[i - 1], depth + 1);
        }
    }

    for (int i = 0; i < depth * 2; i++) {
        fputc(' ', out);
    }
    fprintf(out, "+-");
    print_keys(out, node);
    fputc('\n', out);
}

void btree_print(FILE *out, const btree *tree) {
    if (tree == NULL || tree->root == NULL || tree->root->key_count == 0) {
        fprintf(out, "(empty tree)\n");
        return;
    }
    print_btree_node(out, tree->root, 0);
}

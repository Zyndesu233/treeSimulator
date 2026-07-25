#include "core/forest.h"

#include <stdlib.h>

void forest_init(forest *forest_ptr) {
    forest_ptr->items = NULL;
    forest_ptr->count = 0;
    forest_ptr->capacity = 0;
}

void forest_free(forest *forest_ptr) {
    if (forest_ptr == NULL) {
        return;
    }
    for (size_t i = 0; i < forest_ptr->count; i++) {
        if (forest_ptr->items[i].type == TREE_TYPE_BTREE) {
            btree_free(forest_ptr->items[i].btree_root);
        } else if (forest_ptr->items[i].root != NULL) {
            forest_ptr->items[i].methods->free(forest_ptr->items[i].root);
        }
    }
    free(forest_ptr->items);
    forest_init(forest_ptr);
}

static bool forest_reserve(forest *forest_ptr, size_t needed) {
    if (forest_ptr->capacity >= needed) {
        return true;
    }

    size_t new_capacity = forest_ptr->capacity == 0 ? 4 : forest_ptr->capacity * 2;
    while (new_capacity < needed) {
        new_capacity *= 2;
    }

    tree_handle *new_items = realloc(forest_ptr->items, new_capacity * sizeof(*new_items));
    if (new_items == NULL) {
        return false;
    }

    forest_ptr->items = new_items;
    forest_ptr->capacity = new_capacity;
    return true;
}

bool forest_create_tree(forest *forest_ptr, tree_type type, size_t *index) {
    if (type == TREE_TYPE_BTREE) {
        return false;
    }

    const binary_tree_vtable *methods = binary_tree_vtable_for_type(type);
    if (methods == NULL) {
        return false;
    }

    if (!forest_reserve(forest_ptr, forest_ptr->count + 1)) {
        return false;
    }

    size_t next = forest_ptr->count;
    forest_ptr->items[next].type = type;
    forest_ptr->items[next].methods = methods;
    forest_ptr->items[next].root = NULL;
    forest_ptr->items[next].btree_root = NULL;
    forest_ptr->items[next].btree_min_degree = 0;
    forest_ptr->count++;

    if (index != NULL) {
        *index = next;
    }
    return true;
}

bool forest_create_btree(forest *forest_ptr, size_t min_degree, size_t *index) {
    btree *tree = btree_create(min_degree);
    if (tree == NULL) {
        return false;
    }

    if (!forest_reserve(forest_ptr, forest_ptr->count + 1)) {
        btree_free(tree);
        return false;
    }

    size_t next = forest_ptr->count;
    forest_ptr->items[next].type = TREE_TYPE_BTREE;
    forest_ptr->items[next].methods = NULL;
    forest_ptr->items[next].root = NULL;
    forest_ptr->items[next].btree_root = tree;
    forest_ptr->items[next].btree_min_degree = min_degree;
    forest_ptr->count++;

    if (index != NULL) {
        *index = next;
    }
    return true;
}

tree_handle *forest_get(forest *forest_ptr, size_t index) {
    if (forest_ptr == NULL || index >= forest_ptr->count) {
        return NULL;
    }
    return &forest_ptr->items[index];
}

const tree_handle *forest_get_const(const forest *forest_ptr, size_t index) {
    if (forest_ptr == NULL || index >= forest_ptr->count) {
        return NULL;
    }
    return &forest_ptr->items[index];
}

bool forest_insert(tree_handle *tree, tree_value value) {
    if (tree != NULL && tree->type == TREE_TYPE_BTREE) {
        return btree_insert(tree->btree_root, value);
    }
    if (tree == NULL || tree->methods == NULL) {
        return false;
    }
    bool inserted = false;
    tree->root = tree->methods->insert(tree->root, value, &inserted);
    return inserted;
}

bool forest_delete(tree_handle *tree, tree_value value) {
    if (tree != NULL && tree->type == TREE_TYPE_BTREE) {
        return btree_delete(tree->btree_root, value);
    }
    if (tree == NULL || tree->methods == NULL) {
        return false;
    }
    bool deleted = false;
    tree->root = tree->methods->delete(tree->root, value, &deleted);
    return deleted;
}

bool forest_find(tree_handle *tree, tree_value value) {
    if (tree != NULL && tree->type == TREE_TYPE_BTREE) {
        return btree_find(tree->btree_root, value);
    }
    if (tree == NULL || tree->methods == NULL) {
        return false;
    }
    bool found = false;
    tree->root = tree->methods->find(tree->root, value, &found);
    return found;
}

bool forest_validate(const tree_handle *tree, char *message, size_t message_size) {
    if (tree != NULL && tree->type == TREE_TYPE_BTREE) {
        return btree_validate(tree->btree_root, message, message_size);
    }
    if (tree == NULL || tree->methods == NULL || tree->methods->validate == NULL) {
        return false;
    }
    return tree->methods->validate(tree->root, message, message_size);
}

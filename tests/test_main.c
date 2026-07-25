#include <stdio.h>
#include <string.h>

#include "core/forest.h"
#include "output/tree_printer.h"
#include "repl/input.h"

static int failures = 0;

#define EXPECT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "FAIL %s:%d: expected true: %s\n", __FILE__, __LINE__, #condition); \
            failures++; \
        } \
    } while (0)

#define EXPECT_FALSE(condition) \
    do { \
        if (condition) { \
            fprintf(stderr, "FAIL %s:%d: expected false: %s\n", __FILE__, __LINE__, #condition); \
            failures++; \
        } \
    } while (0)

#define EXPECT_INT_EQ(expected, actual) \
    do { \
        int expected_value = (expected); \
        int actual_value = (actual); \
        if (expected_value != actual_value) { \
            fprintf(stderr, "FAIL %s:%d: expected %d, got %d\n", __FILE__, __LINE__, expected_value, actual_value); \
            failures++; \
        } \
    } while (0)

#define EXPECT_STRING_EQ(expected, actual) \
    do { \
        const char *expected_value = (expected); \
        const char *actual_value = (actual); \
        if (strcmp(expected_value, actual_value) != 0) { \
            fprintf(stderr, "FAIL %s:%d: expected \"%s\", got \"%s\"\n", __FILE__, __LINE__, expected_value, actual_value); \
            failures++; \
        } \
    } while (0)

static void test_input_utilities(void) {
    char line[] = "insert 0 -42 # ignored";
    char *tokens[INPUT_MAX_TOKENS];
    size_t count = input_tokenize(line, tokens, INPUT_MAX_TOKENS);
    int value = 0;
    size_t index = 0;

    EXPECT_INT_EQ(3, (int)count);
    EXPECT_STRING_EQ("insert", tokens[0]);
    EXPECT_TRUE(input_parse_size(tokens[1], &index));
    EXPECT_INT_EQ(0, (int)index);
    EXPECT_TRUE(input_parse_int(tokens[2], &value));
    EXPECT_INT_EQ(-42, value);
    EXPECT_FALSE(input_parse_size("-1", &index));
    EXPECT_FALSE(input_parse_int("12x", &value));
    EXPECT_INT_EQ(REPL_COMMAND_INSERT, input_parse_command("i"));
    EXPECT_INT_EQ(REPL_COMMAND_INSERT_MANY, input_parse_command("im"));
    EXPECT_INT_EQ(REPL_COMMAND_FIND, input_parse_command("f"));
    EXPECT_INT_EQ(TREE_TYPE_AVL, input_parse_tree_type("avl"));
    EXPECT_INT_EQ(TREE_TYPE_SPLAY, input_parse_tree_type("splay"));
    EXPECT_INT_EQ(TREE_TYPE_RED_BLACK, input_parse_tree_type("rbt"));
    EXPECT_INT_EQ(TREE_TYPE_BTREE, input_parse_tree_type("btree"));
}

static void test_bst_operations(void) {
    forest trees;
    forest_init(&trees);

    size_t index = 0;
    EXPECT_TRUE(forest_create_tree(&trees, TREE_TYPE_BST, &index));
    tree_handle *tree = forest_get(&trees, index);
    EXPECT_TRUE(forest_insert(tree, 5));
    EXPECT_TRUE(forest_insert(tree, 3));
    EXPECT_TRUE(forest_insert(tree, 7));
    EXPECT_FALSE(forest_insert(tree, 3));
    EXPECT_TRUE(forest_find(tree, 3));
    EXPECT_TRUE(forest_find(tree, 7));

    char message[128];
    EXPECT_TRUE(forest_validate(tree, message, sizeof(message)));
    EXPECT_TRUE(forest_delete(tree, 5));
    EXPECT_FALSE(forest_find(tree, 5));
    EXPECT_TRUE(forest_validate(tree, message, sizeof(message)));
    EXPECT_FALSE(forest_delete(tree, 100));

    forest_free(&trees);
}

static void test_red_black_tree_operations(void) {
    forest trees;
    forest_init(&trees);

    size_t index = 0;
    EXPECT_TRUE(forest_create_tree(&trees, TREE_TYPE_RED_BLACK, &index));
    tree_handle *tree = forest_get(&trees, index);
    char message[128];
    const int values[] = {10, 5, 15, 1, 7, 12, 18, 6, 8, 11, 13};

    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        EXPECT_TRUE(forest_insert(tree, values[i]));
        EXPECT_TRUE(forest_validate(tree, message, sizeof(message)));
    }

    EXPECT_FALSE(forest_insert(tree, 7));
    EXPECT_TRUE(forest_find(tree, 12));
    EXPECT_FALSE(forest_find(tree, 99));

    const int deletions[] = {1, 15, 10, 7, 18};
    for (size_t i = 0; i < sizeof(deletions) / sizeof(deletions[0]); i++) {
        EXPECT_TRUE(forest_delete(tree, deletions[i]));
        EXPECT_TRUE(forest_validate(tree, message, sizeof(message)));
        EXPECT_FALSE(forest_find(tree, deletions[i]));
    }

    forest_free(&trees);
}

static void test_avl_tree_operations(void) {
    forest trees;
    forest_init(&trees);

    size_t index = 0;
    EXPECT_TRUE(forest_create_tree(&trees, TREE_TYPE_AVL, &index));
    tree_handle *tree = forest_get(&trees, index);
    char message[128];
    const int values[] = {1, 2, 3, 4, 5, 6, 7};

    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        EXPECT_TRUE(forest_insert(tree, values[i]));
        EXPECT_TRUE(forest_validate(tree, message, sizeof(message)));
    }

    EXPECT_TRUE(forest_find(tree, 4));
    EXPECT_FALSE(forest_insert(tree, 4));
    EXPECT_TRUE(forest_delete(tree, 4));
    EXPECT_TRUE(forest_validate(tree, message, sizeof(message)));
    EXPECT_TRUE(forest_delete(tree, 1));
    EXPECT_TRUE(forest_validate(tree, message, sizeof(message)));
    EXPECT_FALSE(forest_find(tree, 4));

    forest_free(&trees);
}

static void test_splay_tree_operations(void) {
    forest trees;
    forest_init(&trees);

    size_t index = 0;
    EXPECT_TRUE(forest_create_tree(&trees, TREE_TYPE_SPLAY, &index));
    tree_handle *tree = forest_get(&trees, index);
    char message[128];
    const int values[] = {10, 5, 15, 1, 7};

    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        EXPECT_TRUE(forest_insert(tree, values[i]));
        EXPECT_TRUE(forest_validate(tree, message, sizeof(message)));
    }

    EXPECT_TRUE(forest_find(tree, 1));
    EXPECT_TRUE(tree->root != NULL);
    EXPECT_INT_EQ(1, tree->root->value);
    EXPECT_TRUE(forest_validate(tree, message, sizeof(message)));

    EXPECT_FALSE(forest_find(tree, 99));
    EXPECT_TRUE(forest_validate(tree, message, sizeof(message)));
    EXPECT_TRUE(forest_delete(tree, 5));
    EXPECT_TRUE(forest_validate(tree, message, sizeof(message)));
    EXPECT_FALSE(forest_find(tree, 5));

    forest_free(&trees);
}

static void test_btree_operations(void) {
    forest trees;
    forest_init(&trees);

    size_t index = 0;
    EXPECT_TRUE(forest_create_btree(&trees, 2, &index));
    tree_handle *tree = forest_get(&trees, index);
    char message[128];

    for (int value = 1; value <= 20; value++) {
        EXPECT_TRUE(forest_insert(tree, value));
        EXPECT_TRUE(forest_validate(tree, message, sizeof(message)));
    }

    EXPECT_FALSE(forest_insert(tree, 10));
    EXPECT_TRUE(forest_find(tree, 1));
    EXPECT_TRUE(forest_find(tree, 11));
    EXPECT_TRUE(forest_find(tree, 20));
    EXPECT_FALSE(forest_find(tree, 99));

    const int deletions[] = {1, 10, 20, 7, 13, 2, 3, 4};
    for (size_t i = 0; i < sizeof(deletions) / sizeof(deletions[0]); i++) {
        EXPECT_TRUE(forest_delete(tree, deletions[i]));
        EXPECT_TRUE(forest_validate(tree, message, sizeof(message)));
        EXPECT_FALSE(forest_find(tree, deletions[i]));
    }

    forest_free(&trees);
}

static void test_tree_printer(void) {
    forest trees;
    forest_init(&trees);

    size_t index = 0;
    EXPECT_TRUE(forest_create_tree(&trees, TREE_TYPE_BST, &index));
    tree_handle *tree = forest_get(&trees, index);
    EXPECT_TRUE(forest_insert(tree, 1));
    EXPECT_TRUE(forest_insert(tree, 2));
    EXPECT_TRUE(forest_insert(tree, 3));

    FILE *out = tmpfile();
    EXPECT_TRUE(out != NULL);
    if (out != NULL) {
        char line[128];
        tree_printer_print(out, tree->root);
        rewind(out);
        EXPECT_TRUE(fgets(line, sizeof(line), out) != NULL);
        EXPECT_STRING_EQ("    +-3\n", line);
        EXPECT_TRUE(fgets(line, sizeof(line), out) != NULL);
        EXPECT_STRING_EQ("  +-2\n", line);
        EXPECT_TRUE(fgets(line, sizeof(line), out) != NULL);
        EXPECT_STRING_EQ("+-1\n", line);
        fclose(out);
    }

    forest_free(&trees);
}

int main(void) {
    test_input_utilities();
    test_bst_operations();
    test_red_black_tree_operations();
    test_avl_tree_operations();
    test_splay_tree_operations();
    test_btree_operations();
    test_tree_printer();

    if (failures == 0) {
        printf("All unit tests passed.\n");
        return 0;
    }

    fprintf(stderr, "%d test failure(s).\n", failures);
    return 1;
}

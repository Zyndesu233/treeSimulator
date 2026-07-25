#include "output/tree_printer.h"

#include <stdio.h>

static char prefix_buffer[1024];

static void format_node(const binary_tree *node, char *buffer, size_t buffer_size) {
    if (node->methods != NULL && node->methods->format_node != NULL) {
        node->methods->format_node(node, buffer, buffer_size);
        return;
    }
    binary_tree_default_format_node(node, buffer, buffer_size);
}

/* Adapted from https://stackoverflow.com/a/64688332 by Luis Colorado.
 * Retrieved 2026-07-25. Original answer licensed CC BY-SA 4.0.
 */
static void print_node(FILE *out, const binary_tree *node) {
    char label[64];
    format_node(node, label, sizeof(label));
    fprintf(out, "%s+-%s\n", prefix_buffer, label);
}

static void print_subtree(
    FILE *out,
    const binary_tree *node,
    const char *right_prefix,
    const char *left_prefix,
    char *buffer,
    int buffer_size
) {
    if (node->right_child != NULL) {
        int written = snprintf(buffer, (size_t)buffer_size, "%s", right_prefix);
        if (written < 0 || written >= buffer_size) {
            return;
        }
        print_subtree(out, node->right_child, "  ", "| ", buffer + written, buffer_size - written);
        *buffer = '\0';
    }

    print_node(out, node);

    if (node->left_child != NULL) {
        int written = snprintf(buffer, (size_t)buffer_size, "%s", left_prefix);
        if (written < 0 || written >= buffer_size) {
            return;
        }
        print_subtree(out, node->left_child, "| ", "  ", buffer + written, buffer_size - written);
        *buffer = '\0';
    }
}

void tree_printer_print(FILE *out, const binary_tree *root) {
    if (root == NULL) {
        fprintf(out, "(empty tree)\n");
        return;
    }

    prefix_buffer[0] = '\0';
    print_subtree(out, root, "  ", "  ", prefix_buffer, (int)sizeof(prefix_buffer));
}

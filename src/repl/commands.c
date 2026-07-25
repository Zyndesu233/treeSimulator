#include "repl/commands.h"

#include <stdbool.h>
#include <stdio.h>

#include "common/log.h"
#include "output/tree_printer.h"
#include "repl/input.h"

#define HELP_PAGE_DIR "help"

static bool dump_help_page(FILE *out, const char *page_name) {
    char path[128];
    snprintf(path, sizeof(path), "%s/%s.txt", HELP_PAGE_DIR, page_name);

    FILE *page = fopen(path, "r");
    if (page == NULL) {
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), page) != NULL) {
        fputs(line, out);
    }
    fclose(page);
    return true;
}

static bool require_argument_count(command_context *context, size_t actual, size_t expected, const char *usage) {
    if (actual == expected) {
        return true;
    }
    log_error(context->out, "Invalid input. Usage: %s", usage);
    return false;
}

static bool parse_tree_index(command_context *context, const char *text, size_t *index, tree_handle **tree) {
    if (!input_parse_size(text, index)) {
        log_error(context->out, "Invalid tree index: %s", text == NULL ? "(missing)" : text);
        return false;
    }

    *tree = forest_get(context->trees, *index);
    if (*tree == NULL) {
        log_error(context->out, "Tree with index %zu does not exist.", *index);
        return false;
    }
    return true;
}

static bool parse_value(command_context *context, const char *text, int *value) {
    if (!input_parse_int(text, value)) {
        log_error(context->out, "Invalid node value: %s", text == NULL ? "(missing)" : text);
        return false;
    }
    return true;
}

static void validate_after_mutation(command_context *context, const tree_handle *tree) {
    char message[128];
    if (!forest_validate(tree, message, sizeof(message))) {
        log_error(context->out, "Tree invariant failure: %s", message);
    }
}

static command_result show_help(command_context *context, size_t argc, char *argv[]) {
    if (argc > 2) {
        log_error(context->out, "Invalid input. Usage: help [command]");
        return COMMAND_RESULT_CONTINUE;
    }

    if (argc == 1) {
        command_print_help(context->out);
        return COMMAND_RESULT_CONTINUE;
    }

    repl_command command = input_parse_command(argv[1]);
    if (command == REPL_COMMAND_UNKNOWN || command == REPL_COMMAND_EMPTY) {
        log_error(context->out, "No help page found for command: %s", argv[1]);
        return COMMAND_RESULT_CONTINUE;
    }

    const char *page_name = input_command_name(command);
    if (!dump_help_page(context->out, page_name)) {
        log_error(context->out, "No help page found for command: %s", argv[1]);
    }
    return COMMAND_RESULT_CONTINUE;
}

static command_result create_tree(command_context *context, size_t argc, char *argv[]) {
    if (argc < 2) {
        log_error(context->out, "Invalid input. Usage: new <bst|avl|splay|rbt> or new btree <min_degree>");
        return COMMAND_RESULT_CONTINUE;
    }

    tree_type type = input_parse_tree_type(argv[1]);
    if (type == TREE_TYPE_UNKNOWN) {
        log_error(context->out, "Unknown tree type: %s", argv[1]);
        return COMMAND_RESULT_CONTINUE;
    }

    size_t index = 0;
    if (type == TREE_TYPE_BTREE) {
        if (!require_argument_count(context, argc, 3, "new btree <min_degree>")) {
            return COMMAND_RESULT_CONTINUE;
        }

        size_t min_degree = 0;
        if (!input_parse_size(argv[2], &min_degree) || min_degree < 2) {
            log_error(context->out, "Invalid B-tree minimum degree: %s. Use an integer >= 2.", argv[2]);
            return COMMAND_RESULT_CONTINUE;
        }

        if (!forest_create_btree(context->trees, min_degree, &index)) {
            log_error(context->out, "Could not create B-tree with minimum degree %zu.", min_degree);
            return COMMAND_RESULT_CONTINUE;
        }

        log_info(context->out, "A new B-tree with minimum degree %zu and index %zu is created.", min_degree, index);
        return COMMAND_RESULT_CONTINUE;
    }

    if (!require_argument_count(context, argc, 2, "new <bst|avl|splay|rbt>")) {
        return COMMAND_RESULT_CONTINUE;
    }

    if (!forest_create_tree(context->trees, type, &index)) {
        log_error(context->out, "Could not create %s.", binary_tree_type_name(type));
        return COMMAND_RESULT_CONTINUE;
    }

    log_info(context->out, "A new %s with index %zu is created.", binary_tree_type_name(type), index);
    return COMMAND_RESULT_CONTINUE;
}

static command_result insert_node(command_context *context, size_t argc, char *argv[]) {
    if (!require_argument_count(context, argc, 3, "insert <tree_index> <value>")) {
        return COMMAND_RESULT_CONTINUE;
    }

    size_t index = 0;
    int value = 0;
    tree_handle *tree = NULL;
    if (!parse_tree_index(context, argv[1], &index, &tree) || !parse_value(context, argv[2], &value)) {
        return COMMAND_RESULT_CONTINUE;
    }

    if (!forest_insert(tree, value)) {
        if (forest_find(tree, value)) {
            log_warning(context->out, "Node %d already exists in tree %zu.", value, index);
        } else {
            log_error(context->out, "Could not insert node %d in tree %zu.", value, index);
        }
        return COMMAND_RESULT_CONTINUE;
    }

    validate_after_mutation(context, tree);
    log_info(context->out, "Node %d inserted in tree %zu.", value, index);
    return COMMAND_RESULT_CONTINUE;
}

static command_result insert_many_nodes(command_context *context, size_t argc, char *argv[]) {
    if (argc < 3) {
        log_error(context->out, "Invalid input. Usage: im <tree_index> <number_of_nodes> <values...>");
        return COMMAND_RESULT_CONTINUE;
    }

    size_t index = 0;
    size_t node_count = 0;
    tree_handle *tree = NULL;
    if (!parse_tree_index(context, argv[1], &index, &tree)) {
        return COMMAND_RESULT_CONTINUE;
    }
    if (!input_parse_size(argv[2], &node_count)) {
        log_error(context->out, "Invalid number of nodes: %s", argv[2]);
        return COMMAND_RESULT_CONTINUE;
    }
    if (node_count > INPUT_MAX_TOKENS - 3) {
        log_error(context->out, "Too many nodes. This build accepts at most %d values per command.", INPUT_MAX_TOKENS - 3);
        return COMMAND_RESULT_CONTINUE;
    }
    if (argc != node_count + 3) {
        log_error(
            context->out,
            "Invalid input. Expected %zu value(s), got %zu.",
            node_count,
            argc > 3 ? argc - 3 : 0
        );
        return COMMAND_RESULT_CONTINUE;
    }

    int values[INPUT_MAX_TOKENS];
    for (size_t i = 0; i < node_count; i++) {
        if (!parse_value(context, argv[i + 3], &values[i])) {
            return COMMAND_RESULT_CONTINUE;
        }
    }

    size_t inserted_count = 0;
    for (size_t i = 0; i < node_count; i++) {
        if (forest_insert(tree, values[i])) {
            inserted_count++;
        }
    }

    validate_after_mutation(context, tree);
    log_info(context->out, "Inserted %zu of %zu node(s) in tree %zu.", inserted_count, node_count, index);
    return COMMAND_RESULT_CONTINUE;
}

static command_result delete_node(command_context *context, size_t argc, char *argv[]) {
    if (!require_argument_count(context, argc, 3, "delete <tree_index> <value>")) {
        return COMMAND_RESULT_CONTINUE;
    }

    size_t index = 0;
    int value = 0;
    tree_handle *tree = NULL;
    if (!parse_tree_index(context, argv[1], &index, &tree) || !parse_value(context, argv[2], &value)) {
        return COMMAND_RESULT_CONTINUE;
    }

    if (!forest_delete(tree, value)) {
        log_error(context->out, "Node %d does not exist in tree %zu.", value, index);
        return COMMAND_RESULT_CONTINUE;
    }

    validate_after_mutation(context, tree);
    log_info(context->out, "Node %d deleted in tree %zu.", value, index);
    return COMMAND_RESULT_CONTINUE;
}

static command_result find_node(command_context *context, size_t argc, char *argv[]) {
    if (!require_argument_count(context, argc, 3, "find <tree_index> <value>")) {
        return COMMAND_RESULT_CONTINUE;
    }

    size_t index = 0;
    int value = 0;
    tree_handle *tree = NULL;
    if (!parse_tree_index(context, argv[1], &index, &tree) || !parse_value(context, argv[2], &value)) {
        return COMMAND_RESULT_CONTINUE;
    }

    if (forest_find(tree, value)) {
        log_info(context->out, "Node %d exists in tree %zu.", value, index);
    } else {
        log_info(context->out, "Node %d does not exist in tree %zu.", value, index);
    }
    return COMMAND_RESULT_CONTINUE;
}

static command_result print_tree(command_context *context, size_t argc, char *argv[]) {
    if (!require_argument_count(context, argc, 2, "print <tree_index>")) {
        return COMMAND_RESULT_CONTINUE;
    }

    size_t index = 0;
    tree_handle *tree = NULL;
    if (!parse_tree_index(context, argv[1], &index, &tree)) {
        return COMMAND_RESULT_CONTINUE;
    }

    if (tree->type == TREE_TYPE_BTREE) {
        log_info(context->out, "Printing tree %zu (B-tree, minimum degree %zu).", index, tree->btree_min_degree);
        btree_print(context->out, tree->btree_root);
    } else {
        log_info(context->out, "Printing tree %zu (%s).", index, tree->methods->name);
        tree_printer_print(context->out, tree->root);
    }
    return COMMAND_RESULT_CONTINUE;
}

static command_result dump_forest(command_context *context, size_t argc, char *argv[]) {
    (void)argv;
    if (!require_argument_count(context, argc, 1, "dump")) {
        return COMMAND_RESULT_CONTINUE;
    }

    if (context->trees->count == 0) {
        log_info(context->out, "Forest is empty.");
        return COMMAND_RESULT_CONTINUE;
    }

    log_info(context->out, "Forest contains %zu tree(s).", context->trees->count);
    for (size_t i = 0; i < context->trees->count; i++) {
        const tree_handle *tree = forest_get_const(context->trees, i);
        if (tree->type == TREE_TYPE_BTREE) {
            fprintf(context->out, "Tree %zu: B-tree, minimum degree %zu\n", i, tree->btree_min_degree);
        } else if (tree->root == NULL) {
            fprintf(context->out, "Tree %zu: empty %s\n", i, tree->methods->name);
        } else {
            char label[64];
            tree->methods->format_node(tree->root, label, sizeof(label));
            fprintf(context->out, "Tree %zu: %s, root %s\n", i, tree->methods->name, label);
        }
    }
    return COMMAND_RESULT_CONTINUE;
}

void command_print_help(FILE *out) {
    if (!dump_help_page(out, "index")) {
        fprintf(out, "Commands:\n");
        fprintf(out, "  new|n <bst|avl|splay|rbt>      create a tree\n");
        fprintf(out, "  new|n btree <min_degree>       create a B-tree\n");
        fprintf(out, "  insert|i <index> <value>       insert a node\n");
        fprintf(out, "  insert-many|im <index> <count> <values...>\n");
        fprintf(out, "  delete|d <index> <value>       delete a node\n");
        fprintf(out, "  find|f|search|s <index> <value> find a node\n");
        fprintf(out, "  print|p <index>                print a tree\n");
        fprintf(out, "  dump                           list all trees\n");
        fprintf(out, "  help|h [command]               show help\n");
        fprintf(out, "  quit|q|exit                    leave the simulator\n");
    }
}

command_result command_execute_line(command_context *context, char *line) {
    char *tokens[INPUT_MAX_TOKENS];
    size_t token_count = input_tokenize(line, tokens, INPUT_MAX_TOKENS);
    if (token_count == 0) {
        return COMMAND_RESULT_CONTINUE;
    }

    repl_command command = input_parse_command(tokens[0]);
    command_result result = COMMAND_RESULT_CONTINUE;
    switch (command) {
        case REPL_COMMAND_HELP:
            result = show_help(context, token_count, tokens);
            break;
        case REPL_COMMAND_NEW:
            result = create_tree(context, token_count, tokens);
            break;
        case REPL_COMMAND_INSERT:
            result = insert_node(context, token_count, tokens);
            break;
        case REPL_COMMAND_INSERT_MANY:
            result = insert_many_nodes(context, token_count, tokens);
            break;
        case REPL_COMMAND_DELETE:
            result = delete_node(context, token_count, tokens);
            break;
        case REPL_COMMAND_FIND:
            result = find_node(context, token_count, tokens);
            break;
        case REPL_COMMAND_PRINT:
            result = print_tree(context, token_count, tokens);
            break;
        case REPL_COMMAND_DUMP:
            result = dump_forest(context, token_count, tokens);
            break;
        case REPL_COMMAND_QUIT:
            log_info(context->out, "Exit.");
            result = COMMAND_RESULT_QUIT;
            break;
        case REPL_COMMAND_EMPTY:
            result = COMMAND_RESULT_CONTINUE;
            break;
        default:
            log_error(context->out, "Unknown command: %s", tokens[0]);
            fprintf(context->out, "Use help to display the command list.\n");
            result = COMMAND_RESULT_CONTINUE;
            break;
    }
    fputc('\n', context->out);
    return result;
}

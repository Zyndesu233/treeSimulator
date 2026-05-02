#include "Commands.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "ADTs/AVLTree.h"
#include "ADTs/BinarySearchTree.h"
#include "ADTs/RedBlackTree.h"
#include "ADTs/SplayTree.h"
#include "Util.h"

void runHelp(char* manualPage) {
    if (manualPage == NULL) {
        printf("[INFO] Printing command list:\n");
        for (int i = 0; i < COMMAND_NUMBER; i++) {
            printf("%2d. ", i + 1);
            printCommandFormat((Command)i);
        }
        printf("\n");
        printf("Use \"help <command>\" command to get more detail.\n");
        return;
    }

    printWarning("Some of the manuals is not completed yet.\n");
    char filePath[] = "Manuals/";
    strcat(filePath, command2string(string2command(manualPage)));
    strcat(filePath, ".txt");
    FILE* manul = fopen(filePath, "r");
    if (!manul) {
        printError("Manual not found.");
        return;
    }
    char buffer[1000];
    while (fgets(buffer, 1000, manul)) {
        printf("%s", buffer);
    }
    fclose(manul);
    printf("\n");
}

static void AVLInOrderTraversal(AVLTreeADT t) {
    if (t == NULL) return;
    AVLInOrderTraversal(t->lst);
    printf("%d ", GetNodeValue(t->rt));
    AVLInOrderTraversal(t->rst);
}

static void AVLPreOrderTraversal(AVLTreeADT t) {
    if (t == NULL) return;
    printf("%d ", GetNodeValue(t->rt));
    AVLPreOrderTraversal(t->lst);
    AVLPreOrderTraversal(t->rst);
}

static void AVLPostOrderTraversal(AVLTreeADT t) {
    if (t == NULL) return;
    AVLPostOrderTraversal(t->lst);
    AVLPostOrderTraversal(t->rst);
    printf("%d ", GetNodeValue(t->rt));
}

void runTraversal(ForestPtr forest) {
    int index;
    char* buf;
    if (!readAndParseSubcommandToInt(&index, TRAVERSAL)) return;
    if (!readSubcommand(&buf, TRAVERSAL)) return;
    printf("[INFO] Traversing Tree(%d) in ", index);
    switch (hashSubcommand(buf)) {
        case IN_ORDER:
            printf("in-order.\n");
            switch (forest->items[index]->type) {
                case AVL:
                    AVLInOrderTraversal(forest->items[index]->root);
                    break;
                case BST:
                    todo("BST traversal");
                    break;
                case SPL:
                    todo("SPL traversal");
                    break;
                default:
                    assert(false && "UNREACHABLE");
                    break;
            }
            break;
        case PRE_ORDER:
            printf("pre-order.\n");
            switch (forest->items[index]->type) {
                case AVL:
                    AVLPreOrderTraversal(forest->items[index]->root);
                    break;
                case BST:
                    todo("BST traversal");
                    break;
                case SPL:
                    todo("SPL traversal");
                    break;
                default:
                    assert(false && "UNREACHABLE");
                    break;
            }
            break;
        case POST_ORDER:
            printf("post-order.\n");
            switch (forest->items[index]->type) {
                case AVL:
                    AVLPostOrderTraversal(forest->items[index]->root);
                    break;
                case BST:
                    todo("BST traversal");
                    break;
                case SPL:
                    todo("SPL traversal");
                    break;
                default:
                    assert(false && "UNREACHABLE");
                    break;
            }
            break;
        default:
            assert(false && "UNREACHABLE");
            break;
    }
    printf("\n");
}

void runInsert(ForestPtr forestPtr, size_t index, int val) {
    assert(TREE_TYPE_NUMBER == 4 && "HAVE NOT EXHAUST ALL TREE TYPES");
    switch (forestPtr->items[index]->type) {
        case AVL:
            forestPtr->items[index]->root = AVLInsertNode(NewTreeNode(val), forestPtr->items[index]->root);
            break;
        case BST:
            forestPtr->items[index]->root = InsertNode(forestPtr->items[index]->root, NewTreeNode(val));
            break;
        case SPL:
            forestPtr->items[index]->root = Splay_Insert(forestPtr->items[index]->root, val);
            break;
        case RBT:
            forestPtr->items[index]->root = Red_Black_Insert(forestPtr->items[index]->root, val);
            break;
        default:
            assert(false && "UNREACHABLE");
            break;
    }
    printf("[INFO] Node %d is inserted in tree %zu.\n", val, index);
}

void runInsertMany(ForestPtr forestPtr, size_t index, int count, int vals[]) {
    assert(TREE_TYPE_NUMBER == 4);
    switch (forestPtr->items[index]->type) {
        case AVL:
            for (int i = 0; i < count; i++) {
                forestPtr->items[index]->root = AVLInsertNode(NewTreeNode(vals[i]), forestPtr->items[index]->root);
            }
            break;
        case BST:
            for (int i = 0; i < count; i++) {
                forestPtr->items[index]->root = InsertNode(forestPtr->items[index]->root, NewTreeNode(vals[i]));
            }
            break;
        case SPL:
            for (int i = 0; i < count; i++) {
                forestPtr->items[index]->root = Splay_Insert(forestPtr->items[index]->root, vals[i]);
            }
            break;
        case RBT:
            for (int i = 0; i < count; i++) {
                forestPtr->items[index]->root = Red_Black_Insert(forestPtr->items[index]->root, vals[i]);
            }
            break;
        default:
            assert(false && "UNREACHABLE");
            break;
    }
    printf("[INFO] %d node(s) is/are inserted in tree %zu.\n", count, index);
}

char buffer[1024] = {0};

static void AVL_print_node(AVLTreeADT t, FILE* o) {
    fprintf(o, "%s+-%d\n", buffer, t->rt->value);
}

static void AVL_print_subtree(AVLTreeADT t,
                              FILE* o,
                              const char* prf_right,
                              const char* prf_left,
                              char* buf, int buf_sz) {
    if (t->rst) {
        int res = snprintf(buf, buf_sz, "%s", prf_right);
        AVL_print_subtree(t->rst, o, "  ", "| ", buf + res, buf_sz - res);
        *buf = '\0';
    }
    AVL_print_node(t, o);
    if (t->lst) {
        int res = snprintf(buf, buf_sz, "%s", prf_left);
        AVL_print_subtree(t->lst, o, "| ", "  ", buf + res, buf_sz - res);
        *buf = '\0';
    }
}

static void BST_print_node(BinaryTreeADT t, FILE* o) {
    fprintf(o, "%s+-%d\n", buffer, t->rt->value);
}

static void BST_print_subtree(BinaryTreeADT t,
                              FILE* o,
                              const char* prf_right,
                              const char* prf_left,
                              char* buf, int buf_sz) {
    if (t->rst) {
        int res = snprintf(buf, buf_sz, "%s", prf_right);
        BST_print_subtree(t->rst, o, "  ", "| ", buf + res, buf_sz - res);
        *buf = '\0';
    }
    BST_print_node(t, o);
    if (t->lst) {
        int res = snprintf(buf, buf_sz, "%s", prf_left);
        BST_print_subtree(t->lst, o, "| ", "  ", buf + res, buf_sz - res);
        *buf = '\0';
    }
}

static void SPL_print_node(SplayTreeADT t, FILE* o) {
    fprintf(o, "%s+-%d\n", buffer, t->val);
}

static void SPL_print_subtree(SplayTreeADT t,
                              FILE* o,
                              const char* prf_right,
                              const char* prf_left,
                              char* buf, int buf_sz) {
    if (t->r) {
        int res = snprintf(buf, buf_sz, "%s", prf_right);
        SPL_print_subtree(t->r, o, "  ", "| ", buf + res, buf_sz - res);
        *buf = '\0';
    }
    SPL_print_node(t, o);
    if (t->l) {
        int res = snprintf(buf, buf_sz, "%s", prf_left);
        SPL_print_subtree(t->l, o, "| ", "  ", buf + res, buf_sz - res);
        *buf = '\0';
    }
}

static void RBT_print_node(RedBlackTreeADT t, FILE* o) {
    fprintf(o, "%s+-", buffer);
    fprintf(o, "%s%d", t->isRed ? "\x1B[31m" : "\x1B[37m", t->val);
    fprintf(o, "%s\n", "\x1B[0m");
}

static void RBT_print_subtree(RedBlackTreeADT t,
                              FILE* o,
                              const char* prf_right,
                              const char* prf_left,
                              char* buf, int buf_sz) {
    if (t->r) {
        int res = snprintf(buf, buf_sz, "%s", prf_right);
        RBT_print_subtree(t->r, o, "  ", "| ", buf + res, buf_sz - res);
        *buf = '\0';
    }
    RBT_print_node(t, o);
    if (t->l) {
        int res = snprintf(buf, buf_sz, "%s", prf_left);
        RBT_print_subtree(t->l, o, "| ", "  ", buf + res, buf_sz - res);
        *buf = '\0';
    }
}

void runPrint(ForestPtr forestPtr, size_t index, FILE* outputStream) {
    printInfo("Printing tree with index %zu.\n", index);
    if (forestPtr->items[index]->root == NULL) {
        printf("(Empty Tree)\n");
        return;
    }

    assert(TREE_TYPE_NUMBER == 4 && "HAVE NOT EXHAUST ALL TREE TYPES");
    switch (forestPtr->items[index]->type) {
        case AVL:
            AVL_print_subtree(forestPtr->items[index]->root, outputStream, "  ", "  ", buffer, sizeof(buffer));
            break;
        case BST:
            BST_print_subtree(forestPtr->items[index]->root, outputStream, "  ", "  ", buffer, sizeof(buffer));
            break;
        case SPL:
            SPL_print_subtree(forestPtr->items[index]->root, outputStream, "  ", "  ", buffer, sizeof(buffer));
            break;
        case RBT:
            RBT_print_subtree(forestPtr->items[index]->root, outputStream, "  ", "  ", buffer, sizeof(buffer));
            break;
        default:
            assert(false && "Unreachable");
            break;
    }
}

void runDelete(ForestPtr forest) {
    int index, val;
    if (!readAndParseSubcommandToInt(&index, DELETE) || !readAndParseSubcommandToInt(&val, DELETE)) return;

    if ((size_t)index >= forest->count) {
        printError("Tree with index %d not found\n", index);
        return;
    }

    assert(TREE_TYPE_NUMBER == 4 && "HAVE NOT EXHAUST ALL TREE TYPES");
    switch (forest->items[index]->type) {
        case AVL:
            todo("AVL delete function");
            break;
        case BST:
            if (!IsNodeExist(forest->items[index]->root, val)) {
                printf("[ERROR] Deleting non-existent node.\n");
                return;
            }
            forest->items[index]->root = DeleteNode(forest->items[index]->root, val);
            printf("[INFO] TreeNode(%d) deleted in Tree(%d).\n", val, index);
            break;
        case SPL:
            todo("SPL delete function");
            break;
        case RBT:
            todo("RBT delete");
            break;
        default:
            assert(false && "[ERROR] UNREACHABLE\n");
            break;
    }
}

void runNew(ForestPtr forestPtr, TreeType treeType) {    
    da_append(*forestPtr, (TreePtr)malloc(sizeof(Tree)));
    int index = forestPtr->count - 1;

    assert(TREE_TYPE_NUMBER == 4 && "HAVE NOT EXHAUST ALL TREE TYPES");
    switch (treeType) {
        case AVL:
            forestPtr->items[index]->type = AVL;
            forestPtr->items[index]->root = EmptyAVLTree();
            break;
        case BST:
            forestPtr->items[index]->type = BST;
            forestPtr->items[index]->root = EmptyBinaryTree();
            break;
        case SPL:
            forestPtr->items[index]->type = SPL;
            forestPtr->items[index]->root = NULL;
            break;
        case RBT:
            forestPtr->items[index]->type = RBT;
            forestPtr->items[index]->root = NULL;
            break;
        default:
            assert(false && "UNREACHABLE");
            return;
    }
    printf("[INFO] A new %s with index %d is created.\n", treetype2string(treeType), index);
}

void runDumpForest(ForestPtr forestPtr) {
    printInfo("Printing forest.");
    if (forestPtr->count == 0) {
        printf("(Empty forest)\n");
        return;
    }

    for (size_t i = 0; i < forestPtr->count; i++) {
        if (forestPtr->items[i]->root == NULL)
            printf("Tree %zu is a empty %s\n", i, treetype2string(forestPtr->items[i]->type));
        else {
            int rootVal;
            switch (forestPtr->items[i]->type) {
                case AVL:
                    rootVal = GetNodeValue(AVLRoot(forestPtr->items[i]->root));
                    break;
                case BST:
                    rootVal = GetNodeValue(Root(forestPtr->items[i]->root));
                    break;
                case SPL:
                    SplayTreeADT SPLroot = forestPtr->items[i]->root;
                    rootVal = SPLroot->val;
                    break;
                case RBT:
                    RedBlackTreeADT RBTroot = forestPtr->items[i]->root;
                    rootVal = RBTroot->val;
                    break;
                default:
                    assert(false && "UNREACHABLE");
                    break;
            }
            printf("Tree %zu is a %s with node %d as root\n", i, treetype2string(forestPtr->items[i]->type), rootVal);
        }
    }
}

void runSearch(ForestPtr forestPtr, size_t index, int val) {
    bool found;
    assert(TREE_TYPE_NUMBER == 4 && "HAVE NOT EXHAUST ALL TREE TYPES");
    switch (forestPtr->items[index]->type) {
        case BST:
            found = IsNodeExist(forestPtr->items[index]->root, val);
            break;
        case AVL:
            found = AVL_IsNodeExist(forestPtr->items[index]->root, val);
            break;
        case SPL:
            forestPtr->items[index]->root = Splay_Find(forestPtr->items[index]->root, val, &found);
            break;
        case RBT:
            found = Red_Black_Find(forestPtr->items[index]->root, val);
            break;
        default:
            assert(false && "UNREACHABLE");
            break;
    }
    if (found)
        printInfo("Node %d exists in tree %zu\n", val, index);
    else
        printInfo("Node %d does not exist in tree %zu\n", val, index);
}
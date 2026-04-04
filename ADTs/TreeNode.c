#include "TreeNode.h"
#include <stdlib.h>

TreeNodeADT NewTreeNode(int d) {
    TreeNodeADT N = malloc(sizeof(*N));
    N->value = d;
    return N;
}

bool IsSpecialErrNode(TreeNodeADT N) {
    return N == NULL;
}

int GetNodeValue(TreeNodeADT N) {
    if (IsSpecialErrNode(N)) exit(EXIT_FAILURE);
    return N->value;
}

TreeNodeADT SpecialErrNode() {
    return NULL;
}
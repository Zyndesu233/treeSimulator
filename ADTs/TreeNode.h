#ifndef TREE_NODE_H_
#define TREE_NODE_H_
#include <stdbool.h>
typedef struct TreeNodeCDT* TreeNodeADT;

struct TreeNodeCDT {
    int value;
};

TreeNodeADT NewTreeNode(int);
int GetNodeValue(TreeNodeADT);
bool IsSpecialErrNode(TreeNodeADT N);
TreeNodeADT SpecialErrNode();
#endif
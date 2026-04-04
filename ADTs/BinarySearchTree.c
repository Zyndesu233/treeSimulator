#include "BinarySearchTree.h"
#include <stdlib.h>
#include <string.h>

BinaryTreeADT EmptyBinaryTree() {
    return NULL;
}

BinaryTreeADT NonemptyBinaryTree(TreeNodeADT N,
                                 BinaryTreeADT L, BinaryTreeADT R) {
    BinaryTreeADT t = malloc(sizeof(*t));
    t->rt = N;
    t->lst = L;
    t->rst = R;
    return t;
}

TreeNodeADT Root(BinaryTreeADT t) {
    if (TreeIsEmpty(t)) exit(EXIT_FAILURE);
    return t->rt;
}

BinaryTreeADT LeftSubtree(BinaryTreeADT t) {
    if (TreeIsEmpty(t)) exit(EXIT_FAILURE);
    return t->lst;
}

BinaryTreeADT RightSubtree(BinaryTreeADT t) {
    if (TreeIsEmpty(t)) exit(EXIT_FAILURE);
    return t->rst;
}

bool TreeIsEmpty(BinaryTreeADT t) {
    return t == NULL;
}

BinaryTreeADT InsertNode(BinaryTreeADT t, TreeNodeADT n) {
    if (TreeIsEmpty(t))
        return NonemptyBinaryTree(n,
                                  EmptyBinaryTree(), EmptyBinaryTree());
    else {
        int sign = GetNodeValue(n) - GetNodeValue(Root(t));
        if (sign == 0) return NonemptyBinaryTree(n,
                                                 LeftSubtree(t), RightSubtree(t));
        if (sign < 0) return NonemptyBinaryTree(Root(t),
                                                InsertNode(LeftSubtree(t), n), RightSubtree(t));
        return NonemptyBinaryTree(Root(t),
                                  LeftSubtree(t), InsertNode(RightSubtree(t), n));
    }
}

static TreeNodeADT FindMinNode(BinaryTreeADT t) {
    if (TreeIsEmpty(t)) exit(EXIT_FAILURE);
    TreeNodeADT minNode = Root(t);
    if (!TreeIsEmpty(LeftSubtree(t))) {
        TreeNodeADT leftMinNode = FindMinNode(LeftSubtree(t));
        minNode = GetNodeValue(leftMinNode) < GetNodeValue(minNode) ?
                      leftMinNode :
                      minNode;
    }
    if (!TreeIsEmpty(RightSubtree(t))) {
        TreeNodeADT leftMinNode = FindMinNode(LeftSubtree(t));
        minNode = GetNodeValue(leftMinNode) < GetNodeValue(minNode) ?
                      leftMinNode :
                      minNode;
    }
    return minNode;
}

BinaryTreeADT DeleteNode(BinaryTreeADT t, int k) {
    if (TreeIsEmpty(t)) exit(EXIT_FAILURE);
    int sign = k - GetNodeValue(Root(t));
    if (sign < 0) return NonemptyBinaryTree(Root(t),
                                            DeleteNode(LeftSubtree(t), k), RightSubtree(t));
    if (sign > 0) return NonemptyBinaryTree(Root(t),
                                            LeftSubtree(t),
                                            DeleteNode(RightSubtree(t), k));
    if (!TreeIsEmpty(LeftSubtree(t)) && !TreeIsEmpty(RightSubtree(t))) {
        TreeNodeADT M = FindMinNode(RightSubtree(t));
        return NonemptyBinaryTree(M, LeftSubtree(t),
                                  DeleteNode(RightSubtree(t), GetNodeValue(M)));
    };
    return TreeIsEmpty(RightSubtree(t)) ? LeftSubtree(t) : RightSubtree(t);
}

bool IsNodeExist(BinaryTreeADT t, int k) {
    if (TreeIsEmpty(t)) return false;
    int sign = k -GetNodeValue(Root(t));
    if(sign < 0) return IsNodeExist(LeftSubtree(t), k);
    if(sign > 0) return IsNodeExist(RightSubtree(t), k);
    return true;
}
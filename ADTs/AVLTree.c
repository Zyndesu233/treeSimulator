#include "AVLTree.h"
#include <stdlib.h>
#include <string.h>

AVLTreeADT EmptyAVLTree() {
    return NULL;
}
AVLTreeADT NonemptyAVLTree(TreeNodeADT N,
                           AVLTreeADT L, AVLTreeADT R) {
    // printf("DEBUG: Executing NonemptyAVLTree\n");
    AVLTreeADT t = malloc(sizeof(*t));
    t->rt = N;
    t->lst = L;
    t->rst = R;
    // printf("DEBUG: t = %p\n", t);
    return t;
}

TreeNodeADT AVLRoot(AVLTreeADT t) {
    if (AVLTreeIsEmpty(t)) exit(EXIT_FAILURE);
    return t->rt;
}

AVLTreeADT LeftAVLSubtree(AVLTreeADT t) {
    if (AVLTreeIsEmpty(t)) exit(EXIT_FAILURE);
    return t->lst;
}

AVLTreeADT RightAVLSubtree(AVLTreeADT t) {
    if (AVLTreeIsEmpty(t)) exit(EXIT_FAILURE);
    return t->rst;
}

int AVLTreeIsEmpty(AVLTreeADT t) {
    return t == NULL;
}

int AVLTreeHeight(AVLTreeADT t) {
    if(t == NULL) return -1;
    return AVLTreeHeight(t->lst) > AVLTreeHeight(t->rst)? AVLTreeHeight(t->lst)+1: AVLTreeHeight(t->rst)+1;
}

AVLTreeADT LeftRotate(AVLTreeADT t) {
    return NonemptyAVLTree(
        AVLRoot(RightAVLSubtree(t)),
        NonemptyAVLTree(
            AVLRoot(t),
            LeftAVLSubtree(t),
            LeftAVLSubtree(RightAVLSubtree(t))
        ),
        RightAVLSubtree(RightAVLSubtree(t))
    );
}

AVLTreeADT RightRotate(AVLTreeADT t) {
    return NonemptyAVLTree(
        AVLRoot(LeftAVLSubtree(t)),
        LeftAVLSubtree(LeftAVLSubtree(t)),
        NonemptyAVLTree(
            AVLRoot(t),
            RightAVLSubtree(LeftAVLSubtree(t)),
            RightAVLSubtree(t)
        )
    );
}

AVLTreeADT LeftRightRotate(AVLTreeADT t) {
    return RightRotate(
        NonemptyAVLTree(
            AVLRoot(t),
            LeftRotate(LeftAVLSubtree(t)),
            RightAVLSubtree(t)
        )
    );
}

AVLTreeADT RightLeftRotate(AVLTreeADT t) {
    return LeftRotate(
        NonemptyAVLTree(
            AVLRoot(t),
            LeftAVLSubtree(t),
            RightRotate(RightAVLSubtree(t))
        )
    );
}

AVLTreeADT AVLInsertNode(TreeNodeADT X, AVLTreeADT T) {
    // printf("DEBUG: Executing AVLInsertNode\n");
    if (AVLTreeIsEmpty(T))
        return NonemptyAVLTree(X, EmptyAVLTree(), EmptyAVLTree());
    int sign = GetNodeValue(X) - GetNodeValue(AVLRoot(T));
    if (sign < 0) {
        AVLTreeADT NewTree = NonemptyAVLTree(AVLRoot(T),
                                             AVLInsertNode(X, LeftAVLSubtree(T)),
                                             RightAVLSubtree(T));
        if (AVLTreeHeight(LeftAVLSubtree(NewTree)) - AVLTreeHeight(RightAVLSubtree(NewTree)) == 2)
            return (GetNodeValue(X) -
                           GetNodeValue(AVLRoot(LeftAVLSubtree(NewTree)))) < 0 ?
                        RightRotate(NewTree) :
                        LeftRightRotate(NewTree);
        return NewTree;
    }
    if (sign > 0) {
        AVLTreeADT NewTree = NonemptyAVLTree(AVLRoot(T),
                                             LeftAVLSubtree(T),
                                             AVLInsertNode(X, RightAVLSubtree(T)));

        if (AVLTreeHeight(RightAVLSubtree(NewTree)) - AVLTreeHeight(LeftAVLSubtree(NewTree)) == 2)
            return (GetNodeValue(X) -
                           GetNodeValue(AVLRoot(RightAVLSubtree(NewTree)))) > 0 ?
                        LeftRotate(NewTree) :
                        RightLeftRotate(NewTree);
        return NewTree;
    }
    return NonemptyAVLTree(X, LeftAVLSubtree(T), RightAVLSubtree(T));
}

bool AVL_IsNodeExist(AVLTreeADT t, int k) {
    if (AVLTreeIsEmpty(t)) return false;
    int sign = k-GetNodeValue(AVLRoot(t));
    if(sign < 0) return AVL_IsNodeExist(LeftAVLSubtree(t), k);
    if(sign > 0) return AVL_IsNodeExist(RightAVLSubtree(t), k);
    return true;
}
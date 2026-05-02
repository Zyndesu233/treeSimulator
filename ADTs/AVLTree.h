#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include "Tree.h"
#include <stdbool.h>

typedef struct AVLTreeCDT* AVLTreeADT;

struct AVLTreeCDT {
    int val;
    int height;
    AVLTreeADT l, r;
};

AVLTreeADT AVL_Insert(AVLTreeADT t, int val);
bool AVL_Find(AVLTreeADT t, int val);
void AVL_Print(AVLTreeADT t);

#endif
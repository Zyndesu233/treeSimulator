#ifndef AVL_TREE_H_
#define AVL_TREE_H_
#include "TreeNode.h"
#include <stdio.h> 
typedef struct AVLTreeCDT *AVLTreeADT; 

AVLTreeADT NonemptyAVLTree(TreeNodeADT, AVLTreeADT, AVLTreeADT); 
AVLTreeADT EmptyAVLTree(void); 
AVLTreeADT LeftAVLSubtree(AVLTreeADT); 
AVLTreeADT RightAVLSubtree(AVLTreeADT); 
int AVLTreeIsEmpty(AVLTreeADT); 
int AVLTreeHeight(AVLTreeADT); 
TreeNodeADT AVLRoot(AVLTreeADT); 
AVLTreeADT AVLInsertNode(TreeNodeADT, AVLTreeADT);
bool AVL_IsNodeExist(AVLTreeADT, int);

struct AVLTreeCDT {
    TreeNodeADT rt;
    AVLTreeADT lst, rst;
};

#endif
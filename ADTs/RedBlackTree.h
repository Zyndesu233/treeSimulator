#ifndef RED_BLACK_TREE_H_
#define RED_BLACK_TREE_H_
#include <stdbool.h>
typedef struct RedBlackTreeCDT* RedBlackTreeADT;

struct RedBlackTreeCDT {
    int val;
    bool isRed;
    RedBlackTreeADT p, l, r;
};

RedBlackTreeADT Red_Black_Insert(RedBlackTreeADT t, int val);
bool Red_Black_Find(RedBlackTreeADT t, int val);
#endif
#ifndef SPLAY_TREE_H_
#define SPLAY_TREE_H_
#include <stdbool.h>
typedef struct SplayTreeCDT* SplayTreeADT;

struct SplayTreeCDT {
    int val;
    SplayTreeADT p, l, r;
};

SplayTreeADT Splay_Insert(SplayTreeADT t, int val);
SplayTreeADT Splay_Find(SplayTreeADT t, int val, bool* found);
#endif
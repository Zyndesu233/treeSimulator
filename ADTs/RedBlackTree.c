#include "RedBlackTree.h"
#include <stdlib.h>

/*
[WARNNING] The following code is AI-generated.
*/

static void rotate_left(RedBlackTreeADT* root, RedBlackTreeADT x) {
    RedBlackTreeADT y = x->r;
    x->r = y->l;
    if (y->l != NULL) y->l->p = x;
    y->p = x->p;
    if (x->p == NULL)
        *root = y;
    else if (x == x->p->l)
        x->p->l = y;
    else
        x->p->r = y;
    y->l = x;
    x->p = y;
}

static void rotate_right(RedBlackTreeADT* root, RedBlackTreeADT y) {
    RedBlackTreeADT x = y->l;
    y->l = x->r;
    if (x->r != NULL) x->r->p = y;
    x->p = y->p;
    if (y->p == NULL)
        *root = x;
    else if (y == y->p->r)
        y->p->r = x;
    else
        y->p->l = x;
    x->r = y;
    y->p = x;
}

RedBlackTreeADT Red_Black_Insert(RedBlackTreeADT t, int val) {
    // 1. Standard BST Insertion
    RedBlackTreeADT z = malloc(sizeof(struct RedBlackTreeCDT));
    z->val = val;
    z->l = z->r = z->p = NULL;
    z->isRed = true;  // New nodes are always Red

    RedBlackTreeADT y = NULL;
    RedBlackTreeADT x = t;

    while (x != NULL) {
        y = x;
        if (z->val < x->val)
            x = x->l;
        else
            x = x->r;
    }

    z->p = y;
    if (y == NULL)
        t = z;  // Tree was empty
    else if (z->val < y->val)
        y->l = z;
    else
        y->r = z;

    // 2. Fix-up phase
    while (z != t && z->p->isRed) {
        if (z->p == z->p->p->l) {  // Parent is a left child
            RedBlackTreeADT uncle = z->p->p->r;
            if (uncle && uncle->isRed) {  // Case 1: Uncle is Red
                z->p->isRed = false;
                uncle->isRed = false;
                z->p->p->isRed = true;
                z = z->p->p;
            } else {
                if (z == z->p->r) {  // Case 2: Zig-Zag
                    z = z->p;
                    rotate_left(&t, z);
                }
                // Case 3: Zig-Zig
                z->p->isRed = false;
                z->p->p->isRed = true;
                rotate_right(&t, z->p->p);
            }
        } else {  // Symmetric case: Parent is a right child
            RedBlackTreeADT uncle = z->p->p->l;
            if (uncle && uncle->isRed) {
                z->p->isRed = false;
                uncle->isRed = false;
                z->p->p->isRed = true;
                z = z->p->p;
            } else {
                if (z == z->p->l) {
                    z = z->p;
                    rotate_right(&t, z);
                }
                z->p->isRed = false;
                z->p->p->isRed = true;
                rotate_left(&t, z->p->p);
            }
        }
    }
    t->isRed = false;  // Root must always be Black
    return t;
}

bool Red_Black_Find(RedBlackTreeADT t, int val) {
    if(t->val == val) return true;
    return Red_Black_Find(t->l, val) || Red_Black_Find(t->r, val);
}
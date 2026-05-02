#include "AVLTree.h"
#include <stdio.h>
#include <stdlib.h>

static int height(AVLTreeADT t) {
    return t ? t->height : -1;
}

static AVLTreeADT new_node(int val) {
    AVLTreeADT n = malloc(sizeof(*n));
    if (!n) exit(EXIT_FAILURE);
    n->val = val;
    n->height = 0;
    n->l = NULL;
    n->r = NULL;
    return n;
}

static void update_height(AVLTreeADT t) {
    if (t) {
        t->height = max(height(t->l), height(t->r)) + 1;
    }
}

static int balance_factor(AVLTreeADT t) {
    return t ? height(t->l) - height(t->r) : 0;
}

static AVLTreeADT right_rotate(AVLTreeADT t) {
    AVLTreeADT x = t->l;
    AVLTreeADT T2 = x->r;

    x->r = t;
    t->l = T2;

    update_height(t);
    update_height(x);
    return x;
}

static AVLTreeADT left_rotate(AVLTreeADT t) {
    AVLTreeADT y = t->r;
    AVLTreeADT T2 = y->l;

    y->l = t;
    t->r = T2;

    update_height(t);
    update_height(y);
    return y;
}

static AVLTreeADT left_right_rotate(AVLTreeADT t) {
    t->l = left_rotate(t->l);
    return right_rotate(t);
}

static AVLTreeADT right_left_rotate(AVLTreeADT t) {
    t->r = right_rotate(t->r);
    return left_rotate(t);
}

AVLTreeADT AVL_Insert(AVLTreeADT t, int val) {
    if (t == NULL) return new_node(val);

    if (val < t->val) {
        t->l = AVL_Insert(t->l, val);
    } else if (val > t->val) {
        t->r = AVL_Insert(t->r, val);
    } else {
        return t;
    }

    update_height(t);

    int bf = balance_factor(t);

    if (bf > 1 && val < t->l->val) return right_rotate(t);       /* LL */
    if (bf < -1 && val > t->r->val) return left_rotate(t);       /* RR */
    if (bf > 1 && val > t->l->val) return left_right_rotate(t);  /* LR */
    if (bf < -1 && val < t->r->val) return right_left_rotate(t); /* RL */

    return t;
}

bool AVL_Find(AVLTreeADT t, int val) {
    if (t == NULL) return false;
    if (val < t->val) return AVL_Find(t->l, val);
    if (val > t->val) return AVL_Find(t->r, val);
    return true;
}

static void avl_print_inorder(AVLTreeADT t) {
    if (t == NULL) return;
    avl_print_inorder(t->l);
    printf("%d ", t->val);
    avl_print_inorder(t->r);
}

void AVL_Print(AVLTreeADT t) {
    avl_print_inorder(t);
    printf("\n");
}
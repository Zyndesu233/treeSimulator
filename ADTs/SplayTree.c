#include "SplayTree.h"
#include <stdio.h>
#include <stdlib.h>

/*
[WARNNING] The following code is AI-generated.
*/

static void leftRotation(SplayTreeADT x) {
    SplayTreeADT y = x->r;
    if (y == NULL) return;

    x->r = y->l;
    if (y->l != NULL) {
        y->l->p = x;
    }

    y->p = x->p;
    if (x->p != NULL) {
        if (x == x->p->l)
            x->p->l = y;
        else
            x->p->r = y;
    }

    y->l = x;
    x->p = y;
}

static void rightRotation(SplayTreeADT x) {
    SplayTreeADT y = x->l;
    if (y == NULL) return;

    x->l = y->r;
    if (y->r != NULL) {
        y->r->p = x;
    }

    y->p = x->p;
    if (x->p != NULL) {
        if (x == x->p->l)
            x->p->l = y;
        else
            x->p->r = y;
    }

    y->r = x;
    x->p = y;
}

static void splay(SplayTreeADT x) {
    while (x->p != NULL) {
        SplayTreeADT p = x->p;
        SplayTreeADT g = p->p;

        if (g == NULL) {  // Zig
            if (x == p->l)
                rightRotation(p);
            else
                leftRotation(p);
        } else if (p == g->l && x == p->l) {  // Zig-Zig (Left-Left)
            rightRotation(g);                 // Rotate parent first
            rightRotation(p);
        } else if (p == g->r && x == p->r) {  // Zig-Zig (Right-Right)
            leftRotation(g);                  // Rotate parent first
            leftRotation(p);
        } else if (p == g->l && x == p->r) {  // Zig-Zag (Left-Right)
            leftRotation(p);
            rightRotation(g);
        } else {  // Zig-Zag (Right-Left)
            rightRotation(p);
            leftRotation(g);
        }
    }
}

SplayTreeADT Splay_Insert(SplayTreeADT t, int val) {
    if (t == NULL) {
        SplayTreeADT newNode = malloc(sizeof(*newNode));
        newNode->val = val;
        newNode->p = newNode->l = newNode->r = NULL;
        return newNode;
    }

    SplayTreeADT curr = t;
    SplayTreeADT parent = NULL;

    // Standard BST Insertion
    while (curr != NULL) {
        parent = curr;
        if (val < curr->val) {
            curr = curr->l;
        } else if (val > curr->val) {
            curr = curr->r;
        } else {
            // Value already exists, splay the existing node
            splay(curr);
            return curr;
        }
    }

    // Create the new node
    SplayTreeADT newNode = malloc(sizeof(*newNode));
    newNode->val = val;
    newNode->p = parent;
    newNode->l = newNode->r = NULL;

    if (val < parent->val)
        parent->l = newNode;
    else
        parent->r = newNode;

    // Splay the new node to the root
    splay(newNode);

    return newNode;  // Return the new root
}

SplayTreeADT Splay_Find(SplayTreeADT t, int val, bool* found) {
    if (t == NULL) {
        *found = false;
        return NULL;
    }

    SplayTreeADT curr = t;
    SplayTreeADT last_valid = t;

    while (curr != NULL) {
        last_valid = curr;
        if (val == curr->val) {
            splay(curr);
            *found = true;
            return curr;  // New root
        }
        if (val < curr->val)
            curr = curr->l;
        else
            curr = curr->r;
    }

    // Not found: Splay the last node we touched before hitting NULL
    splay(last_valid);
    *found = false;
    return last_valid;  // New root
}
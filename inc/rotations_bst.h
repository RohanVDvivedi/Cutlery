#ifndef ROTATIONS_BST_H
#define ROTATIONS_BST_H

#include<bst.h>

/*      A                                _B_
**     /  \                             /   \
**    W    B        left rotation      A     C
**        /  \        --------->>     / \   /  \
**       X    C                      W   X Y    Z
**           / \
**          Y   Z
** returns true if rotation was successfull*/
int left_rotate_tree(bst* bst_p, bstnode* A);

/*           A                                 _B_
**         /   \                              /   \
**       B      W      right rotation        C     A
**     /   \             --------->>        / \   /  \
**   C      X                              Z   Y  X   W
**  / \
** Z   Y
** returns true if rotation was successfull*/
int right_rotate_tree(bst* bst_p, bstnode* A);

void exchange_positions_in_bst(bst* bst_p, bstnode* A, bstnode* B);

#endif
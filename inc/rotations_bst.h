#ifndef ROTATIONS_BST_H
#define ROTATIONS_BST_H

#include<balancedbst.h>

// use generic api only as mentioned in balancedbst.h

#define node bstnode

/*      A                                _B_
**     /  \                             /   \
**    W    B        left rotation      A     C
**        /  \        --------->>     / \   /  \
**       X    C                      W   X Y    Z
**           / \
**          Y   Z
** returns true if rotation was successfull*/
int left_rotate_tree(balancedbst* balancedbst_p, node* A);

/*           A                                 _B_
**         /   \                              /   \
**       B      W      right rotation        C     A
**     /   \             --------->>        / \   /  \
**   C      X                              Z   Y  X   W
**  / \
** Z   Y
** returns true if rotation was successfull*/
int right_rotate_tree(balancedbst* balancedbst_p, node* A);

#undef node

#endif
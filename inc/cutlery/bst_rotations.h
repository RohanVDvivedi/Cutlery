#ifndef ROTATIONS_BST_H
#define ROTATIONS_BST_H

#include<cutlery/bst.h>

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

/*
** 	This method exhanges the positins of the two given nodes
** 	A and B, must not be NULL and must not be the same node
**  and they must belong to the same given bst tree
**
**  It also handles special cases occuring when A is parent of B or when B is parent of A
**  It also makes the relatives of A and B to point to B and A respectively
**
**  As intution suggests this method does not alter data of your nodes, 
**  it just changes the locations of these nodes in the tree by changing pointers appropirately
**  it is used in remove function of non balancing tree, 
**  #### try not to modify or change it, unless you feel that you are super smart
*/
void exchange_positions_in_bst(bst* bst_p, bstnode* A, bstnode* B);

#endif
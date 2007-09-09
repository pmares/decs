#include "node.h"

namespace dlx {

Node::Node():
	left(),
	right(),
	up(),
	down(),
	col()
{
}

Node::~Node() {
	left->right = 0;
	right->left = 0;
	up->down = 0;
	down->up = 0;
	
	left = 0;
	right = 0;
	up = 0;
	down = 0;
}

/**
 * Link the node back into the column list. It changes the links from its up and
 * down nodes to point to itself. It returns 0 on success and -1 if it either
 * one or both of the up and down pointers are null. If one of the pointers are
 * null the linking may only be partially completed.  
 */
int Node::linkColumn() {
	if (!up || !down)
		return -1;
	
//	col->incrementSize();  // TODO This should work, but C++ is being an ass.
	down->up = this;
	up->down = this;
	
	return 0;
}

/**
 * Unlink the node from the column list. It changes the links from its up and
 * down node to point to its down and up node respectively. It returns 0 on
 * success and -1 if it either one or both of the up and down pointers are null.
 * If one of the pointers are null the unlinking may only be partially
 * completed.  
 */
int Node::unlinkColumn() {
	if (!up || !down)
		return -1;
	
	
	down->up = up;
	up->down = down;
//	col->decrementSize();  // TODO This should work, but C++ is being an ass.
	
	return 0;	
}

/**
 * Link the node back into the row list. It changes the links from its left and
 * right node to point to itself. It returns 0 on success and -1 if it either
 * one or both of the left and right pointers are null. If one of the pointers
 * are null the linking may only be partially completed.  
 */
int Node::linkRow() {
	if (!right || !left)
		return -1;
	
	down->up = this;
	up->down = this;
	
	return 0;
}

/**
 * Unlink the node from the row list. It changes the links from its left and
 * right node to point to its right and left node respectively. It returns 0 on
 * success and -1 if it either one or both of the left and right pointers are
 * null. If one of the pointers are null the unlinking may only be partially
 * completed.  
 */
int Node::unlinkRow() {
	if (!right || !left)
		return -1;
	
	down->up = up;
	up->down = down;
	
	return 0;
}

} /*namespace dlx*/

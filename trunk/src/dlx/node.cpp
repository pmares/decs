#include "node.h"
#include <iostream>

namespace dlx {

Node::Node():
	left(),
	right(),
	up(),
	down(),
	col(),
	row(0)
{
	left = right = up = down = this;
}

Node::Node(uint row):
	left(),
	right(),
	up(),
	down(),
	col(),
	row(row)
{
	left = right = up = down = this;
}

Node::~Node() {
	if (left) left->right = 0;
	if (right) right->left = 0;
	if (up) up->down = 0;
	if (down) down->up = 0;
	
	left = 0;
	right = 0;
	up = 0;
	down = 0;
}

/**
 * Link the node back into the column list. It changes the links from its up and
 * down nodes to point to itself. It returns 0 on success and 1 if either
 * one or both of the up and down pointers are null. If one of the pointers are
 * null the linking may only be partially completed.  
 */
int Node::linkColumn() {
	if (!up || !down)
		return 1;
	
//	col->incrementSize();  // TODO This should work, but C++ is being an ass.
	down->up = this;
	up->down = this;
	
	return 0;
}

/**
 * Unlink the node from the column list. It changes the links from its up and
 * down node to point to its down and up node respectively. It returns 0 on
 * success and 1 if either one or both of the up and down pointers are null.
 * If one of the pointers are null the unlinking may only be partially
 * completed.  
 */
int Node::unlinkColumn() {
	if (!up || !down)
		return 1;
	
	down->up = up;
	up->down = down;
//	col->decrementSize();  // TODO This should work, but C++ is being an ass.
	
	return 0;	
}

/**
 * Link the node back into the row list. It changes the links from its left and
 * right node to point to itself. It returns 0 on success and 1 if either
 * one or both of the left and right pointers are null. If one of the pointers
 * are null the linking may only be partially completed.  
 */
int Node::linkRow() {
	if (!right || !left)
		return 1;
	
	right->left = this;
	left->right = this;
	
	return 0;
}

/**
 * Unlink the node from the row list. It changes the links from its left and
 * right node to point to its right and left node respectively. It returns 0 on
 * success and 1 if either one or both of the left and right pointers are
 * null. If one of the pointers are null the unlinking may only be partially
 * completed.  
 */
int Node::unlinkRow() {
	if (!right || !left)
		return 1;
	
	right->left = left;
	left->right = right;
	
	return 0;
}

/**
 * Print out the nodes this node is linked to.
 */
void Node::expose() {
	std::cout << "[row] = [" << ", " << row << "] ";
	std::cout << "[this,left,right,up,down] = [" << this << ", " << left << ", " << right << ", " << up << ", " << down << "]" << std::endl;
}

} /*namespace dlx*/

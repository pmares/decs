/**
 * libdlx - Exact cover solver library using the Dancing Links algorithm.
 * Copyright (C) 2007  Jan Magne Tjensvold
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include "node.h"

namespace dlx {

Node::Node():
	col(),
	row(0)
{
	left = right = up = down = this;
}

Node::Node(uint row):
	col(),
	row(row)
{
	left = right = up = down = this;
}

Node::~Node() {
	left = 0;
	right = 0;
	up = 0;
	down = 0;
	col = 0;
}

/**
 * Link the node back into the column list. It changes the links from its up and
 * down nodes to point to itself. If one of the pointers are null the linking
 * may only be partially completed.
 */
void Node::linkColumn() {
//	col->incrementSize();  // TODO This should work, but C++ is being an ass.
	down->up = this;
	up->down = this;
}

/**
 * Unlink the node from the column list. It changes the links from its up and
 * down node to point to its down and up node respectively. If one of the
 * pointers are null the unlinking may only be partially completed.
 */
void Node::unlinkColumn() {
	down->up = up;
	up->down = down;
//	col->decrementSize();  // TODO This should work, but C++ is being an ass.
}

/**
 * Link the node back into the row list. It changes the links from its left and
 * right node to point to itself. If one of the pointers are null the linking
 * may only be partially completed.
 */
void Node::linkRow() {
	right->left = this;
	left->right = this;
}

/**
 * Unlink the node from the row list. It changes the links from its left and
 * right node to point to its right and left node respectively. If one of the
 * pointers are null the unlinking may only be partially completed.
 */
void Node::unlinkRow() {
	right->left = left;
	left->right = right;
}

} /*namespace dlx*/

/**
 * sbmatrix - Sparse boolean matrix data structure library for DECS.
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

#include "sbnode.h"

SBNode::SBNode():
	col(),
	row(0)
{
	left = right = up = down = this;
}

SBNode::SBNode(uint row):
	col(),
	row(row)
{
	left = right = up = down = this;
}

SBNode::~SBNode() {
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
void SBNode::linkColumn() {
	down->up = this;
	up->down = this;
}

/**
 * Unlink the node from the column list. It changes the links from its up and
 * down node to point to its down and up node respectively. If one of the
 * pointers are null the unlinking may only be partially completed.
 */
void SBNode::unlinkColumn() {
	down->up = up;
	up->down = down;
}

/**
 * Link the node back into the row list. It changes the links from its left and
 * right node to point to itself. If one of the pointers are null the linking
 * may only be partially completed.
 */
void SBNode::linkRow() {
	right->left = this;
	left->right = this;
}

/**
 * Unlink the node from the row list. It changes the links from its left and
 * right node to point to its right and left node respectively. If one of the
 * pointers are null the unlinking may only be partially completed.
 */
void SBNode::unlinkRow() {
	right->left = left;
	left->right = right;
}

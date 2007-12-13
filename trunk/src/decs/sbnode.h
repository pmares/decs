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

#ifndef SB_NODE_H_
#define SB_NODE_H_

#include "type.h"

// Forward declaration to prevent circular dependency between headers.
class SBColumn;

class SBNode {
public:
	SBNode();
	SBNode(uint row);
	virtual ~SBNode();
	SBNode* getLeft() const { return left; }
	SBNode* getRight() const { return right; }
	SBNode* getUp() const { return up; }
	SBNode* getDown() const { return down; }
	uint getRow() const {return row; }
	SBColumn* getColumn() const { return col; }
	void setLeft(SBNode* node) { left = node; }
	void setRight(SBNode* node) { right = node; }
	void setUp(SBNode* node) { up = node; }
	void setDown(SBNode* node) { down = node; }
	void setColumn(SBColumn* column) { col = column; }
	void linkColumn();
	void unlinkColumn();
	void linkRow();
	void unlinkRow();
private:
	SBNode* left;
	SBNode* right;
	SBNode* up;
	SBNode* down;
	SBColumn* col;
	uint row;
};

#endif /*SB_NODE_H_*/

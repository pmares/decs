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

#ifndef DLX_NODE_H_
#define DLX_NODE_H_

#include "util.h"

namespace dlx {

// Forward declaration to prevent circular dependency between headers.
class Column;

class Node {
public:
	Node();
	Node(uint row);
	virtual ~Node();
	Node* getLeft() const { return left; }
	Node* getRight() const { return right; }
	Node* getUp() const { return up; }
	Node* getDown() const { return down; }
	uint getRow() const {return row; }
	Column* getColumn() const { return col; }
	void setLeft(Node* node) { left = node; }
	void setRight(Node* node) { right = node; }
	void setUp(Node* node) { up = node; }
	void setDown(Node* node) { down = node; }
	void setColumn(Column* column) { col = column; }
	void linkColumn();
	void unlinkColumn();
	void linkRow();
	void unlinkRow();
private:
	Node* left;
	Node* right;
	Node* up;
	Node* down;
	Column* col;
	uint row;
};

} /*namespace dlx*/

#endif /*DLX_NODE_H_*/

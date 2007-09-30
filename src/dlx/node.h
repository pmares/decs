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
	int linkColumn();
	int unlinkColumn();
	int linkRow();
	int unlinkRow();
	void expose();
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

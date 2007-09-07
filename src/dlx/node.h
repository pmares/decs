#ifndef NODE_H_
#define NODE_H_

// Forward declaration to prevent circular dependency.
class Column;

namespace dlx
{

class Node {
public:
	Node();
	virtual ~Node();
	Node* getLeft() const { return left; }
	Node* getRight() const { return right; }
	Node* getUp() const { return up; }
	Node* getDown() const { return down; }
	Column* getColumn() const { return col; }
	void setLeft(Node *node) { left = node; }
	void setRight(Node *node) { right = node; }
	void setUp(Node *node) { right = node; }
	void setDown(Node *node) { right = node; }
	void setColumn(Column *column) { col = column; }
private:
	Node *left;
	Node *right;
	Node *up;
	Node *down;
	Column *col;
};

}

#endif /*NODE_H_*/

#include <climits>
#include <iostream>
#include "dlx.h"
#include "node.h"
#include "column.h"

namespace dlx {

const uint MAX_LEVELS = 128;

Column* r;
Node* o[MAX_LEVELS];

using namespace std;

/**
 * Write a message to stderr and exit with a -1 exit code indicating failure.
 */
void panic(char* msg) {
	cerr << msg << endl;
	exit(-1);
}

/**
 * Print the solutions.
 */
void print_solution(uint level) {
	cout << "Solution found: " << endl;
	for (uint i = 0; i < level - 1; i++) {
		cout << "Level " << i << ": ";

		Node* obj = o[i];
		cout << obj->getColumn()->getName() << " ";
		for (Node* j = obj->getRight(); j != obj ; j = j->getRight()) {
			cout << j->getColumn()->getName() << " ";
		}
		
		cout << endl;
	}
}

/**
 * Cover column c.
 */
void cover(Column* c) {
	c->unlinkRow();
	for (Node* i = c->getDown(); i != c; i = i->getDown()) {
		for (Node* j = i->getRight(); j != i; j = j->getRight()) {
			j->unlinkColumn();
			j->getColumn()->decrementSize(); // TODO C++ being an ass...
		}
	}
}

/**
 * Uncover column c.
 */
void uncover(Column* c) {
	for (Node* i = c->getUp(); i != c; i = i->getUp()) {
		for (Node* j = i->getLeft(); j != i; j = j->getLeft()) {
			j->getColumn()->incrementSize(); // TODO C++ being an ass...
			j->linkColumn();
		}
	}
	c->linkRow();
}

/**
 * Choose a column to cover by using the column size heuristic in order to
 * minimize the branching factor.
 */
Column* choose_column(Column* header) {
	Column* c;
	uint s = UINT_MAX;

	for (Node* j = header->getRight(); j != header; j = j->getRight()) {
		Column* temp = j->getColumn();
		if (temp->getSize() < s) {
			c = temp;
			s = temp->getSize();
		}
	}
	return c;
}

/**
 * Search through the remaining matrix for a solution.
 * Initially invoked with k = 0.
 */
void search(uint k) {
	Column* h;

	// Return if all columns have been covered.
	if (h->getRight() == h) {
		print_solution(k);
		return;
	}
	
	Column* c = choose_column(h);
	if (!c) panic("No column found");
	
	cover(c);
	
	// Cover all columns which had nodes removed from cover(c).
	for (Node* r = c->getDown(); r != c; r = r->getDown()) {
		o[k] = r;

		for (Node* j = r->getRight(); j != r; j = j->getRight())
			cover(j->getColumn());
		
		search(k + 1);		
		r = o[k];
		c = r->getColumn();
		
		for (Node* j = r->getLeft(); j != r; j = j->getLeft())
			uncover(j->getColumn());
	}

	uncover(c);
}

} /*namespace dlx*/

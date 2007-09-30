#include <climits>
#include <iostream>
#include "dlx.h"
#include "node.h"
#include "column.h"
#include "util.h"

namespace dlx {

const uint MAX_LEVELS = 128;

Column* h = 0;
Node* o[MAX_LEVELS];
uint solutions = 0;

using namespace std;

/**
 * Print the solutions.
 */
void print_solution(uint level) {
	solutions++;
	cout << "Solution: ";
	for (uint i = 0; i < level; i++) {
		cout << o[i]->getRow() << " ";
		
//		Node* n = o[i];
//		cout << n->getColumn()->getIndex() << " ";
//		for (Node* j = n->getRight(); j != n ; j = j->getRight()) {
//			cout << j->getColumn()->getIndex() << " ";
//		}
		
	}
	cout << endl;
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
	Column* c = 0;
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

/**
 * Read from a file and solve the DLX matrix within.
 */
int solve(char* file) {
	// Verify the file format and create the column and node objects.
	h = new Column();
	int result = read_file(file, h);
	if (result) return result;

	// Do the dance.
	cout << "Searching..." << endl;
	search(0);
	cout << "Search complete: " << solutions << " solution(s) found." << endl;
	return 0;
}


} /*namespace dlx*/

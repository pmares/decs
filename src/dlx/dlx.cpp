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
uint verbose = 1;

using namespace std;

/**
 * Print the solutions.
 */
void printSolution(uint level) {
	cout << "Solution: ";
	for (uint i = 0; i < level; i++) {
		cout << o[i]->getRow() << " ";
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
Column* chooseColumn(Column* header) {
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
		solutions++;
		if (verbose > 1) printSolution(k);
		return;
	}
	
	Column* c = chooseColumn(h);
	if (!c) panic("no column found");
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
 * Set the verbose level. Higher means more output.
 * Default is 1.
 */
void setVerboseLevel(uint level) {
	verbose = level;
}

/**
 * Read from a file and solve the DLX matrix within.
 */
int solve(char* file) {
	// Verify the file format and create the column and node objects.
	h = new Column();
	int result = read_file(file, h, verbose);
	if (result) return result;

	// Do the dance.
	if (verbose > 0) cout << "Searching..." << endl;
	search(0);
	if (verbose > 0) cout << "Search complete: " << solutions << " solution(s) found." << endl;
	return 0;
}


} /*namespace dlx*/

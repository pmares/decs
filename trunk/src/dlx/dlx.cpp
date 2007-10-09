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

Column* h = 0;
Node* o[MAX_LEVELS];
uint solutions = 0;
uint verbose = 1;
uint updates = 0;
uint profile[MAX_LEVELS];
uint maxLevel = 0;
uint level = 0;

using namespace std;

/**
 * Print the solutions.
 */
void printSolution() {
	cout << "Solution: ";
	for (uint i = 0; i < level; i++) {
		cout << o[i]->getRow() << " ";
	}
	cout << endl;
}

/**
 * Cover column c.
 */
inline void cover(Column* c) {
	c->unlinkRow();
	uint k = 1;
	for (Node* i = c->getDown(); i != c; i = i->getDown()) {
		for (Node* j = i->getRight(); j != i; j = j->getRight()) {
			j->unlinkColumn();
			k++;
			j->getColumn()->decrementSize(); // TODO C++ being an ass...
		}
	}
	updates += k;
	profile[level] += k;
}

/**
 * Uncover column c.
 */
inline void uncover(Column* c) {
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
inline Column* chooseColumn() {
	Column* c = NULL;
	uint s = UINT_MAX;
	
	for (Node* j = h->getRight(); j != h; j = j->getRight()) {
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
 * Initially invoked when level = 0.
 */
void search() {
	if (level > maxLevel) maxLevel = level; 
	
	// Return if all columns have been covered.
	if (h->getRight() == h) {
		solutions++;
		if (verbose > 1) printSolution();
		return;
	}
	
	Column* c = chooseColumn();
	if (!c) panic("Unable to choose a column because no columns could be found");
	cover(c);
	
	// Cover all columns which had nodes removed from cover(c).
	for (Node* r = c->getDown(); r != c; r = r->getDown()) {
		o[level] = r;
		
		for (Node* j = r->getRight(); j != r; j = j->getRight())
			cover(j->getColumn());
		
		level++;
		search();
		level--;
		// r = o[k];
		
		c = r->getColumn();
		
		for (Node* j = r->getLeft(); j != r; j = j->getLeft())
			uncover(j->getColumn());
	}
	
	uncover(c);
}

/**
 * Set the verbose level. Higher means more output. Default is 1.
 */
void setVerboseLevel(uint level) {
	verbose = level;
}

uint getUpdates() {
	return updates;
}

uint getMaxLevel() {
	return maxLevel;
}

uint getProfile(uint level) {
	return profile[level];
}

uint countSolutions() {
	return solutions;
}

/**
 * Returns the number of columns currently in the linked list. This is the
 * number of uncovered columns.
 */
uint countColumns() {
	uint cols = 0;
	for (Node* c = h->getRight(); c != h; c = c->getRight())
		cols++;
	return cols;
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
	search();
	return 0;
}


} /*namespace dlx*/

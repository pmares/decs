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
bool heuristic = true;

using namespace std;

/**
 * Print the solutions.
 */
void printSolution() {
	cout << "Solution: ";
	for (uint i = 0; i <= level; i++) {
		cout << o[i]->getRow() << " ";
	}
	cout << '\n';
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
			if (heuristic) j->getColumn()->decrementSize();
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
			if (heuristic) j->getColumn()->incrementSize();
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
		if (j->getColumn()->getSize() < s) {
			c = j->getColumn();
			s = c->getSize();
		}
	}
	return c;
}

/**
 * Search through the remaining matrix for a solution.
 * Initially invoked when level = 0.
 */
void search() {
	Column* c;
	if (heuristic) {
		c = chooseColumn();
	} else {
		c = h->getRight()->getColumn();
	}
	if (!c) panic("Unable to choose a column because no columns could be found");
//	if (c->getSize() == 0) {
//		uncover(c);
//		return;
//	}
	if (level > maxLevel) maxLevel = level; 
	cover(c);
	
	for (Node* r = c->getDown(); r != c; r = r->getDown()) {
		o[level] = r;
		
		// Cover all columns which had nodes removed from cover(c).
		for (Node* j = r->getRight(); j != r; j = j->getRight())
			cover(j->getColumn());
		
		// Do we have a solution?
		if (h->getRight() == h) {
			solutions++;
			if (verbose > 1) printSolution();
		} else {
			level++;
			search();
			level--;
		}
		
		// Uncover all columns which had nodes removed from cover(c).
		for (Node* j = r->getLeft(); j != r; j = j->getLeft())
			uncover(j->getColumn());
	}
	uncover(c);
}

/**
 * Set the verbose level. Higher equals more output. Default: 1.
 */
void setVerboseLevel(uint level) {
	verbose = level;
}

void setHeuristic(bool value) {
	heuristic = value;
}

bool getHeuristic() {
	return heuristic;
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
 * Returns the number of columns currently in the linked list. Covered columns
 * and secondary columns will not be counted.
 */
uint countColumns() {
	uint cols = 0;
	for (Node* c = h->getRight(); c != h; c = c->getRight()) cols++;
	return cols;
}

/**
 * Deallocate space used for the matrix data structure and reset global
 * variables.
 */
void cleanup() {
	solutions = updates = maxLevel = level = 0;
	
	for (uint i = 0; i < MAX_LEVELS; i++) {
		o[i] = 0;
		profile[i] = 0;
	}
	
	if (!h) return;
	
	for (Node* c = h->getLeft(); c != h ; c = h->getLeft()) {
		for (Node* r = c->getUp(); r != c; r = c->getUp()) {
			r->unlinkColumn();
			delete r;
		}
		c->unlinkRow();
		delete c;
	}
	delete h;
}

/**
 * Read from a file and solve the DLX matrix within.
 */
int solve(char* file) {
	if (h) cleanup();
	
	// Verify the file format and create the column and node objects.
	h = new Column();
	int result = read_file(file, h, verbose);
	if (result) return result;
	
	// Do the dance.
	search();
	return 0;
}


} /*namespace dlx*/

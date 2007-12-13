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
#include "sbnode.h"
#include "sbcolumn.h"
#include "sbmatrix.h"

SBMatrix* m = 0;
SBColumn* h = 0;
SBNode* o[MAX_LEVELS];
uint solutions = 0;
uint verbose = 1;
uint updates = 0;
uint profile[MAX_LEVELS];
uint maxLevel = 0;
uint level = 0;
bool heuristic = true;
bool working = false;

using namespace std;


/**
 * Write a message to stderr and exit with an exit code of 1 to indicate failure.
 * TODO: Remove this as it is only temporary.
 */
void panic(const char* msg) {
	cerr << "Error: " << msg << endl;
	exit(1);
}

/**
 * Print the solutions.
 */
void print_solution() {
	cout << "Solution: ";
	for (uint i = 0; i <= level; i++) {
		cout << o[i]->getRow() << " ";
	}
	cout << '\n';
}

/**
 * Cover column c.
 */
inline void cover(SBColumn* c) {
	c->unlinkRow();
	uint k = 1;
	for (SBNode* i = c->getDown(); i != c; i = i->getDown()) {
		for (SBNode* j = i->getRight(); j != i; j = j->getRight()) {
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
inline void uncover(SBColumn* c) {
	for (SBNode* i = c->getUp(); i != c; i = i->getUp()) {
		for (SBNode* j = i->getLeft(); j != i; j = j->getLeft()) {
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
inline SBColumn* choose_column() {
	SBColumn* c = NULL;
	uint s = UINT_MAX;
	
	for (SBNode* j = h->getRight(); j != h; j = j->getRight()) {
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
	SBColumn* c;
	if (heuristic) {
		c = choose_column();
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
	
	for (SBNode* r = c->getDown(); r != c; r = r->getDown()) {
		o[level] = r;
		
		// Cover all columns which had nodes removed from cover(c).
		for (SBNode* j = r->getRight(); j != r; j = j->getRight())
			cover(j->getColumn());
		
		// Do we have a solution?
		if (h->getRight() == h) {
			solutions++;
			if (verbose > 1) print_solution();
		} else {
			level++;
			search();
			level--;
		}
		
		// Uncover all columns which had nodes removed from cover(c).
		for (SBNode* j = r->getLeft(); j != r; j = j->getLeft())
			uncover(j->getColumn());
	}
	uncover(c);
}

/**
 * Initialize the search with an initialization vector (partial solution) and
 * do the actual searching.
 */
void psearch() {
	SBColumn* c;
	
	uint depth = m->getInitVectorSize();
	uint* initv = m->getInitVector();
	for (uint k = 0; k < depth; k++) {
		if (heuristic) {
			c = choose_column();
		} else {
			c = h->getRight()->getColumn();
		}
		if (!c) panic("Unable to choose a column because no columns could be found");
		cover(c);
		
		o[k] = 0;
		SBNode* r;
		
		for (r = c->getDown(); r != c; r = r->getDown()) {
			if (r->getRow() == initv[k]) {
				o[k] = r;
				break;
			}
		}
		
		if (!o[k]) panic("Unable to choose the correct row from the initialization vector");
		
		// Cover all columns which had nodes removed from cover(c).
		for (SBNode* j = r->getRight(); j != r; j = j->getRight())
			cover(j->getColumn());
	}
	
	solutions = updates = 0;  // Ignore changes made in this procedure.
	level = depth;
	search();
	level = 0;
}


/**
 * Set the verbose level. Higher equals more output. Default: 1.
 */
void dlx_set_verbose_level(uint level) {
	verbose = level;
}

void dlx_set_heuristic(bool value) {
	heuristic = value;
}

bool dlx_get_heuristic() {
	return heuristic;
}

uint dlx_get_updates() {
	return updates;
}

uint dlx_get_max_level() {
	return maxLevel;
}

uint dlx_get_profile(uint level) {
	return profile[level];
}

uint dlx_count_solutions() {
	return solutions;
}


/**
 * Returns the number of columns currently in the linked list. Covered columns
 * and secondary columns will not be counted.
 */
uint dlx_count_columns() {
	uint cols = 0;
	for (SBNode* c = h->getRight(); c != h; c = c->getRight())
		cols++;
	return cols;
}

bool dlx_working() {
	return working;
}

/**
 * Deallocate space used for the matrix data structure and reset global
 * variables.
 */
void cleanup() {
//	util_cleanup();
	
	solutions = updates = maxLevel = level = 0;
	
	for (uint i = 0; i < MAX_LEVELS; i++) {
		o[i] = 0;
		profile[i] = 0;
	}
	
// This should be done by the caller since that's where the matrix was created.
//	if (!h) return;
//	
//	for (SBNode* c = h->getLeft(); c != h ; c = h->getLeft()) {
//		for (SBNode* r = c->getUp(); r != c; r = c->getUp()) {
//			r->unlinkColumn();
//			delete r;
//		}
//		c->unlinkRow();
//		delete c;
//	}
//	delete h;
//	delete m;
}


/**
 * Read from a file and solve the DLX matrix within.
 */
int dlx_solve(SBMatrix* matrix) {
	if (h) cleanup();
	
	m = matrix;
	h = m->getRoot();
	
	// Do the dance.
	working = true;
	if (m->getInitVectorSize() == 0)
		search();
	else
		psearch();
	working = false;
	
	return 0;
}

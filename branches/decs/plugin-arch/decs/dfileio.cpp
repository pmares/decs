/**
 * libdecs - Exact cover solver library using the Dancing Links algorithm.
 * Copyright (C) 2007-2009 Jan Magne Tjensvold
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

#include <cstdio>
#include <iostream>
#include <fstream>

#include "decs.pb.h"
#include "dfileio.h"
#include "dformat.h"
#include "sbmatrix.h"
#include "sbnode.h"
#include "sbcolumn.h"


char* err_msg = 0;
DFIOError err_last = DFIO_ERR_SUCCESS;

decs::DataFile* data_file;

uint dfio_set_last_err(DFIOError err, uint exitval) {
	err_last = err;
	return exitval;
}


uint dfio_read_matrix(SBMatrix* matrix, uint index) {
	// This code could be in the constructor if this was a class.
	// In that case remember to add proper cleanup code to the destructor.
	//
	// Example:
	//
	// FILE* f = fopen(file, "rb");  // BOINC/bdance (extra "b" only needed in windows)
	// SBColumn* header = sb_from_file(FILE* f);
	// ...
	
	if (data_file->problem_size() < index + 1) return 0;
	const decs::DataFile::Problem& problem = data_file->problem(index);

	SBColumn* root = new SBColumn();
	matrix->setRoot(root);

	uint cols = problem.column_count();
	uint rows = problem.row_size();
	uint elems = problem.element_count();

	if (cols == 0) return (err_last = DFIO_ERR_OOB_COLUMNS);
	if (rows == 0) return (err_last = DFIO_ERR_OOB_ROWS);
	if (elems > cols * rows || elems < rows) return (err_last = DFIO_ERR_OOB_ELEMS);
	
	
	// Read the initialization vector, if available.
	uint init_size = problem.init_vector_size();
	if (init_size > 0) {
		uint* initv = new uint[init_size];

		// Read all initialization vector data.
		for (uint i = 0; i < init_size; i++) {
			initv[i] = problem.init_vector(i);
		}
		
		// Validate data.
		for (uint i = 0; i < init_size; i++) {
			if (i > 0 && initv[i] <= initv[i-1])
				return (err_last = DFIO_ERR_ROW_UNSORTED);
		}
		matrix->setInitVector(initv, init_size);
	}

	
	// Read the secondary column list if available.
	uint secol_size = problem.secondary_column_size();
	if (secol_size > cols) return (err_last = DFIO_ERR_OOB_COLUMNS);
	uint* secol = new uint[secol_size];
	if (secol_size > 0) {
		// Read all secondary column data.
		for (uint i = 0; i < secol_size; i++) {
			secol[i] = problem.secondary_column(i);
		}
		
		// Validate data.
		for (uint i = 0; i < secol_size; i++) {
			if (secol[i] >= cols) return (err_last = DFIO_ERR_OOB_COL_IDX);
			if (i > 0 && secol[i] <= secol[i-1]) return (err_last = DFIO_ERR_COL_UNSORTED);
		}
	}
	
	uint nextcol = 0;
	uint secol_index = 0;
	bool secol_ready = false;
	if (secol_size > 0) {
		nextcol = secol[0];
		secol_ready = true;
	}
	
	// Create the circular doubly-linked list of columns.
	SBColumn** he = new SBColumn*[cols];
	SBColumn* t = root;
	for (uint i = 0; i < cols; i++) {
		SBColumn* c = new SBColumn(i);
		he[i] = c;
		if (!secol_ready || nextcol != i) {  // Primary column test.
			c->setLeft(t);
			t->setRight(c);
			t = c;
		} else {  // Secondary column.
			c->setLeft(c);
			c->setRight(c);
			secol_index++;
			if (secol_index < secol_size) {
				nextcol = secol[secol_index];
			} else {
				secol_ready = false;
			}
		}
	}
	matrix->setHeaders(he, cols);
	root->setLeft(t);
	t->setRight(root);
	delete[] secol;
	

	// Create the circular quad-linked matrix structure.
	uint itemstot = 0;
	for (uint i = 0; i < rows; i++) {
		decs::DataFile::ElementList elements = problem.row(i);
		SBNode* u = 0; // instead of t
		uint items = elements.element_size();
		itemstot += items;
		
		if (items > cols) return (err_last = DFIO_ERR_OOB_COLUMNS);
		if (itemstot > elems) return (err_last = DFIO_ERR_OOB_ELEMS);
		
		uint tmpcol = 0;
		for (uint j = 0; j < items; j++) {
			uint column = elements.element(j);
			if (column >= cols) return (err_last = DFIO_ERR_OOB_COL_IDX);
			if (j > 0 && column <= tmpcol) return (err_last = DFIO_ERR_COL_UNSORTED);
			
			tmpcol = column;
			SBNode* n = new SBNode(i);
			SBColumn* c = he[column];
			
			n->setColumn(c);
			c->incrementSize();
			
			n->setUp(c->getUp());
			n->setDown(c);
			c->getUp()->setDown(n);
			c->setUp(n);
			
			if (u) {
				n->setLeft(u);
				n->setRight(u->getRight());
				u->getRight()->setLeft(n);
				u->setRight(n);
			} else {
				n->setLeft(n);
				n->setRight(n);
			}
			u = n;
		}
	}

	if (itemstot < elems) return (err_last = DFIO_ERR_OOB_ELEMS);
	
	return DFIO_ERR_SUCCESS;
}

uint dfio_set_data_file(decs::DataFile* df) {
	data_file = df;
	return DFIO_ERR_SUCCESS;
}

uint dfio_cleanup() {
	data_file = 0;
	return DFIO_ERR_SUCCESS;
}

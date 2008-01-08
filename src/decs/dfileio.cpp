/**
 * libdecs - Exact cover solver library using the Dancing Links algorithm.
 * Copyright (C) 2007-2008 Jan Magne Tjensvold
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

#include "dfileio.h"
#include "dformat.h"
#include "sbmatrix.h"
#include "sbnode.h"
#include "sbcolumn.h"


char* err_msg = 0;
DFIOError err_last = DFIO_ERR_SUCCESS;
FILE* file = 0;
DFIOCompat cmode = DFIO_COMP_UNKNOWN;
DFIOFileType file_type = DFIO_TYPE_UNKNOWN;

// Header cache.
bool dirty_tophead = true;  // Used to determine primary header cache status.
bool dirty_lowhead = true;  // Used to determine secondary header cache status.
DFIOHeader* header = 0;          // Primary header.
DFIOMatrixHeader* mheader = 0;   // Secondary matrix header.
DFIOResultHeader* rheader = 0;   // Secondary result header.


uint dfio_set_last_err(DFIOError err, uint exitval) {
	err_last = err;
	return exitval;
}


uint dfio_write_headers() {
	// Write the main header.
	fseek(file, 0, SEEK_SET);
	if (!header) return DFIO_ERR_NO_DATA;
	DFIOHeader* head = new DFIOHeader((*header));
	dfio_bswap_header(head);
	fwrite(head, sizeof(DFIOHeader), 1, file);
	delete head;
	dirty_tophead = false;

	// Write type specific header.
	fseek(file, sizeof(DFIOHeader), SEEK_SET);
	if (file_type == DFIO_TYPE_MATRIX) {
		if (!mheader) return DFIO_ERR_NO_DATA;
		DFIOMatrixHeader* mhead = new DFIOMatrixHeader((*mheader));
		dfio_bswap_matrix_header(mhead);
		fwrite(mhead, sizeof(DFIOMatrixHeader), 1, file);
		delete mhead;
		dirty_lowhead = false;
	} else if (file_type == DFIO_TYPE_RESULT) {
		if (!rheader) return DFIO_ERR_NO_DATA;
		DFIOResultHeader* rhead = new DFIOResultHeader((*rheader));
		dfio_bswap_result_header(rhead);
		fwrite(rhead, sizeof(DFIOResultHeader), 1, file);
		delete rhead;
		dirty_lowhead = false;
	} else {
		return DFIO_ERR_FILE_TYPE;
	}

	return DFIO_ERR_SUCCESS;

}


uint dfio_read_header() {
	fseek(file, 0, SEEK_SET);
	if (!header) header = new DFIOHeader();
	fread(header, sizeof(DFIOHeader), 1, file);
	dirty_tophead = false;
	dfio_bswap_header(header);
	return DFIO_ERR_SUCCESS;
}


uint dfio_read_matrix_header() {
	if (file_type != DFIO_TYPE_MATRIX) return (err_last = DFIO_ERR_FILE_TYPE);
	fseek(file, sizeof(DFIOHeader), SEEK_SET);
	if (!mheader) mheader = new DFIOMatrixHeader();
	fread(mheader, sizeof(DFIOMatrixHeader), 1, file);
	dirty_lowhead = false;
	dfio_bswap_matrix_header(mheader);
	return DFIO_ERR_SUCCESS;
}


uint dfio_read_result_header() {
	if (file_type != DFIO_TYPE_RESULT) return (err_last = DFIO_ERR_FILE_TYPE);
	fseek(file, sizeof(DFIOHeader), SEEK_SET);
	if (!rheader) rheader = new DFIOResultHeader();
	fread(rheader, sizeof(DFIOResultHeader), 1, file);
	dirty_lowhead = false;
	dfio_bswap_result_header(rheader);
	return DFIO_ERR_SUCCESS;
}


uint dfio_new_file(FILE* f, DFIOFileType type) {
	if (file) dfio_cleanup();
	if (!f) return (err_last = DFIO_ERR_NO_FILE);
	file = f;

	header = new DFIOHeader();
	header->id = DFIO_FILE_ID;
	header->version = DFIO_FILE_VERSION;
	header->minor = DFIO_FILE_MINOR;
	header->reserved = 0;
	dirty_tophead = true;

	if (type == DFIO_TYPE_MATRIX) {
		header->type = 0;
		file_type = DFIO_TYPE_MATRIX;
		mheader = new DFIOMatrixHeader();
		dirty_lowhead = true;
	} else if (type == DFIO_TYPE_RESULT) {
		header->type = 1;
		file_type = DFIO_TYPE_RESULT;
		rheader = new DFIOResultHeader();
		dirty_lowhead = true;
	} else {
		file_type = DFIO_TYPE_UNKNOWN;
		dfio_cleanup();
		return DFIO_ERR_FILE_TYPE;
	}

	dfio_write_headers();

	return DFIO_ERR_SUCCESS;
}


uint dfio_load_file(FILE* f) {
	if (file) dfio_cleanup();
	if (!f) return (err_last = DFIO_ERR_NO_FILE);
	
	file = f;
	uint result = dfio_read_header();
	if (result != DFIO_ERR_SUCCESS) return result;
	if (header->id != DFIO_FILE_ID) return (err_last = DFIO_ERR_FILE_ID);
	if (header->version != DFIO_FILE_VERSION) return (err_last = DFIO_ERR_FILE_VERSION);
	cmode = DFIO_COMP_NATIVE;

	// Process based on file type.
	if (header->type == 0 ) {  //DFIO_TYPE_MATRIX) { TODO: Fix for version 3.
		file_type = DFIO_TYPE_MATRIX;
		result = dfio_read_matrix_header();
		if (result != DFIO_ERR_SUCCESS) return result;
	} else if (header->type == 1) { //DFIO_TYPE_RESULT) { TODO: Fix for version 3.
		file_type = DFIO_TYPE_RESULT;
		result = dfio_read_result_header();
		if (result != DFIO_ERR_SUCCESS) return result;
	} else {
		file_type = DFIO_TYPE_UNKNOWN;
		return (err_last = DFIO_ERR_FILE_TYPE);
	}

	return DFIO_ERR_SUCCESS;
}


uint dfio_cleanup() {
	if (err_msg) delete err_msg;
	err_msg = 0;
	err_last = DFIO_ERR_SUCCESS;
	file = 0;
	file_type = DFIO_TYPE_UNKNOWN;
	cmode = DFIO_COMP_UNKNOWN;

	if (header) delete header;
	if (mheader) delete mheader;
	if (rheader) delete rheader;
	header = 0;
	mheader = 0;
	rheader = 0;
	dirty_tophead = true;
	dirty_lowhead = true;

	return DFIO_ERR_SUCCESS;
}


DFIOError dfio_last_error() {
	return err_last;
}


char* dfio_error_msg() {
	return err_msg;
}


uint dfio_read_uint_next() {
	uint value = 0;
	fread(&value, sizeof(value), 1, file);
	return dfio_cbswap_uint(value);
}


uint dfio_read_uint(long offset) {
	if (offset >= 0) fseek(file, offset, SEEK_SET);
	return dfio_read_uint_next();;
}


uint dfio_read_uint_rewind(long offset) {
	long oldpos = ftell(file);
	uint value = dfio_read_uint(offset);
	fseek(file, oldpos, SEEK_SET);
	return value;
}

/**
 * Check if a specific set of properties are available.
 * prop can be an OR'ed value of the properties to check for.
 * If the returned value is identical to prop then all properties are available.
 */
uint dfio_has_prop(uint prop) {
	uint values = 0;
	if ((prop & DFIO_PROP_FILEID) != 0) {
		if (header != 0) values |= DFIO_PROP_FILEID;
	}
	if ((prop & DFIO_PROP_VERSION) != 0) {
		if (header != 0) values |= DFIO_PROP_VERSION;
	}
	if ((prop & DFIO_PROP_COMPAT) != 0) {
		if (header != 0) values |= DFIO_PROP_COMPAT;
	}
	if ((prop & DFIO_PROP_COLS_NUM) != 0) {
		if (mheader != 0) values |= DFIO_PROP_COLS_NUM;
	}
	if ((prop & DFIO_PROP_ROWS_NUM) != 0) {
		if (mheader != 0) values |= DFIO_PROP_ROWS_NUM;
	}
	if ((prop & DFIO_PROP_ELEMS_NUM) != 0) {
		if (mheader != 0) values |= DFIO_PROP_ELEMS_NUM;
	}
	if ((prop & DFIO_PROP_ELEMS_OFF) != 0) {
		if (mheader != 0) values |= DFIO_PROP_ELEMS_OFF;
	}
	if ((prop & DFIO_PROP_SECOLS_NUM) != 0) {
		if (mheader != 0) values |= DFIO_PROP_SECOLS_NUM;
	}
	if ((prop & DFIO_PROP_SECOLS_NUM) != 0) {
		if (mheader != 0) values |= DFIO_PROP_SECOLS_NUM;
	}
	if ((prop & DFIO_PROP_SECOLS_OFF) != 0) {
		if (mheader != 0) values |= DFIO_PROP_SECOLS_OFF;
	}
	if ((prop & DFIO_PROP_INIT_OFF) != 0) {
		if (mheader != 0) values |= DFIO_PROP_INIT_OFF;
	}
	if ((prop & DFIO_PROP_INIT_NUM) != 0) {
		if (mheader != 0) values |= DFIO_PROP_INIT_NUM;
	}
	if ((prop & DFIO_PROP_NAMES_OFF) != 0) {
		if (mheader != 0) values |= DFIO_PROP_NAMES_OFF;
	}
	if ((prop & DFIO_PROP_PROB_ID) != 0) {
		if (mheader != 0 || rheader != 0) values |= DFIO_PROP_PROB_ID;
	}
	if ((prop & DFIO_PROP_PROB_OFF) != 0) {
		if (mheader != 0 || rheader != 0) values |= DFIO_PROP_PROB_OFF;
	}
	if ((prop & DFIO_PROP_RESULTS_NUM) != 0) {
		if (rheader != 0) values |= DFIO_PROP_RESULTS_NUM;
	}
	if ((prop & DFIO_PROP_RESULTS_OFF) != 0) {
		if (rheader != 0) values |= DFIO_PROP_RESULTS_OFF;
	}

	// Not specified and implemented in this version.
//	DFIO_PROP_COL_NAMES_NUM = 0x400,
//	DFIO_PROP_ROW_NAMES_NUM = 0x800,

	return values;
};


uxlong dfio_get_prop(DFIOProperty prop) {
	if ((dfio_has_prop(prop) & (uint)prop) != (uint)prop) {
		err_last = DFIO_ERR_NO_PROP;
		return 0;
	}

	uxlong value = 0;

	switch (prop) {
		case DFIO_PROP_FILEID: return header->id;
		case DFIO_PROP_VERSION: return header->version;
		case DFIO_PROP_COMPAT: return header->minor;
		case DFIO_PROP_COLS_NUM: return mheader->cols;
		case DFIO_PROP_ROWS_NUM: return mheader->rows;
		case DFIO_PROP_ELEMS_NUM: return mheader->elems;
		case DFIO_PROP_ELEMS_OFF:  // Because secols is treaded as part elems in version 1 and 2. TODO: Fix for v3.
			return mheader->elem_off + (dfio_get_prop(DFIO_PROP_SECOLS_NUM) + 1) * sizeof(uint);
		case DFIO_PROP_SECOLS_NUM:
			value = dfio_get_prop(DFIO_PROP_SECOLS_OFF);
			if (!value) return dfio_set_last_err(DFIO_ERR_NO_PROP, 0);
			return dfio_read_uint_rewind((long)value);
		case DFIO_PROP_SECOLS_OFF: return mheader->elem_off;  // Treat as part of elems in version 1 and 2. TODO: Fix for v3.
		case DFIO_PROP_INIT_OFF: return mheader->init_off;
		case DFIO_PROP_INIT_NUM:
			value = dfio_get_prop(DFIO_PROP_INIT_OFF);
			if (!value) return dfio_set_last_err(DFIO_ERR_NO_PROP, 0);
			return dfio_read_uint_rewind((long)value);
		case DFIO_PROP_NAMES_OFF: return mheader->name_off;
		case DFIO_PROP_PROB_ID: 
			if (file_type == DFIO_TYPE_MATRIX) {
				return mheader->probid;
			} else if (file_type == DFIO_TYPE_RESULT) {
				return rheader->probid;
			}
			break;
		case DFIO_PROP_PROB_OFF: 
			if (file_type == DFIO_TYPE_MATRIX) {
				return mheader->prob_off;
			} else if (file_type == DFIO_TYPE_RESULT) {
				return rheader->prob_off;
			}
			break;
		case DFIO_PROP_RESULTS_NUM: return rheader->results;
		case DFIO_PROP_RESULTS_OFF: return rheader->result_off;
	}

	return dfio_set_last_err(DFIO_ERR_NO_PROP, 0);
}


uint dfio_set_prop(DFIOProperty prop, uxlong value) {
	if ((dfio_has_prop(prop) & (uint)prop) != (uint)prop) {
		err_last = DFIO_ERR_NO_PROP;
		return 0;
	}

	switch (prop) {
		case DFIO_PROP_RESULTS_NUM:
			rheader->results = value;
			dirty_lowhead = true;
			break;
		default: dfio_set_last_err(DFIO_ERR_READ_ONLY, 0);
	}

	return DFIO_ERR_SUCCESS;
}


uint dfio_comp_mode() {
	return cmode;
}


DFIOFileType dfio_file_type() {
	return file_type;
}


void dfio_read_uints(uint* ary, uint number) {
	fread(ary, sizeof(uint), number, file);
	dfio_bswap_uints(ary, number);
}


uint dfio_read_matrix(SBMatrix* matrix) {
	// This code could be in the constructor if this was a class.
	// In that case remember to add proper cleanup code to the destructor.
	//
	// Example:
	//
	// FILE* f = fopen(file, "rb");  // BOINC/bdance (extra "b" only needed in windows)
	// SBColumn* header = sb_from_file(FILE* f);
	// ...
	
	if (file_type != DFIO_TYPE_MATRIX) return 0;

	SBColumn* root = new SBColumn();
	matrix->setRoot(root);

	// Check if all the properties are available.
	uint props = DFIO_PROP_COLS_NUM | DFIO_PROP_ROWS_NUM | DFIO_PROP_ELEMS_NUM
		| DFIO_PROP_SECOLS_NUM | DFIO_PROP_SECOLS_OFF | DFIO_PROP_ELEMS_OFF
		| DFIO_PROP_INIT_OFF;
	if (dfio_has_prop(props) != props)
		return (err_last = DFIO_ERR_NO_PROP);
	
	uint cols = (uint)dfio_get_prop(DFIO_PROP_COLS_NUM);
	uint rows = (uint)dfio_get_prop(DFIO_PROP_ROWS_NUM);
	uint elems = (uint)dfio_get_prop(DFIO_PROP_ELEMS_NUM);
	uint elem_off = (uint)dfio_get_prop(DFIO_PROP_ELEMS_OFF);

	if (cols == 0) return (err_last = DFIO_ERR_OOB_COLUMNS);
	if (rows == 0) return (err_last = DFIO_ERR_OOB_ROWS);
	if (elems > cols * rows || elems < rows) return (err_last = DFIO_ERR_OOB_ELEMS);
	if (elem_off == 0) return (err_last = DFIO_ERR_NO_DATA);
	
	
	// Read the initialization vector, if available.
	uint init_size = (uint)dfio_get_prop(DFIO_PROP_INIT_NUM);
	uint* initv = new uint[init_size];

	uint init_off = (uint)dfio_get_prop(DFIO_PROP_INIT_OFF);
	if (init_off) {
		// Read all initialization vector data.
		fseek(file, init_off + sizeof(uint), SEEK_SET);
		dfio_read_uints(initv, init_size);
		
		// Validate data.
		for (uint i = 0; i < init_size; i++) {
			if (i > 0 && initv[i] <= initv[i-1])
				return (err_last = DFIO_ERR_ROW_UNSORTED);
		}
		matrix->setInitVector(initv, init_size);
	}

	
	// Read the secondary column list if available.
	uint secol_size = (uint)dfio_get_prop(DFIO_PROP_SECOLS_NUM);
	if (secol_size > cols) return (err_last = DFIO_ERR_OOB_COLUMNS);
	uint* secol = new uint[secol_size];

	uint secol_off = (uint)dfio_get_prop(DFIO_PROP_SECOLS_OFF);
	if (secol_off) {
		// Read all secondary column data.
		fseek(file, secol_off + sizeof(uint), SEEK_SET);
		dfio_read_uints(secol, secol_size);
		
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
	fseek(file, elem_off, SEEK_SET);
	uint itemstot = 0;
	for (uint i = 0; i < rows; i++) {
		SBNode* u = 0; // instead of t
		uint items = dfio_read_uint_next();
		itemstot += items;
		
		if (items > cols) return (err_last = DFIO_ERR_OOB_COLUMNS);
		if (itemstot > elems) return (err_last = DFIO_ERR_OOB_ELEMS);
		
		uint* rowdata = new uint[items];
		dfio_read_uints(rowdata, items);  // Read all row data.
		
		uint tmpcol = 0;
		for (uint j = 0; j < items; j++) {
			uint column = rowdata[j];
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
		delete[] rowdata;
	}

	if (itemstot < elems) return (err_last = DFIO_ERR_OOB_ELEMS);
	
	return DFIO_ERR_SUCCESS;
}

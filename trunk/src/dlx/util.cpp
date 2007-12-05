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

#include <fstream>
#include <iostream>
#include <cstdlib>
#include "util.h"
#include "column.h"
#include "node.h"
#include "dlx.h"

namespace dlx {

using namespace std;

struct FileHeader {
	uint fileid;
	byte version;
	byte compat;
	byte reserved[2];
	uint cols;
	uint rows;
	uint elems;
	uint elem_off;
	uint init_off;
	uint name_off;
	uint probid;
	uint prob_off;
};

uint* initv = 0;
uint init_size = 0;

uint initv_size() {
	return init_size;
}

uint get_initv(uint index) {
	return initv[index];
}

/**
 * Write a message to stderr and exit with an exit code of 1 to indicate failure.
 */
void panic(const char* msg) {
	cerr << "Error: " << msg << endl;
	exit(1);
}

void util_cleanup() {
	if (initv) delete[] initv;
	init_size = 0;
}

int read_file(char* file, Column* header, uint verbose, bool &init) {
	ifstream in(file, ios::binary);
	if (!in) return ERR_FILE_OPEN;
	
	FileHeader fh;
	in.read(reinterpret_cast<char *>(&fh),sizeof(FileHeader));
	
	if (verbose > 1) { 
		cout << "File information" << '\n';
//		cout << fh.fileid << '\n';
		cout << "  version = " << int(fh.version) << '\n';
		cout << "  compat = " << int(fh.compat) << '\n';
		cout << "  columns = " << fh.cols << '\n';
		cout << "  rows = " << fh.rows << '\n';
		cout << "  elements = " << fh.elems << '\n';
		cout << "  density = " << ( (double)fh.elems / ((double)fh.rows / (1.0 / (double)fh.cols) ) ) << '\n';
		cout << "  element offset = " << fh.elem_off << '\n';
		cout << "  init offset = " << fh.init_off << '\n';
		cout << "  name offset = " << fh.name_off << '\n';
		cout << "  problem id = " << fh.probid << '\n';
		cout << "  problem offset = " << fh.prob_off << '\n' << endl;
	}
	
	if (fh.fileid != FILE_ID) return ERR_FILE_ID;
	if (fh.version != FILE_VERSION) return ERR_FILE_VERSION;
	if (fh.elems > fh.cols * fh.rows) return ERR_ELEMS_OOB;
	
	
	// Read the initialization vector, if available.
	init_size = 0;
	if (fh.init_off > 0) {
		in.seekg(fh.init_off);
		in.read(reinterpret_cast<char *>(&init_size),sizeof(init_size));
	}
	
	if (verbose > 1) {
		cout << "Found " << init_size << " initializer rows";
		if (init_size > 0) cout << ": ";
	}
	
	initv = new uint[init_size];
	init = (init_size > 0);
	
	for (uint i = 0; i < init_size; i++) {
		in.read(reinterpret_cast<char *>(&initv[i]),sizeof(initv[i]));
		if (i > 0 && initv[i] <= initv[i-1]) return ERR_ROW_UNSORTED;
		if (verbose > 1) cout << initv[i] << " ";			
	}	
	if (verbose > 1) cout << endl;
	
	
	// Read the secondary column list if available.
	in.seekg(fh.elem_off);
	uint secol_size;
	in.read(reinterpret_cast<char *>(&secol_size),sizeof(secol_size));
	if (secol_size > fh.cols) return ERR_COL_COUNT_OOB;  // TODO Can the number of secondary columns be equal to the total number of columns?
	
	if (verbose > 1) {
		cout << "Found " << secol_size << " secondary columns";
		if (secol_size > 0) cout << ": ";
	}
	
	uint* secol = new uint[secol_size];
	
	for (uint i = 0; i < secol_size; i++) {
		in.read(reinterpret_cast<char *>(&secol[i]),sizeof(secol[i]));
		if (secol[i] >= fh.cols) return ERR_COL_IDX_OOB;
		if (i > 0 && secol[i] <= secol[i-1]) return ERR_COL_UNSORTED;
		if (verbose > 1) cout << secol[i] << " ";
	}
	if (verbose > 1) cout << endl;
	
	uint nextcol = 0;
	uint secol_index = 0;
	bool secol_ready = false;
	if (secol_size > 0) {
		nextcol = secol[0];
		secol_ready = true;
	}
	
	
	// Create the circular doubly-linked list of columns.
	Column** he = new Column*[fh.cols];
	Column* t = header;
	for (uint i = 0; i < fh.cols; i++) {
		Column* c = new Column(i);
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
	header->setLeft(t);
	t->setRight(header);
	delete[] secol;
	
	
	// Create the circular quad-linked matrix structure.
	if (verbose > 1) cout << "Building the circular quad-linked matrix structure" << endl;
	uint itemstot = 0;
	for (uint i = 0; i < fh.rows; i++) {
		Node* u = 0; // instead of t
		
		uint items;
		in.read(reinterpret_cast<char *>(&items),sizeof(items));
		if (items > fh.cols) return ERR_COL_COUNT_OOB;
		itemstot += items;
		if (itemstot > fh.elems) return ERR_ELEMS_OOB;
		
		if (verbose > 1) cout << "  Row " << i << " [size = " << items << "] = ", cout.flush();
		uint tmpcol = 0;
		for (uint j = 0; j < items; j++) {
			Node* n = new Node(i);
			
			uint column;
			in.read(reinterpret_cast<char *>(&column),sizeof(column));
			if (column >= fh.cols) return ERR_COL_IDX_OOB;
			if (j > 0 && column <= tmpcol) return ERR_COL_UNSORTED;
			tmpcol = column;
			
			if (verbose > 1) {
				cout << column << " ";
				cout.flush();
			}
			
			Column* c = he[column];
			
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
		if (verbose > 1) cout << endl;
	}
	if (verbose > 1) cout << endl;
	delete[] he;
	if (itemstot < fh.elems) return ERR_ELEMS_OOB;

	return 0;
}

} /*namespace dlx*/

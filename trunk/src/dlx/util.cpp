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

namespace dlx {

using namespace std;

struct FileHeader {
	uint fileid;
	unsigned char version;
	unsigned char compat;
	char reserved[2];
	uint cols;
	uint rows;
	uint elems;
	uint elem_off;
	uint name_off;
	uint probid;
	uint prob_off;
};

/**
 * Write a message to stderr and exit with an exit code of 1 to indicate failure.
 */
void panic(const char* msg) {
	cerr << "Error: " << msg << endl;
	exit(1);
}

int read_file(char* file, Column* header, uint verbose) {
	ifstream in(file, ios::binary);
	if (!in) panic("Cannot open file");
	
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
		cout << "  name offset = " << fh.name_off << '\n';
		cout << "  problem id = " << fh.probid << '\n';
		cout << "  problem offset = " << fh.prob_off << "\n\n";
	}

	if (fh.fileid != FILE_ID) panic("Incompatible file ID");
	if (fh.version != FILE_VERSION) panic("Incompatible file version");
	
	
	// Read the secondary column list if available.
	in.seekg(fh.elem_off);
	uint secol_size;
	in.read(reinterpret_cast<char *>(&secol_size),sizeof(secol_size));

	if (verbose > 1) {
		cout << "Found " << secol_size << " secondary columns";
		if (secol_size > 0) cout << ": ";
	}
	
	uint* secol = new uint[secol_size];

	for (uint i = 0; i < secol_size; i++) {
		in.read(reinterpret_cast<char *>(&secol[i]),sizeof(secol[i]));
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
		c->setColumn(c);
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
	delete secol;
	
	
	// Create the circular quad-linked matrix structure.
	if (verbose > 1) cout << "Building the circular quad-linked matrix structure\n";
	for (uint i = 0; i < fh.rows; i++) {
		Node* u = 0; // instead of t
		uint items;
		in.read(reinterpret_cast<char *>(&items),sizeof(items));
		if (verbose > 1) cout << "  Row " << i << " [size = " << items << "] = ";
		for (uint j = 0; j < items; j++) {
			Node* n = new Node(i);
			
			uint column;
			in.read(reinterpret_cast<char *>(&column),sizeof(column));
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
		if (verbose > 1) cout << '\n';
	}
	if (verbose > 1) cout << '\n';
	delete he;

	return 0;
}

} /*namespace dlx*/

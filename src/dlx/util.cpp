#include <fstream>
#include <iostream>
#include <cstdlib>
#include "util.h"
#include "column.h"
#include "node.h"

namespace dlx {

using namespace std;

struct FileHeader {
	char fileid[4];
	char version;
	char compat;
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
	cerr << msg << endl;
	exit(1);
}

int read_file(char* file, Column* header) {
	ifstream in(file, ios::binary);
	if (!in) panic("cannot open file");

	FileHeader fh;
	in.read(reinterpret_cast<char *>(&fh),sizeof(FileHeader));

	cout << "File information" << '\n';
//	cout << fh.fileid << '\n';
	cout << "  version = " << int(fh.version) << '\n';
	cout << "  compat = " << int(fh.compat) << '\n';
	cout << "  columns = " << fh.cols << '\n';
	cout << "  rows = " << fh.rows << '\n';
	cout << "  elements = " << fh.elems << '\n';
	cout << "  element offset = " << fh.elem_off << '\n';
	cout << "  name offset = " << fh.name_off << '\n';
	cout << "  problem id = " << fh.probid << '\n';
	cout << "  problem offset = " << fh.prob_off << "\n\n";
	
	if (fh.fileid[0] != 'D' ||
		fh.fileid[1] != 'E' ||
		fh.fileid[2] != 'C' ||
		fh.fileid[3] != 'S') {
		cerr << "Incompatible file ID." << endl;
		return 1;
	}
	
	
	// Create the circular doubly-linked list of columns.
	Column* he[fh.cols];
	Column* t = header;
	
	for (uint i = 0; i < fh.cols; i++) {
		Column* c = new Column(i);
		he[i] = c;
		c->setColumn(c);
		if (true) { // TODO Add primary column test
			c->setLeft(t);
			t->setRight(c);
			t = c;
		} else {
			c->setLeft(c);
			c->setRight(c);
		}
	}
	header->setLeft(t);
	t->setRight(header);
	
	
	// Create the circular quad-linked matrix structure.
	cout << "Building the circular quad-linked matrix structure" << endl;
	in.seekg(fh.elem_off);
	for (uint i = 0; i < fh.rows; i++) {
		Node* u = 0; // instead of t
		uint items;
		in.read(reinterpret_cast<char *>(&items),sizeof(items));
		cout << "  Row " << i << " [size = " << items << "] = ";
		for (uint j = 0; j < items; j++) {
			Node* n = new Node(i);
			
			uint column;
			in.read(reinterpret_cast<char *>(&column),sizeof(column));
			cout << column << " ";
			cout.flush();
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
		cout << '\n';
	}
	cout << endl;
	
	return 0;
}

} /*namespace dlx*/

/**
 * libnqdecs - DECS transform for the n-queens problem.
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

#include "nqdecs.h"

using namespace std;

struct ProbInfo {
	ubyte version;
	ubyte queens;
	ubyte oporder;  // Is it using organ pipe ordering?
	ubyte reserved[5];
};

const uint PROB_ID = 0x10;

uint queens = 2;
bool organ = false;  // Organ pipe ordering optimization.

/**
 * Write a message to stderr and exit with an exit code of 1 to indicate failure.
 */
void panic(const char* msg) {
	cerr << msg << endl;
	exit(1);
}

/**
 * Set the number of queens. Returns 1 if the set operation failed.
 * The number of queens must be larger than 1 for the operation
 * to succeed.
 */
int nq_set_queens(uint n) {
	if (n > 1) {
		queens = n;
		return 0;
	} else {
		return 1;
	}
}

int nq_set_organ_pipe_order(bool enable) {
	if (enable) return 1;  // Organ pipe ordering currently not supported.
	return 0;
}

uint nq_transform(FILE* file) {
	uint retval = dfio_new_file(file, DFIO_TYPE_MATRIX);
	if (retval != DFIO_ERR_SUCCESS)
		return retval;

	
	fh.cols = 6 * queens - 6;

	// Write problem specific information.
	ProbInfo pi;
	fh.probid = PROB_ID;
	fh.prob_off = sizeof(FileHeader);
	pi.version = LIB_VERSION;
	pi.queens = queens;
	pi.oporder = organ ? 1 : 0;
	pi.reserved[0] = pi.reserved[1] = pi.reserved[2] = pi.reserved[3]
			= pi.reserved[4] = 0;
	out.seekp(fh.prob_off);
	out.write(reinterpret_cast<char *>(&pi),sizeof(ProbInfo));
	
	
	// Begin the element section. 
	fh.elem_off = out.tellp();
	
	// Write the secondary column list.
	uint secols = 4 * queens - 6;
	out.write(reinterpret_cast<char *>(&secols),sizeof(secols));
	for (uint i = 0; i < secols; i++) {
		uint col = 2 * queens + i;
		out.write(reinterpret_cast<char *>(&col),sizeof(col));
	}
	
	uint rows = 0;
	uint elems = 0;
	
	// Corner diagonals to omit.
	const uint VOID_DIAG1 = 0;
	const uint VOID_DIAG2 = 2 * queens - 2;
	
	// Generate the boolean matrix.
	// i and j represents the rank and file respectively.
	for (uint i = 0; i < queens; i++) {      // Rank
		for (uint j = 0; j < queens; j++) {  // File
			uint cols = 4;
			uint a = i + j;
			uint b = queens - 1 - i + j;
			
			if (a == VOID_DIAG1 || a == VOID_DIAG2) cols--;  // A0 and A(2N-2) should be omitted.
			if (b == VOID_DIAG1 || b == VOID_DIAG2) cols--;  // B0 and B(2N-2) should be omitted.
			out.write(reinterpret_cast<char *>(&cols),sizeof(cols));
			
			uint js = j + queens;
			out.write(reinterpret_cast<char *>(&i),sizeof(i));
			out.write(reinterpret_cast<char *>(&js),sizeof(js));

			uint as, bs;
			as = a < VOID_DIAG2 ? a + 2 * queens - 1 : a + 2 * queens - 2;
			bs = b < VOID_DIAG2 ? b + 4 * queens - 4 : b + 4 * queens - 5;
			
			if (a != VOID_DIAG1 && a != VOID_DIAG2)
				out.write(reinterpret_cast<char *>(&as),sizeof(as));
			if (b != VOID_DIAG1 && b != VOID_DIAG2)
				out.write(reinterpret_cast<char *>(&bs),sizeof(bs));
			
			elems += cols;
			rows++;
		}
	}
	
	// Finally write the file header.
	out.seekp(0);
	fh.rows = rows;
	fh.elems = elems;
	out.write(reinterpret_cast<char *>(&fh),sizeof(FileHeader));
	
	return 0;
}


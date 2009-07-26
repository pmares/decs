/**
 * libnqdecs - DECS transform for the n-queens problem.
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

#include "nqdecs.h"
#include "dfileio.h"
#include "decs.pb.h"

using namespace std;

struct ProbInfo {
	ubyte version;
	ubyte queens;
	ubyte oporder;  // Is it using organ pipe ordering?
	ubyte reserved[5];
};

const uint PROB_ID = 0x10;
const uint LIB_VERSION = 2;

uint queens = 2;
bool organ = false;  // Organ pipe ordering optimization.

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

uint nq_transform(char* file) {
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	decs::DataFile data_file;
	decs::DataFile::Problem* problem = data_file.add_problem();

	problem->set_column_count(6 * queens - 6);

	// Write problem specific information.
	// TODO: Convert this to Protocol Buffers
	/**
	ProbInfo pi;
	fh.probid = PROB_ID;
	fh.prob_off = sizeof(DFIOHeader);
	pi.version = LIB_VERSION;
	pi.queens = queens;
	pi.oporder = organ ? 1 : 0;
	pi.reserved[0] = pi.reserved[1] = pi.reserved[2] = pi.reserved[3]
			= pi.reserved[4] = 0;
	out.seekp(fh.prob_off);
	out.write(reinterpret_cast<char *>(&pi),sizeof(ProbInfo));
	*/
	
	
	// Write the secondary column list.
	uint secols = 4 * queens - 6;
	for (uint i = 0; i < secols; i++) {
		uint col = 2 * queens + i;
		problem->add_secondary_column(col);
	}
	
	uint elems = 0;
	
	// Corner diagonals to omit.
	const uint VOID_DIAG1 = 0;
	const uint VOID_DIAG2 = 2 * queens - 2;
	
	// Generate the boolean matrix.
	// i and j represents the rank and file respectively.
	for (uint i = 0; i < queens; i++) {      // Rank
		for (uint j = 0; j < queens; j++) {  // File
			uint a = i + j;
			uint b = queens - 1 - i + j;
			
			//if (a == VOID_DIAG1 || a == VOID_DIAG2) cols--;  // A0 and A(2N-2) should be omitted.
			//if (b == VOID_DIAG1 || b == VOID_DIAG2) cols--;  // B0 and B(2N-2) should be omitted.
			
			uint js = j + queens;
			decs::DataFile::ElementList* row = problem->add_row();
			row->add_element(i);
			row->add_element(js);

			uint as, bs;
			as = a < VOID_DIAG2 ? a + 2 * queens - 1 : a + 2 * queens - 2;
			bs = b < VOID_DIAG2 ? b + 4 * queens - 4 : b + 4 * queens - 5;
			
			if (a != VOID_DIAG1 && a != VOID_DIAG2)
				row->add_element(as);
			if (b != VOID_DIAG1 && b != VOID_DIAG2)
				row->add_element(bs);
			
			elems += row->element_size();
		}
	}
	
	// Finally write the file header.
	problem->set_element_count(elems);
	/*
	out.seekp(0);
	fh.rows = rows;
	fh.elems = elems;
	out.write(reinterpret_cast<char *>(&fh),sizeof(DFIOHeader));
	*/

	// Write to file.
	fstream output(file, ios::out | ios::trunc | ios::binary);
	if (!data_file.SerializeToOstream(&output)) {
		return DFIO_ERR_FILE_WRITE;
	}
	
	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}


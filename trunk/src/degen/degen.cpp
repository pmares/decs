/**
 * degen - Exact cover matrix generator for the DECS library.
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

#include <iostream>
#include "nqdecs.h"
#include "dfileio.h"

const char* VERSION = "0.2";

using namespace std; 

/**
 * Prints command line options.
 */
void print_usage() {
	cout <<
		"Usage:\n"
		"degen [options] [file]\n"
		"degen <transform> [--forward] [transform-options] <outfile>\n"
		"degen [transform] --reverse [transform-options] <infile> [outfile]\n"
		"\n"
		"Options:\n"
		"  --forward          Perform a forward transform (default).\n"
		"  --reverse          Perform a reverse transform.\n"
		"  --transforms       List the available transforms.\n"
		"  --split <level>    Splits a matrix into sub-matrices at the specified\n"
		"                       recursive level and writes output to file_x.decs where x\n"
		"                       goes from 1 to the total number of sub-matrices found.\n"
		"  --help             Print help information and exit.\n"
		"  --version          Print program version and exit.\n"
		"\n"
		"Transform options: \n"
		"F = forward, R = reverse, B = bidirectional (both ways)\n"
		"nqueens transform options:\n"
		"F  --queens <n>      n number of queens\n"
		"\n"
		"Examples:\n"
		"degen --transforms\n"
		"degen nqueens --forward --queens 20 --split 4 queens20.decs\n"
		"degen nqueens --reverse queens8-solutions.decs solutions.txt\n"
		"\n"
		"Report bugs to http://code.google.com/p/decs/issues/list.";
	//   --------------------------------------------------------------------------------
	//   80 chars long line
}

/**
 * Prints version and copyright notice.
 */
void print_version() {
	cout <<
		"degen (DECS toolkit) " << VERSION << "\n"
		"Copyright (C) 2007-2009 Jan Magne Tjensvold\n"
		"This is free software; See the source for copying conditions. There is NO\n"
		"WARRANTY; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.";
}

void print_transforms() {
	cout << "degen (DECS toolkit) " << VERSION << "\n"
		"Transforms:\n"
		"nqueens";
}

void usage_error() {
	print_usage();
	exit(1);
}

int main(int argc, char* argv[]) {
	char* file = 0;
	char* file2 = 0;
	bool forward = true;
	bool reverse = false;

	// Transform specific variables.
	uint queens = 0;
	
	// Interpret command line parameters.
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--help")) {
			print_usage();
			return 0;
		} else if (!strcmp(argv[i], "--version")) {
			print_version();
			return 0;
		} else if (!strcmp(argv[i], "--transforms")) {
			print_transforms();
			return 0;
		} else if (!strcmp(argv[i], "--reverse")) {
			reverse = true;
			forward = false;
		} else if (!strcmp(argv[i], "--forward")) {
			reverse = false;
			forward = true;
			
		/* Transform parameters */
		} else if (!strcmp(argv[i],"--queens")) {
			if (i+1 < argc) {
				i++;
				queens = atoi(argv[i]);
			} else {
				cerr << "'--queens' was not followed by a number\n";
				usage_error();
			}
		} else if (i+2 >= argc) {
			file = argv[i];
		} else if (file != 0 && i+1 == argc) {
			file2 = argv[i];
		} else {
			cerr << "Unknown command line parameter '" << argv[i] << "'\n";
			usage_error();
		}
	}
	
	if (file == NULL) {
		cerr << "No file specified on the command line\n";
		usage_error();
	}
	
	if (forward) {  // Perform forward transformation
		if (nq_set_queens(queens)) {
			cerr << "Failed to set library parameter 'number of queens' = " << queens << '\n';
			usage_error();
		}
		
		cout << "Generating matrix..." << '\n';
		if (nq_transform(file) != DFIO_ERR_SUCCESS) {
			cerr << "Unable to complete forward transform";
			return 1;
		}
		
		cout << "Matrix generated and written to file:\n";
		cout << file;
	} else {  // Perform reverse transformation
		cerr << "Reverse transform not implemented for libnqdecs\n";
		usage_error();
	}
	return 0;
}

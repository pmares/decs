/**
 * Dance - Exact cover solver program for the Dancing Links library.
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

#include <iostream>
#include "dlx.h"

const char* version = "0.2";

using namespace std;
using namespace dlx;

/**
 * Prints command line options.
 */
void print_usage() {
	cout << "Usage: dance [OPTIONS] [FILE]\n"
			"\n"
			"Options:\n"
			"  -v, --verbose      Verbose output.\n"
			"  -q, --quiet        Quiet. Don't output to stdout.\n"
			"  --count-updates    Print the number of link updates.\n"
			"  --profile          Print the link update profile.\n"
			"  --help             Print help information and exit.\n"
			"  --version          Print program version and exit.\n"
			"\n"
			"Report bugs to <http://code.google.com/p/decs/issues/list>.";
}

/**
 * Prints version and copyright notice.
 */
void print_version() {
	cout << "dance (DECS toolkit) " << version << "\n"
			"Copyright (C) 2007 Jan Magne Tjensvold\n"
			"This is free software; See the source for copying conditions. There is NO\n"
			"WARRANTY; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n";
}

void usage_error() {
	print_usage();
	exit(1);
}

int solve_error(uint code) {
	switch (code) {
	case ERR_FILE_OPEN:
		cerr << "Unable to open the specified file";
		return 1;
		break;
	case ERR_FILE_VERSION:
		cerr << "Incompatible file version";
		return 1;
		break;
	case ERR_FILE_ID:
		cerr << "Incompatible file ID";
		return 1;
		break;
	case ERR_COL_IDX_OOB:
		cerr << "Out of bounds column index encountered while reading from file";
		return 1;
		break;
	case ERR_COL_COUNT_OOB:
		cerr << "Out of bounds column count encountered while reading from file";
		return 1;
		break;
	case ERR_COL_UNSORTED:
		cerr << "Unsorted column index encountered while reading from file";
		return 1;
		break;
	case ERR_ELEMS_OOB:
		cerr << "Number of elements read from the file is out of bounds";
		return 1;
		break;
		
 // 

	default:
		cerr << "Unknown error returned by libdlx";
		return 1;
		break;
	}
}

int main(int argc, char* argv[]) {
	uint verbose = 1;
	bool countUpdates = false;
	bool showProfile = false;
	char* file = 0;
	
	// Interpret command line parameters.
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-q") || !strcmp(argv[i], "--quiet")) {
			verbose = 0;
		} else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose")) {
			verbose++;
		} else if (!strcmp(argv[i], "--help")) {
			print_usage();
			return 0;
		} else if (!strcmp(argv[i], "--version")) {
			print_version();
			return 0;
		} else if (!strcmp(argv[i], "--count-updates")) {
			countUpdates = true;
		} else if (!strcmp(argv[i], "--profile")) {
			showProfile = true;
		} else if (i == argc-1) {
			file = argv[i];
		} else {
			cerr << "Unknown command line parameter '" << argv[i] << "'\n";
			usage_error();
		}
	}
	
	if (!file) {
		cerr << "No file specified on the command line\n";
		usage_error();
	}
	
	setVerboseLevel(verbose);
	
	if (verbose > 0) {
		cout << "Dancing to the rhythm of\n" << file << "\n\n";
		cout << "Searching..." << endl;
	}
	
	uint result = solve(file);
	if (result != ERR_SUCCESS) return solve_error(result);
	
	if (verbose > 0) {
		cout << "Search complete\n";
		cout << "\nNumber of solutions: " << countSolutions();
		cout << "\nFinal primary column size: " << countColumns();
	}
	
	uint updates = getUpdates();
	if (countUpdates) cout << "\nTotal link updates: " << updates;
	if (showProfile) {
		cout << "\n\nLink update profile:\n";
		uint maxLevel = getMaxLevel() + 1;
		for (uint i = 0; i < maxLevel; i++) {
			uint upd = getProfile(i);
			cout << i << '\t' << upd << " (" << (double(upd) / double(updates)) * 100.0 << "%)\n"; 
		}
	}
	
	return 0;
}

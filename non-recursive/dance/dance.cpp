/**
 * dance - Exact cover solver program for the Dancing Links library.
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
#include <fstream>
#include <string>

#include "dlx.h"
#include "dfileio.h"
#include "sbmatrix.h"
#include "decs.pb.h"

const char* VERSION = "0.3.0";

using namespace std;

/**
 * Prints command line options.
 */
void print_usage() {
	cout << 
		"Usage: dance [options] [file]\n"
		"\n"
		"Options:\n"
		"  -v, --verbose      Verbose output.\n"
		"  -q, --quiet        Quiet. Don't output to stdout.\n"
		"  --statistics       Print the number of nodes and link updates.\n"
		"  --profile          Print the node count and link update profile.\n"
		"  --no-heuristic     Disable the column selection by size heuristic.\n"
		"  --help             Print help information and exit.\n"
		"  --version          Print program version and exit.\n"
		"\n"
		"Report bugs to http://code.google.com/p/decs/issues/list.";
}

/**
 * Prints version and copyright notice.
 */
void print_version() {
	cout << 
		"dance (DECS toolkit) " << VERSION << "\n"
		"Copyright (C) 2007-2009 Jan Magne Tjensvold\n"
		"This is free software; See the source for copying conditions. There is NO\n"
		"WARRANTY; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.";
}

void usage_error() {
	print_usage();
	exit(1);
}

int solve_error(uint code) {
	switch (code) {
	case DFIO_ERR_FILE_OPEN:
		cerr << "Unable to open the specified file";
		return 1;
		break;
	case DFIO_ERR_FILE_VERSION:
		cerr << "Incompatible file version";
		return 1;
		break;
	case DFIO_ERR_FILE_PARSE:
		cerr << "Failed to parse data file";
		return 1;
		break;
	case DFIO_ERR_OOB_COL_IDX:
		cerr << "Out of bounds column index encountered while reading from file";
		return 1;
		break;
	case DFIO_ERR_OOB_COLUMNS:
		cerr << "Out of bounds column count encountered while reading from file";
		return 1;
		break;
	case DFIO_ERR_COL_UNSORTED:
		cerr << "Unsorted column index encountered while reading from file";
		return 1;
		break;
	case DFIO_ERR_OOB_ELEMS:
		cerr << "Number of elements read from the file is out of bounds";
		return 1;
		break;
	case DFIO_ERR_ROW_UNSORTED:
		cerr << "Unsorted row index encountered while reading from file";
		return 1;
		break;
	default:
		cerr << "Unknown error code returned by libdlx: " << code;
		return 1;
		break;
	}
}

int main(int argc, char* argv[]) {
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	
	uint matrix_id = 0; // Default matrix index. TODO: Create command line param to allow user to specify this.
	uint verbose = 1;
	bool showStats = false;
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
		} else if (!strcmp(argv[i], "--statistics")) {
			showStats = true;
		} else if (!strcmp(argv[i], "--profile")) {
			showProfile = true;
		} else if (!strcmp(argv[i], "--no-heuristic")) {
			dlx_set_heuristic(false);
		} else if (i == argc-1) {
			file = argv[i];
		} else {
			cerr << "Unknown command line parameter '" << argv[i] << "'" << endl;
			usage_error();
		}
	}
	
	if (!file) {
		cerr << "No file specified on the command line" << endl;
		usage_error();
	}
	
	dlx_set_verbose_level(verbose);
	
	if (verbose > 0) {
		cout << "Dancing to the rhythm of\n" << file << endl << endl;
		cout << "Loading..." << endl;
	}
	
	decs::DataFile df;
	fstream f(file, ios::in | ios::binary);
	if (!f) {
		f.close();
		return DFIO_ERR_FILE_OPEN;
	} else if (!df.ParseFromIstream(&f)) {
		f.close();
		return DFIO_ERR_FILE_PARSE;
	}
	f.close();
		
	SBMatrix* matrix = new SBMatrix();
	dfio_set_data_file(&df);
	dfio_read_matrix(matrix, matrix_id);
	dfio_cleanup();

	if (verbose > 0) cout << "Searching..." << endl;
	int result = dlx_solve(matrix);
	if (result != DFIO_ERR_SUCCESS) return solve_error(result);
	

	// Output results and statistics from the solving.
	uxlong updates = dlx_get_updates();
	uxlong nodes = dlx_get_nodes();
	uxlong solutions =  dlx_count_solutions();

	if (verbose > 0) {
		cout << "Search complete" << endl;
		cout << endl << "Number of solutions: " << solutions;
	}
	if (showStats) {
		cout << endl;
		cout << "Total nodes: " << nodes << endl;
		cout << "Total link updates: " << updates;
	}
	if (showProfile) {
		cout << endl << endl;
		cout << "Node, link update and solution profile:" << endl;
		cout << "Level       Nodes               Updates             Solutions" << endl; 
		uint maxLevel = dlx_get_max_level() + 1;
		for (uint i = 0; i < maxLevel; i++) {
			uxlong nod = dlx_get_node_profile(i);
			uxlong upd = dlx_get_update_profile(i);
			uxlong sol = dlx_get_solution_profile(i);

			cout.precision(1);
			cout.setf(ios_base::right | ios_base::fixed);

			cout.width(4);
			cout << i;

			cout.width(12);
			cout << nod << " (";
			cout.width(5);
			cout << double(nod) / double(nodes) * 100 << "%)";

			cout.width(12);
			cout << upd << " (";
			cout.width(5);
			cout << double(upd) / double(updates) * 100 << "%)";

			cout.width(12);
			cout << sol << " (";
			cout.width(5);
			cout << double(sol) / double(solutions) * 100 << "%)";

			cout << endl;


//			printf(" %3u  %10u (%5.1f%%)  %10u (%5.1f%%)  %10u (%5.1f%%)\n", i,
//				nod, double(nod) / double(nodes) * 100,
//				upd, double(upd) / double(updates) * 100,
//				sol, double(sol) / double(solutions) * 100);
		}

		cout << endl << endl;
		cout << "Fanout profile:" << endl;
		cout << "Level    Nodes      Updates     Solutions" << endl; 
		uxlong upd1 = dlx_get_update_profile(0);
		uxlong nod1 = dlx_get_node_profile(0);
		uxlong sol1 = dlx_get_solution_profile(0);
		for (uint i = 1; i < maxLevel; i++) {
			uxlong upd = dlx_get_update_profile(i);
			uxlong nod = dlx_get_node_profile(i);
			uxlong sol = dlx_get_solution_profile(i);

			cout.width(4);
			cout << i;

			cout.width(10);
			cout << double(nod) / double(nod1) * 100 << "%";

			cout.width(10);
			cout << double(upd) / double(upd1) * 100 << "%";

			if (sol1 > 0) {
				cout.width(10);
				cout << double(sol) / double(sol1) * 100 << "%";
			}

			cout << endl;

			upd1 = upd;
			nod1 = nod;
			sol1 = sol;
		}
	}
	
	return 0;
}

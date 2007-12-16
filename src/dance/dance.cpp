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
#include "dfileio.h"
#include "sbmatrix.h"

const char* VERSION = "0.3";

using namespace std;

/**
 * Prints command line options.
 */
void print_usage() {
	cout << "Usage: dance [OPTIONS] [FILE]\n"
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
			"Report bugs to <http://code.google.com/p/decs/issues/list>.";
}

/**
 * Prints version and copyright notice.
 */
void print_version() {
	cout << "dance (DECS toolkit) " << VERSION << "\n"
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
	case DFIO_ERR_FILE_OPEN:
		cerr << "Unable to open the specified file";
		return 1;
		break;
	case DFIO_ERR_FILE_VERSION:
		cerr << "Incompatible file version";
		return 1;
		break;
	case DFIO_ERR_FILE_ID:
		cerr << "Incompatible file ID";
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
			cerr << "Unknown command line parameter '" << argv[i] << "'\n";
			usage_error();
		}
	}
	
	if (!file) {
		cerr << "No file specified on the command line\n";
		usage_error();
	}
	
	dlx_set_verbose_level(verbose);
	
	if (verbose > 0) {
		cout << "Dancing to the rhythm of\n" << file << "\n\n";
		cout << "Loading..." << endl;
	}
	
#ifdef _WIN32
	FILE* f = fopen(file, "rb");
#else
	FILE* f = fopen(file, "r");
#endif
	if (!f) {
		cerr << "Unable to open file for reading '" << file << "'\n";
		return 1;
	}
	
	
	dfio_load_file(f);
	SBMatrix* matrix = new SBMatrix();
	dfio_read_matrix(matrix);
	dfio_cleanup();

	if (verbose > 0) cout << "Searching..." << endl;
	uint result = dlx_solve(matrix);
	if (result != DFIO_ERR_SUCCESS) return solve_error(result);
	

	// Output results from the solving.
	uint updates = dlx_get_updates();
	uint nodes = dlx_get_nodes();
	uint solutions =  dlx_count_solutions();

	if (verbose > 0) {
		cout << "Search complete\n";
		cout << "\nNumber of solutions: " << solutions;
	}
	if (showStats) {
		cout << "\nTotal nodes: " << nodes;
		cout << "\nTotal link updates: " << updates;
	}
	if (showProfile) {
		cout << "\n\nLink, update and solution profile:\n";
		cout << "Level        Nodes               Updates             Solutions\n"; 
		uint maxLevel = dlx_get_max_level() + 1;
		for (uint i = 0; i < maxLevel; i++) {
			uint upd = dlx_get_update_profile(i);
			uint nod = dlx_get_node_profile(i);
			uint sol = dlx_get_solution_profile(i);
			printf(" %3u  %10u (%5.1f%%)  %10u (%5.1f%%)  %10u (%5.1f%%)\n", i,
				nod, double(nod) / double(nodes) * 100,
				upd, double(upd) / double(updates) * 100,
				sol, double(sol) / double(solutions) * 100);
		}

		cout << "\n\nFanout profile:\n";
		cout << "Level    Nodes      Updates     Solutions\n"; 
		uint upd1 = dlx_get_update_profile(0);
		uint nod1 = dlx_get_node_profile(0);
		uint sol1 = dlx_get_solution_profile(0);
		for (uint i = 1; i < maxLevel; i++) {
			uint upd = dlx_get_update_profile(i);
			uint nod = dlx_get_node_profile(i);
			uint sol = dlx_get_solution_profile(i);
			if (sol1 > 0)
				printf(" %3u  %8.1f%  %8.1f%  %8.1f%\n", i,
					double(nod) / double(nod1) * 100,
					double(upd) / double(upd1) * 100,
					double(sol) / double(sol1) * 100);
			else
				printf(" %3u  %8.1f%  %8.1f%\n", i,
					double(nod) / double(nod1) * 100,
					double(upd) / double(upd1) * 100);
			upd1 = upd;
			nod1 = nod;
			sol1 = sol;
//			cout << i << '\t' << upd << " (" << () * 100.0 << "%)" << '\t' << nod << " (" << (double(nod) / double(nodes)) * 100.0 << "%)\n"; 
		}
	}
	
	return 0;
}

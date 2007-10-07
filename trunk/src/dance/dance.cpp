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

typedef unsigned int uint;

const char* version = "0.2";

using namespace std; 

/**
 * Prints command line options.
 */
void print_usage() {
	cout << "Usage: dance [OPTIONS] [FILE]\n\n";
	cout << "Options:\n";
	cout << "  -v                 Verbose output.\n";
	cout << "  -q                 Quiet. Don't output to stdout.\n";
	cout << "  --count-updates    Print the number of link updates.\n";
	cout << "  --profile          Print the link update profile.\n";
	cout << "  --help             Print help information and exit.\n";
	cout << "  --version          Print program version and exit.\n\n";
	cout << "Report bugs to <http://code.google.com/p/decs/issues/list>.";
}

/**
 * Prints version and copyright notice.
 */
void print_version() {
	cout << "Dance (DECS) " << version << '\n';
	cout << "Copyright (c) 2007 Jan Magne Tjensvold\n";
	cout << "This is free software. You may redistribute copies of it under the terms of\n";
	cout << "the GNU General Public License version 2 as published by the Free Software\n";
	cout <<	"Foundation <http://www.gnu.org/licenses/gpl.html>.\n";
	cout << "There is NO WARRANTY, to the extent permitted by law.";
}

int main(int argc, char* argv[]) {
	uint verbose = 1;
	bool countUpdates = false;
	bool showProfile = false;
	char* file = 0;
	
	// Interpret command line parameters.
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i],"-q")) {
			verbose = 0;
		} else if (!strcmp(argv[i],"-v")) {
			verbose++;
		} else if (!strcmp(argv[i],"--help")) {
			print_usage();
			return 0;
		} else if (!strcmp(argv[i],"--version")) {
			print_version();
			return 0;
		} else if (!strcmp(argv[i],"--count-updates")) {
			countUpdates = true;
		} else if (!strcmp(argv[i],"--profile")) {
			showProfile = true;
		} else {
			if (i == argc-1) {
				file = argv[i];
			} else {
				cerr << "Unknown command line parameter '" << argv[i] << "'\n";
				print_usage();
				return 1;
			}
		}
	}
	
	if (file == NULL) {
		cerr << "No file specified on the command line\n";
		print_usage();
		return 1;
	}
	
	if (verbose > 0) { 
		cout << "Dancing to the rhythm of" << '\n';
		cout << file << "\n\n";
	}
	dlx::setVerboseLevel(verbose);
	dlx::solve(file);
	
	uint updates = dlx::getUpdates();
	
	if (showProfile) {
		cout << "Link update profile:\n";
		uint maxLevel = dlx::getMaxLevel();
		for (uint i = 0; i < maxLevel; i++) {
			uint upd = dlx::getProfile(i);
			cout << i << '\t' << upd << " (" << (double(upd) / double(updates)) * 100.0 << "%)\n"; 
		}
	}
	
	if (countUpdates)
		cout << "Link updates: " << updates << '\n';

	return 0;
}

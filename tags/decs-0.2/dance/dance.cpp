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
uint verbose = 1;

using namespace std; 

void print_usage() {
	cout << "Usage: dance.exe [options] <file.decs>\n\n";
	cout << "Options:\n";
	cout << "    -v   Verbose output.\n";
	cout << "    -q   Quiet. Don't output to stdout.\n";
}

int main(int argc, char* argv[]) {
	if (argc > 2) {
		if (!strcmp(argv[1],"-q")) {
			verbose = 0;
		} else if (!strcmp(argv[1],"-v")) {
			verbose = 2;
		}
	}
	
	if (verbose > 0) {
		cout << "Dance v" << version << '\n';
		cout << "Copyright (c) 2007  Jan Magne Tjensvold\n\n";
	}
	
	if (argc < 2) {
		cerr << "No file specified on the command line" << endl;
		print_usage();
		return 1;
	}
	
	if (verbose > 0) { 
		cout << "Dancing to the rhythm of" << '\n';
		cout << argv[argc-1] << "\n\n";
	}
	dlx::setVerboseLevel(verbose);
	dlx::solve(argv[argc-1]);
	return 0;
}

/**
 * degen - Exact cover matrix generator for the Dancing Links library.
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
#include "nqdecs.h"

const char* version = "0.2";

using namespace std; 

void print_usage() {
	cout << "Usage: degen.exe <params> <file.decs>" << '\n';
}

int main(int argc, char* argv[]) {
	cout << "Degen v" << version << '\n';
	cout << "Copyright (c) 2007  Jan Magne Tjensvold" << "\n\n";
	cout.flush();
	
	if (argc < 2) {
		cerr << "No file specified on the command line" << endl;
		print_usage();
		return 1;
	} else if (!strcmpi(argv[1],"-HELP")) {
		print_usage();
		return 0;
	}
	
	cout << "Generating matrix..." << '\n';
	setQueens(atoi(argv[1]));
	transform(argv[2]);
	cout << "Matrix generated and written to the file:" << '\n';
	cout << argv[2] << endl;
	return 0;
}

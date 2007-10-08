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

const char* version = "1.0";

using namespace std; 

void print_usage() {
	cout << "Usage: dance.exe <file.decs>" << '\n';
}

int main(int argc, char* argv[]) {
	cout << "Dance v" << version << '\n';
	cout << "Copyright (c) 2007  Jan Magne Tjensvold" << "\n\n";
	cout.flush();
	
	if (argc != 2) {
		cerr << "No file specified on the command line" << endl;
		print_usage();
		return 1;
	}
	
	cout << "Dancing to the rhythm of" << '\n';
	cout << argv[1] << "\n\n";
	dlx::solve(argv[1]);
	return 0;
}

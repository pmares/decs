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

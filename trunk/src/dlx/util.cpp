#include <fstream>
#include <iostream>
#include <cstdlib>
#include "util.h"

namespace dlx {

using namespace std;

/**
 * Write a message to stderr and exit with an exit code of 1 to indicate failure.
 */
void panic(const char* msg) {
	cerr << msg << endl;
	exit(1);
}

int read_file(const char* file, Column* header) {
	
	
	return 0;
}

} /*namespace dlx*/

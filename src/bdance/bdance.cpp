/**
 * bdance - Exact cover solver program to be used with BOINC.
 * Copyright (C) 2007-2008 Jan Magne Tjensvold
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

#ifdef _WIN32
#include "boinc_win.h"
#else
#include "config.h"
#endif

#ifndef _WIN32
#include <cstdio>
#include <cctype>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <unistd.h>
#endif

#include "sbmatrix.h"
#include "dfileio.h"
#include "dlx.h"

#include "diagnostics.h"
#include "str_util.h"
#include "util.h"
#include "filesys.h"
#include "boinc_api.h"
#include "mfile.h"

const char* APP_VERSION = "0.3";
const char* INPUT_FILE = "in.decs";
const char* OUTPUT_FILE = "out.decs";

void worker() {
	int nchars = 0;
	int retval;
    char input_path[512];
	char output_path[512];
	FILE* outfile;
	FILE *infile;

    boinc_resolve_filename(INPUT_FILE, input_path, sizeof(input_path));
#ifdef _WIN32
    infile = boinc_fopen(input_path, "rb");
#else
    infile = boinc_fopen(input_path, "r");
#endif
    if (!infile) {
        fprintf(stderr,
            "Couldn't find input file, resolved name %s.\n", input_path
        );
        exit(-1);
    }

	// Dance Dance Revolution
	dfio_load_file(infile);
	SBMatrix* matrix = new SBMatrix();
	retval = dfio_read_matrix(matrix);
	if (retval) {
        fprintf(stderr, "APP: bdance parse input file failed:\n");
        fprintf(stderr, "error code %d\n", retval);
		dfio_cleanup();
        exit(1);
    }
	dfio_cleanup();
	fclose(infile);


	// Solve
	dlx_solve(matrix);
	uxlong solutions = dlx_count_solutions();
	delete matrix;


	// Write results.
    boinc_resolve_filename(OUTPUT_FILE, output_path, sizeof(output_path));
	outfile = boinc_fopen(output_path, "w");
    if (!outfile) {
        fprintf(stderr,
            "Couldn't find output file, resolved name %s.\n", output_path
        );
        exit(-1);
    }

	dfio_new_file(outfile, DFIO_TYPE_RESULT);
//	if (dfio_get_prop(DFIO_PROP_NO_SOLUTION_DATA)) {
		dfio_set_prop(DFIO_PROP_RESULTS_NUM, solutions);
		dfio_write_headers();
//	}

	dfio_cleanup();
	fclose(outfile);
    boinc_finish(0);
}

int main(int argc, char **argv) {
    int retval = boinc_init();
    if (retval) exit(retval);
    worker();
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR Args, int WinMode) {
    LPSTR command_line;
    char* argv[100];
    int argc;

    command_line = GetCommandLine();
    argc = parse_command_line( command_line, argv );
    return main(argc, argv);
}
#endif

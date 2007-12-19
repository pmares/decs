// Berkeley Open Infrastructure for Network Computing
// http://boinc.berkeley.edu
// Copyright (C) 2005 University of California
//
// This is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation;
// either version 2.1 of the License, or (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// To view the GNU Lesser General Public License visit
// http://www.gnu.org/copyleft/lesser.html
// or write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

// This is the primary sample BOINC application;
// it shows most of the features of the BOINC API.
//
// read "in.decs", convert to upper case, write to "out.decs"
//
// command line options (use for debugging various scenarios):
// -run_slow: sleep 1 second after each character; useful for debugging
// -cpu_time N: use about N CPU seconds after copying files
// -early_exit: exit(10) after 30 chars
// -early_crash: crash after 30 chars
//

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

using std::string;

const char* VERSION = "0.3";

const char* CHECKPOINT_FILE = "bdance_state";
const char* INPUT_FILE = "in.decs";
const char* OUTPUT_FILE = "out.decs";


//bool run_slow = false;
//bool early_exit = false;
//bool early_crash = false;
//bool early_sleep = false;
//double cpu_time = 20;


int do_checkpoint(MFILE& mf, int nchars) {
    int retval;
    string resolved_name;

    FILE* f = fopen("temp", "w");
    if (!f) return 1;
    fprintf(f, "%d", nchars);
    fclose(f);

    fprintf(stderr, "APP: bdance checkpointing\n");

    retval = mf.flush();
    if (retval) return retval;
    boinc_resolve_filename_s(CHECKPOINT_FILE, resolved_name);
    retval = boinc_rename("temp", resolved_name.c_str());
    if (retval) return retval;

	//use_some_cpu();
    fprintf(stderr, "APP: bdance checkpoint done\n");
    return 0;
}


void worker() {
//	int c;
	int nchars = 0;
	int retval;
//	int n;
//	double fsize;
    char input_path[512];
	char output_path[512];
//	char chkpt_path[512];
	FILE* outfile;
//	FILE* state
	FILE *infile;

    // open the input file (resolve logical name first)
    //
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

    // get size of input file (used to compute fraction done)
    //
//    file_size(input_path, fsize);

    // open output file
    //

    // See if there's a valid checkpoint file.
    // If so seek input file and truncate output file
    //
//    boinc_resolve_filename(CHECKPOINT_FILE, chkpt_path, sizeof(chkpt_path));
//    state = boinc_fopen(chkpt_path, "r");
//    if (state) {
//        n = fscanf(state, "%d", &nchars);
//        fclose(state);
//    }
//    if (state && n==1) {
//        fseek(infile, nchars, SEEK_SET);
//        boinc_truncate(output_path, nchars);
//        retval = out.open(output_path, "a");
//    } else {

	// Dance Dance Revolution
	dfio_load_file(infile);
	SBMatrix* matrix = new SBMatrix();
	retval = dfio_read_matrix(matrix);
	if (retval) {
        fprintf(stderr, "APP: bdance parse input file failed:\n");
        fprintf(stderr, "error code %d\n", retval);
//        perror("open");
		dfio_cleanup();
        exit(1);
    }
	dfio_cleanup();
	fclose(infile);


	// Solve
	dlx_solve(matrix);
	uint solutions = dlx_count_solutions();
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

	while (dlx_working()) {
//		boinc_sleep(1);

		// Update progress.
//		boinc_fraction_done(f);

/*		if (boinc_time_to_checkpoint()) {
			retval = do_checkpoint(out, nchars);
			if (retval) {
				fprintf(stderr, "APP: bdance checkpoint failed %d\n", retval);
				exit(retval);
			}
			boinc_checkpoint_completed();
		}
*/	}

//    retval = out.flush();
//    if (retval) {
//        fprintf(stderr, "APP: bdance flush failed %d\n", retval);
//        exit(1);
//    }

    // burn up some CPU time if needed
    //
//    if (cpu_time) {
//        double start = dtime();
//        while (1) {
//            double e = dtime()-start;
//            if (e > cpu_time) break;
//            boinc_fraction_done(.5 + e/(cpu_time*2));
//
//			if (boinc_time_to_checkpoint()) {
//				retval = do_checkpoint(out, nchars);
//				if (retval) {
//					fprintf(stderr, "APP: bdance checkpoint failed %d\n", retval);
//					exit(1);
//				}
//				boinc_checkpoint_completed();
//			}
//
//            use_some_cpu();
//        }
//    }
    boinc_finish(0);
}

int main(int argc, char **argv) {
//	int i;
    int retval = 0;

//    for (i=0; i<argc; i++) {
//        if (!strcmp(argv[i], "-early_exit")) early_exit = true;
//        if (!strcmp(argv[i], "-early_crash")) early_crash = true;
//        if (!strcmp(argv[i], "-early_sleep")) early_sleep = true;
//        if (!strcmp(argv[i], "-run_slow")) run_slow = true;
//        if (!strcmp(argv[i], "-cpu_time")) {
//            cpu_time = atof(argv[++i]);
//        }
//    }

    retval = boinc_init();
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

//const char *BOINC_RCSID_33ac47a071 = "$Id: bdance.cpp 112 2007-12-08 17:36:46Z janmagnet $";

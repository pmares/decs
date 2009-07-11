/**
 * libdecs - Exact cover solver library using the Dancing Links algorithm.
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

#ifndef DFIO_FORMAT_H_
#define DFIO_FORMAT_H_

/**
 * This file contains structs for the DECS file format. These structs has been
 * placed in a separate header so that client applications does not have to
 * import them directly.
 */

/** DECS file version header which should always be present. */
struct DFIOHeader {
	uint id;
	ubyte version;
	ubyte minor; /* Sort of deprecated/Not used */
	ubyte type;  
	ubyte reserved;
};

/** Matrix header format version 1. */ 
struct DFIOMatrixHeader_1_0 {
	uint cols;
	uint rows;
	uint elems;
	uint elem_off;
	uint name_off;
	uint probid;
	uint prob_off;
};

/** Matrix header format version 2. */
struct DFIOMatrixHeader_2_0 {
	uint cols;
	uint rows;
	uint elems;
	uint elem_off;
	uint init_off;
	uint name_off;
	uint probid;
	uint prob_off;
};

/** Matrix header format version 3. */
/** Experimental: Work in progress */
struct DFIOMatrixHeader_3_0 {
	uint cols;
	uint rows;
	uint elems;
	uint elem_off;
	uint secol_off;  // Readded: Separate it from the element data.
	uint init_off;
	uint name_off;
	uint probid;
	uint prob_off;
	uint flags; // New: If LSB set "conserver bandwidth" is on and only the number of solutions is returned.
};


/** Result header format version 1. */
struct DFIOResultHeader_1_0 {
	uint results;
	uint result_off;
	uint probid;
	uint prob_off;
};

/** Result header format version 2. */
/** Experimental: Work in progress */
struct DFIOResultHeader_2_0 {
	uxlong results;  // Changed from ulong to uxlong
	uint result_off;
	uint profile_size;  // New: Indicates the size of the profile. It consists entirely of 64-bit unsigned integers.
	uint profile_off;   // New: Added profile: <nodes> <updates> <solutions> for each recursion level (starting from 0 to profile_size - 1)
	uint probid;
	uint prob_off;
};


/** The current file header versions. */
typedef DFIOMatrixHeader_2_0 DFIOMatrixHeader;
typedef DFIOResultHeader_2_0 DFIOResultHeader;

/** File type ID. This is a 32-bit hex representation for the ASCII "DECS". */
const uint DFIO_FILE_ID = 0x53434544;

/** The major file format version implemented by this library. */
const uint DFIO_FILE_VERSION = 3;

/** Deprecated: Minor file format version implemented by this library. */
const uint DFIO_FILE_MINOR = 0;


void dfio_bswap_header(DFIOHeader* fh);
void dfio_bswap_matrix_header(DFIOMatrixHeader* fh);
void dfio_bswap_result_header(DFIOResultHeader* fh);
void dfio_bswap_uints(uint* ary, uint elements);
uint dfio_cbswap_uint(uint value);
bool dfio_is_big_endian();

#endif /*DFIO_FORMAT_H_*/

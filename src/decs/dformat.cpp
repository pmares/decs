/**
 * fileio - Library for reading and writing DECS files.
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

#include "type.h"
#include "dformat.h"


enum DFIOEndianness {
	DFIO_END_UNKNOWN = 0,
	DFIO_END_BIG_ENDIAN = 1,
	DFIO_END_LITTLE_ENDIAN = 2,
};


// Endian cache.
DFIOEndianness endian = DFIO_END_UNKNOWN;


/** Test platform Endianness. */
inline bool dfio_is_big_endian() {
	if (endian == DFIO_END_UNKNOWN) {
		union {
			char Array[4];
			long Chars;
		} TestUnion;
		char c = 'a';
		for (int x = 0; x < 4; x++)
			TestUnion.Array[x] = c++;
		endian = (TestUnion.Chars == 0x61626364) ? DFIO_END_BIG_ENDIAN : DFIO_END_LITTLE_ENDIAN;
	}
	return (endian == DFIO_END_BIG_ENDIAN);
}


inline uint dfio_bswap_uint(uint value) {
	return (((value) >> 24) & 0x000000FF) 
		| (((value) >> 8) & 0x0000FF00)
		| (((value) << 8) & 0x00FF0000)
		| (((value) << 24) & 0xFF000000);
}


uint dfio_cbswap_uint(uint value) {
	if (!dfio_is_big_endian()) {
		return value;
	} else {
		return dfio_cbswap_uint(value);
	}
}


void dfio_bswap_header(DFIOHeader* fh) {
	if (!dfio_is_big_endian()) return;
	fh->id = dfio_bswap_uint(fh->id);
}


void dfio_bswap_matrix_header(DFIOMatrixHeader* fh) {
	if (!dfio_is_big_endian()) return;
	fh->cols = dfio_bswap_uint(fh->cols);
	fh->rows = dfio_bswap_uint(fh->rows);
	fh->elems = dfio_bswap_uint(fh->elems);
	fh->elem_off = dfio_bswap_uint(fh->elem_off);
	fh->init_off = dfio_bswap_uint(fh->init_off);
	fh->name_off = dfio_bswap_uint(fh->name_off);
	fh->probid = dfio_bswap_uint(fh->probid);
	fh->prob_off = dfio_bswap_uint(fh->prob_off);
}


void dfio_bswap_result_header(DFIOResultHeader* fh) {
	if (!dfio_is_big_endian()) return;
	fh->results = dfio_bswap_uint(fh->results);
	fh->result_off = dfio_bswap_uint(fh->result_off);
	fh->probid = dfio_bswap_uint(fh->probid);
	fh->prob_off = dfio_bswap_uint(fh->prob_off);
}


void dfio_bswap_uints(uint* ary, uint elements) {
	if (!dfio_is_big_endian()) return;
	for (uint i = 0; i < elements; i++) {
		ary[i] = dfio_bswap_uint(ary[i]);
	}
}

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

#ifndef DFIO_H_
#define DFIO_H_

#include <cstdio>

#include "type.h"
#include "dformat.h"
#include "sbmatrix.h"

/**
 * Global library constants.
 */


/**
 * Error codes which some of the function returns.
 */

enum DFIOError {
	/** No error. */
	DFIO_ERR_SUCCESS = 0,
	
	/** Unable to open the specified file. */
	DFIO_ERR_FILE_OPEN = 1,
	
	/** The file version is incompatible. */
	DFIO_ERR_FILE_VERSION = 2,
	
	/** Unknown file ID (the first 4 bytes was not 'DECS'). */
	DFIO_ERR_FILE_ID = 3,
	
	/** The column indexes read from the file is not sorted. */
	DFIO_ERR_COL_UNSORTED = 4,
	
	/** A column index read from the file is out of bounds. */
	DFIO_ERR_OOB_COL_IDX = 5,
	
	/** The number of columns in a row read from the file is out of bounds. */
	DFIO_ERR_OOB_COLUMNS = 6,
	
	/** The number of elements read from the file is out of bounds. */
	DFIO_ERR_OOB_ELEMS = 7,
	
	/** The row indexes read from the file is not sorted. */
	DFIO_ERR_ROW_UNSORTED = 8,
	
	/** No file loaded, call dfio_load_file(); */
	DFIO_ERR_NO_FILE = 9,
	
	/** Unable to write to file. */
	DFIO_ERR_FILE_WRITE = 10,
	
	/** Unknown or wrong file type. */
	DFIO_ERR_FILE_TYPE = 11,
	
	/** The property does not exist. */
	DFIO_ERR_NO_PROP = 12,
	
	/** Could not find element or result data. */
	DFIO_ERR_NO_DATA = 13,

	/** The number of rows is out of bounds. */
	DFIO_ERR_OOB_ROWS = 14,

	/** File/Property/Element is read only. */
	DFIO_ERR_READ_ONLY = 15,
};


/**
 * File property constants. Pass one of these to the decs_fio_get_prop(uint)
 * function to get the value of the property. All byte offsets calculated from
 * the beginning of the file.
 */

enum DFIOProperty {
	/** File type ID. Should always be "DECS". */
	DFIO_PROP_FILEID = 0x1,
	
	/** Major file format version. */
	DFIO_PROP_VERSION = 0x2,
	
	/**
	 * Minor file format version. Reading a file with the same major version as the
	 * one supported by the library, but different minor version will not trigger
	 * the version incompatibility error. However it might trigger a warning.
	 */
	DFIO_PROP_COMPAT = 0x4,
	
	/** Number of columns. Should always be 1 or more. */
	DFIO_PROP_COLS_NUM = 0x8,
	
	/** Number of rows. Should always be 1 or more. */
	DFIO_PROP_ROWS_NUM = 0x10,
	
	/**
	 * Number of elements. Should always be at least max(columns, rows), but never
	 * more than "columns x rows".
	 */
	DFIO_PROP_ELEMS_NUM = 0x20,
	
	/** Byte offset to the matrix element entries. Should never be 0. */
	DFIO_PROP_ELEMS_OFF = 0x40,
	
	/**
	 * Number of secondary columns. If DFIO_PROP_SECOLS_OFF is 0 then this value is
	 * 0 as well.
	 */
	DFIO_PROP_SECOLS_NUM = 0x80,
	
	/** Byte offset to the initialization vector row list. 0 if unavailable. */
	DFIO_PROP_INIT_OFF = 0x100,

	/**
	 * Number of initialization vector rows. If DFIO_PROP_INIT_OFF is 0 then this
	 * value is 0 as well.
	 */
	DFIO_PROP_INIT_NUM = 0x200,

	/**
	 * Number of column names. Should never be more than the value of
	 * DFIO_PROP_COLS_NUM. If DFIO_PROP_NAMES_OFF is 0 then this value is 0 as well.
	 */
	DFIO_PROP_COL_NAMES_NUM = 0x400,
	
	/**
	 * Number of row names. Should never be more than DFIO_PROP_ROWS_NUM. If
	 * DFIO_PROP_NAMES_OFF is 0 then this value is 0 as well.
	 */
	DFIO_PROP_ROW_NAMES_NUM = 0x800,
	
	/** Byte offset to the column and row name list. 0 if unavailable. */
	DFIO_PROP_NAMES_OFF = 0x1000,
	
	/**
	 * Problem type ID. Identifies which library wrote this file so that the
	 * DFIO_PROP_PROB_OFF data can be decoded.
	 */
	DFIO_PROP_PROB_ID = 0x2000,
	
	/** Byte offset to problem specific information. 0 if unavailable. */
	DFIO_PROP_PROB_OFF = 0x4000,
	
	/** Byte offset to the secondary column list. 0 if unavailable. */
	DFIO_PROP_SECOLS_OFF = 0x8000,

	/** Number of results in result file. 0 if unavailable. */
	DFIO_PROP_RESULTS_NUM = 0x10000,

	/** Byte offset to the results row list. 0 if unavailable. */
	DFIO_PROP_RESULTS_OFF = 0x20000,
};


/**
 * File format compatibility. 
 */

enum DFIOCompat {
	DFIO_COMP_UNKNOWN = 0x0,

	/**
	 * Forward compatibility mode. This is normally enabled when the file has the
	 * same major version and a higher minor version number than the library. In
	 * certain special cases it may also be enabled when the major number of the
	 * file is higher.
	 */
	DFIO_COMP_FORWARD = 0x1,
	
	/**
	 * Backward compatibility mode. This is normally enabled when the file has a
	 * lower major and/or minor version number than the library.
	 */
	DFIO_COMP_BACKWARD = 0x2,
	
	/**  */
	DFIO_COMP_INCOMPATIBLE = 0x4,
	
	/** No compatibility mode enabled. */
	DFIO_COMP_NATIVE = 0x8,
};


/**
 * File types. 
 */

enum DFIOFileType {
	/**
	 * Unknown file type. If opening a file for read access this indicates
	 * an error condition.
	 */
	DFIO_TYPE_UNKNOWN = 0,
	
	/** File contains an exact cover matrix. */
	DFIO_TYPE_MATRIX = 1,

	/** File contains results/solutions to an exact cover matrix. */
	DFIO_TYPE_RESULT = 2,
};


/** Opening and closing files */
//uint dfio_open_read(char* file);
//uint dfio_open_write(char* file);
//uint dfio_close();
uint dfio_new_file(FILE* f, DFIOFileType type);
uint dfio_load_file(FILE* f);
uint dfio_cleanup();

DFIOError dfio_last_error();
char* dfio_error_msg();
//void dfio_error_msg(char[] msg, uint len);
//void dfio_error_clear();


/* int read_file(char* file, Column* header, uint verbose); */ /* OLD VERSION */
// uint dfio_read(...);


/**
 * Checks if the specified property or properties are available in for the
 * current file. A bitwise OR of the needed values in DFIOProperty can be given
 * as input. If only one property is passed to the function it will return a
 * non-zero value if that property is available, and zero if it is not
 * available. If several bitwise OR'ed properties are passed it will return
 * zero only if all the properties are unavailable. Otherwise it returns a
 * bitwise OR'ed value of the properties which are available.
 */
uint dfio_has_prop(uint prop);
/* uint decs_fio_get_prop(uint property, uint* value); */
uint dfio_get_prop(DFIOProperty prop);
uint dfio_set_prop(DFIOProperty prop, uint value);

/**
 * Returns the version compatibility mode currently in effect. The DFIO_COMP_*
 * constants determine if it runs in native, forward or backward compatibility
 * mode. It will also flag any incompatibilities and warn if potential problems
 * like loss of information might arise.
 */
uint dfio_comp_mode();

DFIOFileType dfio_file_type();

uint dfio_read_matrix(SBMatrix* matrix);
uint dfio_write_headers();


#endif /*DFIO_H_*/

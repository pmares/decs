/**
 * libdlx - Exact cover solver library using the Dancing Links algorithm.
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

#ifndef DLX_H_
#define DLX_H_

namespace dlx {

typedef unsigned int uint;

const uint MAX_LEVELS = 128;

// Error codes which can be returned from the solve() method.
const uint ERR_SUCCESS = 0;          // No error.
const uint ERR_FILE_OPEN = 1;        // Unable to open the specified file.
const uint ERR_FILE_VERSION = 2;     // The file version is incompatible.
const uint ERR_FILE_ID = 3;          // Unknown file ID (the first 4 bytes was not 'DECS').
const uint ERR_COL_IDX_OUB = 4;      // A column index read from the file is out of bounds.
const uint ERR_COL_COUNT_OUB = 5;    // The number of columns in a row read from the file is out of bounds.
const uint ERR_COL_UNSORTED = 6;     // The column indexes read from the file is not sorted.
const uint ERR_ELEMS_OUB = 7;        // The number of elements read from the file is out of bounds.

int solve(char* file);
void setVerboseLevel(uint level);
uint getUpdates();
uint getMaxLevel();
uint getProfile(uint level);
uint countSolutions();
uint countColumns();

} /*namespace dlx*/

#endif /*DLX_H_*/

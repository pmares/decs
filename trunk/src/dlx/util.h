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

#ifndef DLX_UTIL_H_
#define DLX_UTIL_H_

namespace dlx {

// Forward declaration to prevent circular dependency between headers.
class Column;

#if CPU_ARCHITECTURE == BIG_ENDIAN

#define SwapTwoBytes(data) ( \
		(((data) >> 8) & 0x00FF) | \
		(((data) << 8) & 0xFF00) )

#define SwapFourBytes(data) ( \
		(((data) >> 24) & 0x000000FF) | \
		(((data) >> 8)  & 0x0000FF00) | \
		(((data) << 8)  & 0x00FF0000) | \
		(((data) << 24) & 0xFF000000) )

#define SwapEightBytes(data) ( \
		(((data) >> 56) & 0x00000000000000FF) | \
		(((data) >> 40) & 0x000000000000FF00) | \
		(((data) >> 24) & 0x0000000000FF0000) | \
		(((data) >> 8)  & 0x00000000FF000000) | \
		(((data) << 8)  & 0x000000FF00000000) | \
		(((data) << 24) & 0x0000FF0000000000) | \
		(((data) << 40) & 0x00FF000000000000) | \
		(((data) << 56) & 0xFF00000000000000) )

#else

#define SwapTwoBytes (data) (data)
#define SwapFourBytes (data) (data)
#define SwapEightBytes (data) (data)

#endif

typedef unsigned int uint;
const uint FILE_ID = 0x53434544;
const uint FILE_VERSION = 2;
const uint FILE_COMPAT = 0;

void panic(const char* msg);
int read_file(char* file, Column* header, uint verbose, bool &init);
uint initv_size();
uint get_initv(uint index);
void util_cleanup();

} /*namespace dlx*/


#endif /*DLX_UTIL_H_*/

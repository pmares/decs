/**
 * sbmatrix - Sparse boolean matrix data structure library for DECS.
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

#ifndef SB_MATRIX_H_
#define SB_MATRIX_H_

#include <cstdio>

#include "sbcolumn.h"
#include "type.h"

class SBMatrix {
public:
	SBMatrix();
	SBMatrix(SBColumn* header);
	~SBMatrix();
	uint* getInitVector() { return initv; }
	uint getInitVectorSize() { return initv_size; }
	void setInitVector(uint* vector, uint vector_size) { initv = vector; initv_size = vector_size; }
	void setRoot(SBColumn* header) { root = header; }
	SBColumn* getRoot() { return root; }
	void setHeaders(SBColumn** headers, uint size);
private:
	SBColumn* root;
	uint* initv;
	uint initv_size;
	SBColumn** columns;
	uint ccount;
};

#endif /*SB_MATRIX_H_*/

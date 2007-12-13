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

#include "sbmatrix.h"


SBMatrix::SBMatrix(): 
	initv_size(0),
	ccount(0)
{
	initv = 0;
	columns = 0;
}

SBMatrix::SBMatrix(SBColumn* header):
	initv_size(0),
	ccount(0),
	root(header)
{
	initv = 0;
	columns = 0;
}

SBMatrix::~SBMatrix() {
	if (initv) delete[] initv;
	initv = 0;

	if (!root) return;
	if (!columns) return;

	SBNode* r1 = 0;
	SBNode* c1 = 0;
	SBNode* c = 0;
	for (uint i = 0; i < ccount; i++) {
		c = columns[i];
		for (SBNode* r = c->getUp(); r != c; r = r->getUp()) {
//			r->unlinkColumn();
			if (r1) delete r1;
			r1 = r;
		}
//		c->unlinkRow();
			if (r1) delete r1;
		r1 = 0;
		if (c1) delete c1;
		c1 = c;
	}
	if (c1) delete c1;
	delete root;
}


void SBMatrix::setHeaders(SBColumn** headers, uint size) {
	columns = headers;
	ccount = size;
}

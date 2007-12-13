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

#ifndef SB_COLUMN_H_
#define SB_COLUMN_H_

#include "sbnode.h"
#include "type.h"

class SBColumn : public SBNode {
public:
	SBColumn();
	SBColumn(uint index);
	virtual ~SBColumn();
	uint getSize() const { return size; }
	uint getIndex() const { return index; }
	void incrementSize() { size++; }
	void decrementSize() { size--; }
private:
	uint size;
	uint index;
};

#endif /*SB_COLUMN_H_*/

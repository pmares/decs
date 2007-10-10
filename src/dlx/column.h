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

#ifndef DLX_COLUMN_H_
#define DLX_COLUMN_H_

#include "node.h"
#include "util.h"

namespace dlx {

using namespace std;

class Column : public Node {
public:
	Column();
	Column(uint index);
	virtual ~Column();
	uint getSize() const { return size; }
	uint getIndex() const { return index; }
	void incrementSize() { size++; }
	void decrementSize() { size--; }
private:
	uint size;
	uint index;
};

} /*namespace dlx*/

#endif /*DLX_COLUMN_H_*/

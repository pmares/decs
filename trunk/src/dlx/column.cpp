#include "column.h"

namespace dlx {

Column::Column():
	size(0),
	index(0),
	name()
{
	setColumn(this);
}
Column::Column(uint index):
	size(0),
	index(index),
	name()
{
	setColumn(this);
}

Column::~Column()
{
}

} /*namespace dlx*/

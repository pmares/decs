#include <climits>
#include "node.h"
#include "column.h"

namespace dlx {

Column *r;

void addSolution() {
	// TODO
}

void search(int level) {
	Column *h;
	unsigned int s = UINT_MAX;

	// Return if all columns have been covered.
	if (h->getRight() == h) {
		addSolution();
		return;
	}

	// Choose a column to cover.
	for (Node *j = h->getRight(); j != h ; j = j->getRight()) {
		//		if (j->getColumn() < s)
	}

	// Cover the column.
}

}

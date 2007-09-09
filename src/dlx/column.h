#ifndef COLUMN_H_
#define COLUMN_H_

#include <string>
#include "dlx.h"
#include "node.h"

namespace dlx {

using namespace std;

class Column : public Node {
public:
	Column();
	virtual ~Column();
	uint getSize() const { return size; }
	uint getIndex() const { return index; }
	string getName() const { return name; }
	void incrementSize() { size++; }
	void decrementSize() { size--; }
private:
	uint size;
	uint index;
	std::string name;
};

} /*namespace dlx*/

#endif /*COLUMN_H_*/

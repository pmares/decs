#ifndef DLX_COLUMN_H_
#define DLX_COLUMN_H_

#include <string>
#include "node.h"
#include "util.h"

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

#endif /*DLX_COLUMN_H_*/

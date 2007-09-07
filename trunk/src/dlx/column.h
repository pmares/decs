#ifndef COLUMN_H_
#define COLUMN_H_

#include <string>
#include "node.h"

namespace dlx
{

class Column : public Node
{
public:
	Column();
	virtual ~Column();
	unsigned int getSize() const { return size; }
	unsigned int getIndex() const { return index; }
	std::string getName() const { return name; }
	void incrementSize() { size++; }
	void decrementSize() { size--; }
private:
	unsigned int size;
	unsigned int index;
	std::string name;
};

}

#endif /*COLUMN_H_*/

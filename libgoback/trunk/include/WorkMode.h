#ifndef WORKMODE_H
#define WORKMODE_H

#include <string>
#include <list>

class DataSource;

class ViewBlock {
public:
	ViewBlock(std::string std, bool editable);

	std::string _str;
	bool _editable;
};

typedef std::list<ViewBlock> ViewLine;

class WorkMode {
public:
	static WorkMode *create(std::string name, DataSource *ds);

	WorkMode(DataSource *ds);

	virtual int getNumberLines() = 0;
	virtual ViewLine getLine(int line) = 0;

protected:
	DataSource *_dataSource;
};

#endif // WORKMODE_H

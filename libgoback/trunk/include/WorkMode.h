#ifndef WORKMODE_H
#define WORKMODE_H

#include <Module.h>

#include <list>

class DataSource;

class ViewBlock {
public:
	ViewBlock(std::string str, bool editable) : _str(str), _editable(editable) {}

	std::string _str;
	bool _editable;
};

typedef std::list<ViewBlock> ViewLine;

class WorkMode {
public:
	static WorkMode *create(std::string id, DataSource *ds);

	WorkMode(DataSource *ds);
	virtual ~WorkMode() {}

	virtual unsigned int getNumberLines() = 0;
	virtual ViewLine getLine(unsigned int line) = 0;

protected:
	DataSource *_dataSource;
};

// WorkMode module
class WorkModeModule : public Module {
public:
	// Get the applicable format
	virtual std::string getApplicableFormat() const = 0;

	// WorkMode instantiation
	virtual WorkMode *create(DataSource *ds) const = 0;
};

#endif // WORKMODE_H

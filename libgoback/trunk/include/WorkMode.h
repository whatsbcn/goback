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
	virtual bool setDataSource(DataSource *ds);
	virtual int getNumberLines() = 0;
	virtual ViewLine getLine(int line) = 0;

protected:
	DataSource *_dataSource;
};

#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H

// TODO: Use Module instead
#include <DataFormat.h>

#include <list>

typedef std::list<DataFormatModule *> DataFormatModules;

// Module manager
class ModuleManager {
public:
	static ModuleManager *getInstance();
	~ModuleManager();

	DataFormatModules getDataFormats() const;

private:
	ModuleManager();
	static ModuleManager *_instance;

	DataFormatModules _dataFormats;
};

#endif // MODULEMANAGER_H

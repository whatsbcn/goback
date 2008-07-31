#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H

// TODO: Use Module instead
#include <DataFormat.h>
#include <WorkMode.h>

#include <list>

typedef std::list<DataFormatModule *> DataFormatModules;
typedef std::list<WorkModeModule *> WorkModeModules;

// Module manager
class ModuleManager {
public:
	static ModuleManager *getInstance();
	~ModuleManager();

	DataFormatModules getDataFormats() const;
	WorkModeModules getWorkModes() const;

private:
	ModuleManager();
	static ModuleManager *_instance;

	DataFormatModules _dataFormats;
	WorkModeModules _workModes;
};

#endif // MODULEMANAGER_H

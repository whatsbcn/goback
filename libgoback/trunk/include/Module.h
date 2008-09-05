#ifndef MODULE_H
#define MODULE_H

#include <string>

// Generic module
class Module {
public:
	virtual ~Module() {}

	// Identification
	virtual std::string id() const = 0;
	virtual std::string name() const = 0;
};

#endif // MODULE_H

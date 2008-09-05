#ifndef VALUE_H
#define VALUE_H

#include <string>

class Value {
public:
	virtual ~Value() {}

	//virtual bool set(DataSource *ds, unsigned int offset) = 0;
	//virtual bool set(unsigned char *buf, unsigned int size) = 0;

	virtual std::string getString() = 0;
	virtual long int getInt() = 0;

	//virtual ValueType *getValueType() = 0;
};

class ValueString : public Value {
public:
	ValueString(std::string value);

	std::string getString();
	long int getInt();

	//std::string getValue();

protected:
	std::string _value;
};

class ValueInt : public Value {
public:
	ValueInt(long int value);

	std::string getString();
	long int getInt();

	//long int getValue();

protected:
	long int _value;
};

#endif // VALUE_H

#include "Value.h"

ValueString::ValueString(std::string value) :
	_value(value) {
}

std::string ValueString::getString() {
	return _value;
}

long int ValueString::getInt() {
	// TODO: atol?
	return 0;
}

ValueInt::ValueInt(long int value) :
	_value(value) {
}

std::string ValueInt::getString() {
	// TODO: sprintf?
	return "";
}

long int ValueInt::getInt() {
	return _value;
}

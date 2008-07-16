#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "BufferWindow.h"

#include <string>

class StatusBar {
public:
	StatusBar();

	void update(BufferWindow *win);

	std::string getCommand();
	//void showMessage(std::string msg);

private:
	std::string _message;
};

#endif // STATUSBAR_H

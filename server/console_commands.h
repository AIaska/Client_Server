#ifndef _CONSOLE_COMMANDS_H_INCLUDED_
#define _CONSOLE_COMMANDS_H_INCLUDED_

#include <map>

#ifdef _WIN64
#include "socket_utils/socket_helper_wnd.h"
#endif

using namespace std;

class CConsoleCommands
{
public:
	CConsoleCommands();
	~CConsoleCommands();

	bool Process();

private:
	CServerSocketHelper socketHelper;
};

#endif // _CONSOLE_COMMANDS_H_INCLUDED_
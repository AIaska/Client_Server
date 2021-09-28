#include "rapidxml/rapidxml_utils.hpp"

#include "console_commands.h"

using namespace rapidxml;

CConsoleCommands::CConsoleCommands()
{

};

CConsoleCommands::~CConsoleCommands()
{

};

bool CConsoleCommands::Process()
{
	try
	{
		// add cout about usage
		string sInput = "";
		cin >> sInput;

		if (sInput == "init")
		{
			socketHelper.Init();
			cout << "Socket initiated\n";
			return true;
		}

		if (sInput == "listen")
		{
			socketHelper.Listen();
			cout << "Listener socket was setup\n";
			return true;
		}

		if (sInput == "accept")
		{
			socketHelper.Accept();
			cout << "Permitted an incoming connection attempt\n";
			return true;
		}

		if (sInput == "receive")
		{
			socketHelper.Receive(sInput);
			cout << "Server received: " << sInput << '\n';
			return true;
		}

		if (sInput == "send")
		{
			cin >> sInput;
			socketHelper.Send(sInput, sInput.length());
			cout << "Server sent msg: " << sInput << '\n';
			
			return true;
		}

		if (sInput == "shutdown")
		{
			socketHelper.Shutdown();
			cout << "Server was shutdown\n";
			return false;
		}
	}
	catch (const exception& ex)
	{
		cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
	}
}
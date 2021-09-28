#ifndef _LOGGER_H_INCLUDED_
#define _LOGGER_H_INCLUDED_

#include <iostream>
#include <string>

using namespace std;

class Logger
{
public:
    static void to_console(string sMsg)
    {
        cout << sMsg << "\n";
    }
};

#endif // _LOGGER_H_INCLUDED_

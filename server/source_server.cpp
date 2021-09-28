#ifdef _WIN64
#include "socket_utils/socket_helper_wnd.h"
#endif

#include "commands.h"


int main()
{
    try
    {
        CServerSocketHelper socketHelper;
        socketHelper.Init();
        socketHelper.Listen();
        socketHelper.Accept();

        CServerCommands* serverCmds = new CServerCommands();
        xml_document<> XMLDoc;
        string sReceiveMsg = "";
        string sResponseMsg = "";

        while(true)
        {
            socketHelper.Receive(sReceiveMsg);
            serverCmds->Parse(sReceiveMsg);
            serverCmds = serverCmds->GetCmdClass();
            serverCmds->BuildResponseXML(XMLDoc);
            print(back_inserter(sResponseMsg), XMLDoc, 0);
            socketHelper.Send(sResponseMsg.c_str(), sResponseMsg.length());
        }



    }
    catch (const exception& ex)
    {
        cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
    }




    int i;
    std::cin >> i;
    
    return 0;
}

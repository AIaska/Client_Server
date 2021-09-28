#ifdef _WIN64
#include "socket_utils/socket_helper_wnd.h"
#endif

#include "rapidxml/rapidxml_ext.hpp"

using namespace std;
using namespace rapidxml;

int main(int argc, char** argv)
{
    try
    {
        if (argc != 2) {
            printf("usage: %s server-name\n", argv[0]);
            //return 1;
        }

        CClientSocketHelper socketHelper;

        socketHelper.Init("127.0.0.1"/*argv[2]*/);
        socketHelper.Connect();


        xml_document<> XMLDoc;
        xml_node<>* XMLNodeRequest = XMLDoc.allocate_node(node_element, "request");
        XMLNodeRequest->append_node(XMLDoc.allocate_node(node_element, "command", "test"));

        xml_node<>* XMLNodeParams1 = XMLDoc.allocate_node(node_element, "params");
        xml_attribute<>* XMLAttr = XMLDoc.allocate_attribute("name", "1");
        XMLNodeParams1->append_attribute(XMLAttr);
        XMLAttr = XMLDoc.allocate_attribute("value", "test value 1");
        XMLNodeParams1->append_attribute(XMLAttr);
        XMLNodeRequest->append_node(XMLNodeParams1);

        xml_node<>* XMLNodeParams2 = XMLDoc.allocate_node(node_element, "params");
        XMLAttr = XMLDoc.allocate_attribute("name", "2");
        XMLNodeParams2->append_attribute(XMLAttr);
        XMLAttr = XMLDoc.allocate_attribute("value", "test value 2");
        XMLNodeParams2->append_attribute(XMLAttr);
        XMLNodeRequest->append_node(XMLNodeParams2);


        XMLDoc.append_node(XMLNodeRequest);

        cout << "test request to be sent:\n" << XMLDoc;

        string strDoc = "";
        print(back_inserter(strDoc), XMLDoc, 0);

        //for (int i = 0; i < 5; i++)
        //{
            socketHelper.Send(strDoc.c_str());
            socketHelper.Receive();
        //}

        //socketHelper.Shutdown();
    }
    catch (const exception& ex)
    {
        cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
    }

    return 0;
}

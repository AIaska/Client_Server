#include "commands.h"

void CServerCommands::Parse(const string& sData)
{
    int iLength = sData.length() + 1;
    char* szData = new char[iLength];
    strcpy_s(szData, iLength, sData.c_str());
	m_XMLDoc.parse<0>(szData);

    cout << "received request:\n" << m_XMLDoc;
}

CServerCommands* CServerCommands::GetCmdClass()
{
	xml_node<>* XMLNodeCmd = m_XMLDoc.first_node("request");
    XMLNodeCmd = XMLNodeCmd->first_node("command");

    cout << XMLNodeCmd->value();
	if (strcmp(XMLNodeCmd->value(), "test") == 0)
		return new CTestCmd;

	if (strcmp(XMLNodeCmd->value(), "get version") == 0)
		//return new CGetVersionCmd;

	if (strcmp(XMLNodeCmd->value(), "get server stat") == 0)
		//return new CGetStatCmd;

	return nullptr;
}

void CServerCommands::BuildResponseXML(xml_document<>& XMLDoc)
{

}

void CTestCmd::BuildResponseXML(xml_document<>& XMLDoc)
{
    XMLDoc.clear();
    xml_node<>* XMLNodeResponse = XMLDoc.allocate_node(node_element, "response");

    xml_node<>* XMLNodeRequest = m_XMLDoc.first_node("params");
    xml_attribute<>* XMLAttrRequest = XMLNodeResponse->first_attribute();
    xml_node<>* XMLNodeResponseParams;

    for ( ; XMLAttrRequest != NULL; XMLAttrRequest = XMLAttrRequest->next_attribute())
    {
        XMLNodeResponseParams = XMLDoc.allocate_node(node_element, XMLAttrRequest->name(), XMLAttrRequest->value());
        XMLNodeResponse->append_node(XMLNodeResponseParams);
    }

    XMLDoc.append_node(XMLNodeResponse);

    cout << "formed response:\n" << XMLDoc;
    int i;
    cin >> i;
}
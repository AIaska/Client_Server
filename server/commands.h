#ifndef _COMMANDS_H_INCLUDED_
#define _COMMANDS_H_INCLUDED_

#include <iostream>
#include "rapidxml/rapidxml_ext.hpp"

using namespace std;
using namespace rapidxml;

class CServerCommands
{
public:
	CServerCommands() = default;
	virtual ~CServerCommands() = default;

public:
	void Parse(const string& sData);
	CServerCommands* GetCmdClass();

	virtual void BuildResponseXML(xml_document<>& XMLDoc);
	
	xml_document<> m_XMLDoc;
};

class CTestCmd : public CServerCommands
{
public:
	CTestCmd() = default;
	~CTestCmd() = default;

	void BuildResponseXML(xml_document<>& XMLDoc) override;
};

class CGetVersionCmd : public CServerCommands
{
public:
	CGetVersionCmd() = default;
	~CGetVersionCmd() = default;

	void BuildResponseXML(xml_document<>& XMLDoc) override;
};

class CGetStatCmd : public CServerCommands
{
public:
	CGetStatCmd() = default;
	~CGetStatCmd() = default;

	void BuildResponseXML(xml_document<>& XMLDoc) override;
};

#endif // _COMMANDS_H_INCLUDED_


#include "socket_helper_wnd.h"

#include "rapidxml/rapidxml_ext.hpp"

using namespace rapidxml;

CServerSocketHelper::~CServerSocketHelper()
{
	closesocket(m_clientSocket);
	int iRes = WSACleanup();
	if (iRes == SOCKET_ERROR)
	{
		cout << "WSACleanup error \n";
	}
}

int CServerSocketHelper::Init()
{
	try
	{
		// Init Winsock
		int iResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		if (iResult != 0) {
			cout << "WSAStartup failed with error: %d\n" << iResult << "\n";
			return iResult;
		}

		SecureZeroMemory(&m_hints, sizeof(m_hints));
		m_hints.ai_family = AF_INET;
		m_hints.ai_socktype = SOCK_STREAM;
		m_hints.ai_protocol = IPPROTO_TCP;
		m_hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		iResult = getaddrinfo(NULL, DEFAULT_PORT, &m_hints, &m_pResult);
		if (iResult != 0) {
			cout << "getaddrinfo failed with error: " << iResult << "\n";
			return iResult;
		}

		return iResult;
	}
	catch (const exception& ex)
	{
		cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
		int iRes = WSACleanup();
		if (iRes == SOCKET_ERROR)
		{
			cout << "WSACleanup error \n";
		}
	}
}

int CServerSocketHelper::Listen()
{
	try
	{
		m_listenerSocket = socket(m_pResult->ai_family, m_pResult->ai_socktype, m_pResult->ai_protocol);
		if (m_listenerSocket == INVALID_SOCKET) {
			cout << "socket failed with error: " << WSAGetLastError() << "\n";
			freeaddrinfo(m_pResult);
			return INVALID_SOCKET;
		}

		// Setup the TCP listening socket
		int iResult = bind(m_listenerSocket, m_pResult->ai_addr, (int)m_pResult->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			cout << "bind failed with error: " << WSAGetLastError() << "\n";
			freeaddrinfo(m_pResult);
			closesocket(m_listenerSocket);
			return iResult;
		}

		freeaddrinfo(m_pResult);

		iResult = listen(m_listenerSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			cout << "listen failed with error: " << WSAGetLastError() << "\n";
			closesocket(m_listenerSocket);
			return iResult;
		}

		return iResult;
	}
	catch (const exception& ex)
	{
		cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
		freeaddrinfo(m_pResult);
		closesocket(m_listenerSocket);
	}
}

bool CServerSocketHelper::Accept()
{
	try
	{
		// Permit an incoming connection attempt on a socket
		m_clientSocket = accept(m_listenerSocket, NULL, NULL);
		if (m_clientSocket == INVALID_SOCKET) {
			cout << "accept failed with error: " << WSAGetLastError() << "\n";
			closesocket(m_listenerSocket);
			return false;
		}

		closesocket(m_listenerSocket);
		return true;
	}
	catch (const exception& ex)
	{
		cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
		closesocket(m_listenerSocket);
	}
}

int CServerSocketHelper::Receive(string& sReceived, const int iMaxRetryNum)
{
	try
	{
		int iResult = 0;
		char szRecvBuf[DEFAULT_BUFLEN];

		for (int i = 0; i <= iMaxRetryNum; i++)
		{
			iResult = recv(m_clientSocket, szRecvBuf, DEFAULT_BUFLEN, 0);
			if (iResult > 0)
			{
				cout << "attempt num " << i + 1 << ": bytes received: " << iResult << "\n";
				cout << "iResult: " << iResult << '\n';

				sReceived = szRecvBuf;
				sReceived = sReceived.substr(0, iResult);

				return iResult;
			}
			else if (iResult == 0)
			{
				cout << "attempt num " << i + 1 << ": no data received.\n";

				return iResult;
			}
			else
			{
				cout << "attempt num " << i + 1 << ": recv failed with error: " << WSAGetLastError() << "\n";
				Sleep(100);
				continue;
			}
		}

		return iResult;
	}
	catch (const exception& ex)
	{
		cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
	}
}

int CServerSocketHelper::Send(const string& sMsg, const int icNumOfBytes, const int iMaxRetryNum)
{
	try
	{
		int iSendResult = 0;

		for (int i = 0; i <= iMaxRetryNum; i++)
		{
			iSendResult = send(m_clientSocket, sMsg.c_str(), icNumOfBytes, 0);
			if (iSendResult == SOCKET_ERROR)
			{
				cout << "attempt num " << i + 1 << ": send failed with error: " << WSAGetLastError() << "\n";
				Sleep(100);
				continue;
			}
			else
			{
				cout << "attempt num " << i + 1 << ": bBytes sent: " << iSendResult << '\n';
				return iSendResult;
			}
		}

		return iSendResult;
	}
	catch (const exception& ex)
	{
		cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
	}
}

int CServerSocketHelper::Shutdown()
{
	try
	{
		int iResult = shutdown(m_clientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			cout << "shutdown failed with error: " << WSAGetLastError() << "\n";
			closesocket(m_clientSocket);
			int iRes = WSACleanup();
			if (iRes == SOCKET_ERROR)
			{
				cout << "WSACleanup error \n";
			}
			return iResult;
		}

		closesocket(m_clientSocket);
		return iResult;
	}
	catch (const exception& ex)
	{
		cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
		closesocket(m_clientSocket);
		int iRes = WSACleanup();
		if (iRes == SOCKET_ERROR)
		{
			cout << "WSACleanup error \n";
		}
	}
}

//----------------------------------------------------------

CClientSocketHelper::~CClientSocketHelper()
{
	closesocket(m_connectSocket);
	int iRes = WSACleanup();
	if (iRes == SOCKET_ERROR)
	{
		cout << "WSACleanup error \n";
	}
}

int CClientSocketHelper::Init(const std::string& scIpAdr)
{
	try
	{
		// Init Winsock
		int iResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		if (iResult != 0) {
			cout << "WSAStartup failed with error: " << iResult << "\n";
			int iRes = WSACleanup();
			if (iRes == SOCKET_ERROR)
			{
				cout << "WSACleanup error \n";
			}
			return iResult;
		}

		SecureZeroMemory(&m_hints, sizeof(m_hints));
		m_hints.ai_family = AF_UNSPEC;
		m_hints.ai_socktype = SOCK_STREAM;
		m_hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		const char* szcIpAdr = scIpAdr.c_str();
		iResult = getaddrinfo(szcIpAdr, DEFAULT_PORT, &m_hints, &m_pResult);
		if (iResult != 0) {
			cout << "getaddrinfo failed with error: " << iResult << "\n";
			return iResult;
		}

		return iResult;
	}
	catch (const exception& ex)
	{
		cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
		int iRes = WSACleanup();
		if (iRes == SOCKET_ERROR)
		{
			cout << "WSACleanup error \n";
		}
	}
}

int CClientSocketHelper::Connect()
{
	try
	{
		int iResult = 0;

		for (m_pAdrInfo = m_pResult; m_pAdrInfo != NULL; m_pAdrInfo = m_pAdrInfo->ai_next)
		{
			m_connectSocket = socket(m_pAdrInfo->ai_family, m_pAdrInfo->ai_socktype, m_pAdrInfo->ai_protocol);
			if (m_connectSocket == INVALID_SOCKET)
			{
				cout << "socket failed with error: " << WSAGetLastError() << "\n";
				return iResult;
			}

			iResult = connect(m_connectSocket, m_pAdrInfo->ai_addr, (int)m_pAdrInfo->ai_addrlen);
			if (iResult == SOCKET_ERROR)
			{
				closesocket(m_connectSocket);
				m_connectSocket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(m_pResult);

		if (m_connectSocket == INVALID_SOCKET) {
			cout << "Unable to connect to server!\n";
			return iResult;
		}

		return iResult;
	}
	catch (const exception& ex)
	{
		cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
		closesocket(m_connectSocket);
		m_connectSocket = INVALID_SOCKET;
	}
}

int CClientSocketHelper::Send(const string& sMsg, const int iMaxRetryNum)
{
	try
	{
		int iResult = 0;

		for (int i = 0; i <= iMaxRetryNum; i++)
		{
			iResult = send(m_connectSocket, sMsg.c_str(), (int)strlen(sMsg.c_str()), 0);
			if (iResult == SOCKET_ERROR) {
				cout << "attempt num " << i + 1 << ": send failed with error: " << WSAGetLastError() << "\n";
				Sleep(100);
				continue;
			}
			else
			{
				cout << "attempt num " << i + 1 << ": bytes sent: " << iResult << '\n';
				return iResult;
			}
		}

		return iResult;
	}
	catch (const exception& ex)
	{
		cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
		closesocket(m_connectSocket);
	}
}

void CClientSocketHelper::Receive(const int iMaxRetryNum)
{
	try
	{
		int iResult = 0;
		char szRecvBuf[DEFAULT_BUFLEN];

		for (int i = 0; i <= iMaxRetryNum; i++)
		{
			iResult = recv(m_connectSocket, szRecvBuf, DEFAULT_BUFLEN, 0);

			xml_document<> XMLDoc;
			XMLDoc.parse<0>(szRecvBuf);
			cout << "attempt num " << i + 1 << ": received response:\n" << XMLDoc;

			if (iResult > 0)
			{
				cout << "attempt num " << i + 1 << ": bytes received: " << iResult << "\n";
				return;
			}
			else if (iResult == 0)
			{
				cout << "attempt num " << i + 1 << ": connection closed\n";
				return;
			}
			else
			{
				cout << "attempt num " << i + 1 << ": recv failed with error: " << WSAGetLastError() << "\n";
				Sleep(100);
				continue;
			}
		}
	}
	catch (const exception& ex)
	{
		cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
	}
}

int CClientSocketHelper::Shutdown()
{
	try
	{
		int iResult = shutdown(m_connectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			cout << "shutdown failed with error: " << WSAGetLastError() << "\n";
			closesocket(m_connectSocket);
			int iRes = WSACleanup();
			if (iRes == SOCKET_ERROR)
			{
				cout << "WSACleanup error \n";
			}
			return iResult;
		}

		closesocket(m_connectSocket);
		return iResult;
	}
	catch (const exception& ex)
	{
		cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
		closesocket(m_connectSocket);
		int iRes = WSACleanup();
		if (iRes == SOCKET_ERROR)
		{
			cout << "WSACleanup error \n";
		}
	}
}

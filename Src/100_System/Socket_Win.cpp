#include "stdafx.h"
#include <Windows.h>
#include "Socket.h"
#include "Environment.h"
#include "Information.h"
#include "Log.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment (lib, "crypt32.lib")

struct ST_WSA_INITIALIZER
{
	ST_WSA_INITIALIZER(void)	{	WSAData wsaData;	::WSAStartup(MAKEWORD(2, 2), &wsaData);	}
	~ST_WSA_INITIALIZER(void)	{	::WSACleanup();	}
}	g_stWSAInitializer;

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	SOCKET socket(int af, int type, int protocol)
	{
		return (SOCKET)::socket(af, type, protocol);
	}

	//////////////////////////////////////////////////////////////////////////
	int bind(SOCKET s, DWORD dwIP, WORD wPort)
	{
		sockaddr_in service;
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = dwIP;
		service.sin_port = NetworkFromHost(wPort);
		return ::bind((::SOCKET)s, (SOCKADDR*)&service, sizeof(service));
	}

	//////////////////////////////////////////////////////////////////////////
	int listen(SOCKET s, int nPendingCount)
	{
		return ::listen((::SOCKET)s, nPendingCount);
	}

	//////////////////////////////////////////////////////////////////////////
	SOCKET accept(SOCKET s, ST_ACCEPT_INFO* pOutInfo)
	{
		if( NULL == pOutInfo )
			return (SOCKET)::accept((::SOCKET)s, NULL, 0);

		sockaddr_in stInfo = { 0, };
		int nInfoLen = (int)sizeof(stInfo);
		SOCKET newSocket = (SOCKET)::accept((::SOCKET)s, (sockaddr*)&stInfo, &nInfoLen);
		if (INVALID_SOCKET == newSocket)
			return INVALID_SOCKET;

		pOutInfo->dwIP = stInfo.sin_addr.S_un.S_addr;
		pOutInfo->dwFamily = stInfo.sin_family;
		return newSocket;
	}

	//////////////////////////////////////////////////////////////////////////
	int shutdown(SOCKET s, int how)
	{
		return ::shutdown((::SOCKET)s, how);
	}

	//////////////////////////////////////////////////////////////////////////
	int closesocket(SOCKET s)
	{
		return ::closesocket((::SOCKET)s);
	}

	//////////////////////////////////////////////////////////////////////////
	int connect(SOCKET s, DWORD dwIP, WORD wPort)
	{
		sockaddr_in service;
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = dwIP;
		service.sin_port = NetworkFromHost(wPort);
		return ::connect((::SOCKET)s, (SOCKADDR*)&service, sizeof(service));
	}

	//////////////////////////////////////////////////////////////////////////
	int setsockopt(SOCKET s, int level, int optname, const char* optval, int optlen)
	{
		return ::setsockopt((::SOCKET)s, level, optname, optval, optlen);
	}

	//////////////////////////////////////////////////////////////////////////
	int recv(SOCKET s, char* buf, int len, int flags)
	{
		int nRet = ::recv((::SOCKET)s, buf, len, flags);
		if( (SOCKET_ERROR == nRet) && (WSAETIMEDOUT == ::GetLastError()) )
			::SetLastError(EC_TIMEOUT);
		if( 0 == nRet )
			::SetLastError(EC_DISCONNECTED);
		return nRet;
	}

	//////////////////////////////////////////////////////////////////////////
	int send(SOCKET s, const char* buf, int len, int flags)
	{
		int nRet = ::send((::SOCKET)s, buf, len, flags);
		if( (SOCKET_ERROR == nRet) && (WSAETIMEDOUT == ::GetLastError()) )
			::SetLastError(EC_TIMEOUT);
		return nRet;
	}

	//////////////////////////////////////////////////////////////////////////
	int sendto(SOCKET s, const char* buf, int len, int flags, DWORD dwIP, WORD wPort)
	{
		struct sockaddr_in stAddress = { 0, };
		stAddress.sin_family = AF_INET;
		stAddress.sin_port = NetworkFromHost(wPort);
		stAddress.sin_addr.s_addr = dwIP;
		int nRet = ::sendto(s, buf, len, flags, (sockaddr*)&stAddress, sizeof(stAddress));
		if ((SOCKET_ERROR == nRet) && (WSAETIMEDOUT == ::GetLastError()))
			::SetLastError(EC_TIMEOUT);
		return nRet;
	}

	//////////////////////////////////////////////////////////////////////////
	int recvfrom(SOCKET s, char* buf, int len, int flags, ST_SOURCE_INFO* pSourceInfo)
	{
		struct sockaddr_in stAddress = { 0, };
		int nLen = sizeof(stAddress);
		int nRet = ::recvfrom(s, buf, len, 0, (sockaddr*)&stAddress, &nLen);
		if ((SOCKET_ERROR == nRet) && (WSAETIMEDOUT == ::GetLastError()))
			::SetLastError(EC_TIMEOUT);

		if( pSourceInfo )
		{
			pSourceInfo->dwIP = (DWORD)stAddress.sin_addr.S_un.S_addr;
			pSourceInfo->wPort = HostFromNetwork(stAddress.sin_port);
		}
		return nRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string GetHostByName(std::string strDomain)
	{
		try
		{
			struct hostent* pHost = ::gethostbyname(strDomain.c_str());
			if (pHost == NULL)
				throw exception_format("Failed to gethostbyname()");

			while (*pHost->h_addr_list != NULL)
			{
				struct in_addr* pAddr = (in_addr*)(*pHost->h_addr_list);
				//pHost->h_addr_list++;

				std::string strIp = IPAddressFromA(*(DWORD*)pAddr);
				return strIp;
			}
		}
		catch (std::exception& e)
		{
			Log_Debug("%s\n", e.what());
			return strDomain;
		}

		return strDomain;
	}
}
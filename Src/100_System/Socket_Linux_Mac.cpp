#include "stdafx.h"
#include "Socket.h"
#include "Environment.h"
#include "Information.h"
#include "Log.h"
#include <unistd.h>
#include <cerrno>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	SOCKET socket(int af, int type, int protocol)
	{
	    protocol;
		//return (SOCKET)::socket(af, type, protocol);	// it may fail to connect TCP with protocol:IPPROTO_TCP

		int nSocketFD = ::socket(af, type, 0);

#ifdef __APPLE__
		int nNoSigPipe = 1;
        if ( ::setsockopt(nSocketFD, SOL_SOCKET, SO_NOSIGPIPE, &nNoSigPipe, sizeof (nNoSigPipe)) < 0)
        {
            perror ("setsockopt(,,SO_NOSIGPIPE)");
        }
#endif
		return (SOCKET)nSocketFD;
	}

	//////////////////////////////////////////////////////////////////////////
	int bind(SOCKET s, DWORD dwIP, WORD wPort)
	{
		sockaddr_in stSockAddr = { 0, };
		stSockAddr.sin_family = AF_INET;
		stSockAddr.sin_addr.s_addr = dwIP;
		stSockAddr.sin_port = htons((short)wPort);
		return ::bind((int)s, (sockaddr*)&stSockAddr, sizeof(stSockAddr));
	}

	//////////////////////////////////////////////////////////////////////////
	int listen(SOCKET s, int nPendingCount)
	{
		return ::listen((int)s, nPendingCount);
	}

	//////////////////////////////////////////////////////////////////////////
	SOCKET accept(SOCKET s, ST_ACCEPT_INFO* pOutInfo)
	{
		if( NULL == pOutInfo )
			return (SOCKET)::accept((int)s, NULL, 0);

		sockaddr_in stInfo = { 0, };
		socklen_t tInfoLen = sizeof(stInfo);
		SOCKET newSocket = (SOCKET)::accept((int)s, (sockaddr*)&stInfo, &tInfoLen);
		pOutInfo->dwIP = (DWORD)stInfo.sin_addr.s_addr;
		pOutInfo->dwFamily = stInfo.sin_family;
		return newSocket;
	}

	//////////////////////////////////////////////////////////////////////////
	int shutdown(SOCKET s, int how)
	{
		return ::shutdown((int)s, how);
	}

	//////////////////////////////////////////////////////////////////////////
	int closesocket(SOCKET s)
	{
		::shutdown((int)s, SHUT_RDWR);	// to terminate block operations like listen, recv
		return ::close((int)s);	
	}

	//////////////////////////////////////////////////////////////////////////
	int connect(SOCKET s, DWORD dwIP, WORD wPort)
	{
		sockaddr_in service = { 0, };
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = dwIP;
		service.sin_port = NetworkFromHost(wPort);
		return ::connect((int)s, (sockaddr*)&service, sizeof(service));
	}

	//////////////////////////////////////////////////////////////////////////
	int setsockopt(SOCKET s, int level, int optname, const char* optval, int optlen)
	{
		if( SO_RCVTIMEO == optname || SO_SNDTIMEO == optname)
		{
			DWORD dwTimeOut = *(DWORD*)optval;
			struct timeval tv = { dwTimeOut / 1000, ((int)dwTimeOut % 1000) * 1000 };
			return ::setsockopt((::SOCKET)s, level, optname, &tv, sizeof(tv));
		}
		return ::setsockopt((::SOCKET)s, level, optname, optval, optlen);
	}

	//////////////////////////////////////////////////////////////////////////
	int recv(SOCKET s, char* buf, int len, int flags)
	{
		int nRet = ::recv((int)s, buf, len, flags);
		if( (nRet < 0) && (EAGAIN == errno) )
			errno = EC_TIMEOUT;
		if( 0 == nRet )
			errno = EC_DISCONNECTED;
		return nRet;
	}

	//////////////////////////////////////////////////////////////////////////
	int send(SOCKET s, const char* buf, int len, int flags)
	{
		int nRet = ::send((int)s, buf, len, flags);
		if( (nRet < 0) && (EAGAIN == errno) )
			errno = EC_TIMEOUT;
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
		if ((nRet < 0) && (EAGAIN == errno))
			errno = EC_TIMEOUT;
		return nRet;
	}

	//////////////////////////////////////////////////////////////////////////
	int recvfrom(SOCKET s, char* buf, int len, int flags, ST_SOURCE_INFO* pSourceInfo)
	{
	    flags;
		struct sockaddr_in stAddress = { 0, };
		socklen_t tLen = sizeof(stAddress);
		int nRet = ::recvfrom(s, buf, len, 0, (sockaddr*)&stAddress, &tLen);
		if ((nRet < 0))
		{
			if (EAGAIN == errno)				
				errno = EC_TIMEOUT;			
		}
		else if (0 == nRet)
			errno = EC_DISCONNECTED;
		else
		{			
			if( pSourceInfo )
			{
				pSourceInfo->dwIP = (DWORD)stAddress.sin_addr.s_addr;
				pSourceInfo->wPort = HostFromNetwork(stAddress.sin_port);
			}
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

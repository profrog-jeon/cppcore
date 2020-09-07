#pragma once

#include "../__Common/Type.h"

typedef size_t				SOCKET;

namespace core
{
	struct ST_ACCEPT_INFO
	{
		DWORD dwIP;
		DWORD dwFamily;
	};

	struct ST_SOURCE_INFO
	{
		DWORD dwIP;
		WORD wPort;
	};

	struct ST_LINGER
	{
		int l_onoff;
		int l_linger;
	};

	SOCKET	socket(int af, int type, int protocol);
	int		bind(SOCKET s, DWORD dwIP, WORD wPort);
	int		listen(SOCKET s, int nPendingCount);
	SOCKET	accept(SOCKET s, ST_ACCEPT_INFO* pOutInfo = NULL);
	int		shutdown(SOCKET s, int how);
	int		closesocket(SOCKET s);
	int		connect(SOCKET s, DWORD dwIP, WORD wPort);
	int		setsockopt(SOCKET s, int level, int optname, const char* optval, int optlen);
	int		recv(SOCKET s, char* buf, int len, int flags);
	int		send(SOCKET s, const char* buf, int len, int flags);
	int		sendto(SOCKET s, const char* buf, int len, int flags, DWORD dwIP, WORD wPort);
	int		recvfrom(SOCKET s, char* buf, int len, int flags, ST_SOURCE_INFO* pSourceInfo);

	std::string GetHostByName(std::string strDomain);
}
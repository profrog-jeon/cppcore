#include "stdafx.h"
#include "SocketCommon.h"

namespace core
{
	ECODE SetCommonSockOpt(SOCKET hSocket)	
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			ST_LINGER stLinger;
			stLinger.l_onoff = 1;
			stLinger.l_linger = 5000;
			nRet = core::setsockopt(hSocket, SOL_SOCKET_, SO_LINGER_, (char*)&stLinger, sizeof(stLinger));
			if (SOCKET_ERROR_ == nRet)
				Log_Warn("core::setsockopt(m_hSocket, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger)) failure, %d", core::GetLastError());

			int bReuseAddr = 1;
			nRet = core::setsockopt(hSocket, SOL_SOCKET_, SO_REUSEADDR_, (char*)&bReuseAddr, sizeof(bReuseAddr));
			if (SOCKET_ERROR_ == nRet)
				Log_Warn("core::setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&bReuseAddr, sizeof(bReuseAddr)) failure, %d", core::GetLastError());

			int bNodelay = 1;
			nRet = core::setsockopt(hSocket, IPPROTO_TCP_, TCP_NODELAY_, (char*)&bNodelay, sizeof(bNodelay));
			if (SOCKET_ERROR_ == nRet)
				Log_Warn("core::setsockopt(m_hSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&bNodelay, sizeof(bNodelay)) failure, %d", core::GetLastError());
		}
		catch (std::exception& e)
		{
			nRet = GetLastError();
			Log_Error("%s", e.what());
			return nRet;
		}

		return EC_SUCCESS;
	}
}

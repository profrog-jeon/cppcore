#include "stdafx.h"
#include "SyncUDPSocket.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CSyncUDPSocket::CSyncUDPSocket(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CSyncUDPSocket::~CSyncUDPSocket()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSyncUDPSocket::Listen(WORD wPort)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			m_hSocket = socket(AF_INET_, SOCK_DGRAM_, IPPROTO_UDP_);
			if (INVALID_SOCKET_ == m_hSocket)
			{
				nRet = GetLastError();
				throw exception_format("socket(AF_INET, SOCK_DGRAM_, IPPROTO_UDP_) creation failure, %s", GetErrorStringA(GetLastError()).c_str());
			}

			int bReuseAddr = 1;
			nRet = setsockopt(m_hSocket, SOL_SOCKET_, SO_REUSEADDR_, (char*)&bReuseAddr, sizeof(bReuseAddr));
			if (SOCKET_ERROR_ == nRet)
				Log_Warn("setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&bReuseAddr, sizeof(bReuseAddr)) failure, %d", GetLastError());

			if (SOCKET_ERROR_ == bind(m_hSocket, INADDR_ANY_, wPort))
			{
				nRet = GetLastError();
				throw exception_format("Create(%u) failure, %s", wPort, GetErrorStringA(GetLastError()).c_str());
			}
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			Destroy();
			return nRet;
		}

		return EC_SUCCESS;
	}

	ECODE CSyncUDPSocket::Create(void)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			m_hSocket = socket(AF_INET_, SOCK_DGRAM_, IPPROTO_UDP_);
			if (INVALID_SOCKET_ == m_hSocket)
			{
				nRet = GetLastError();
				throw exception_format("socket(AF_INET, SOCK_DGRAM_, IPPROTO_UDP_) creation failure, %s", GetErrorStringA(GetLastError()).c_str());
			}

			int bReuseAddr = 1;
			nRet = setsockopt(m_hSocket, SOL_SOCKET_, SO_REUSEADDR_, (char*)&bReuseAddr, sizeof(bReuseAddr));
			if (SOCKET_ERROR_ == nRet)
				Log_Warn("setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&bReuseAddr, sizeof(bReuseAddr)) failure, %d", GetLastError());
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			Destroy();
			return nRet;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	void CSyncUDPSocket::Destroy(void)
	{
		if (INVALID_SOCKET_ == m_hSocket)
			return;

		shutdown(m_hSocket, SD_BOTH_);
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET_;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSyncUDPSocket::Broadcast(DWORD dwIP, WORD wPort, const void* pData, size_t tDataSize)
	{
		int broadcastEnable = 1;
		int nRet = setsockopt(m_hSocket, SOL_SOCKET_, SO_BROADCAST_, (const char*)&broadcastEnable, sizeof(broadcastEnable));
		if (nRet < 0)
		{
			Log_Error("setsockopt(SOL_SOCKET, SO_BROADCAST) failure, %d(%s)", nRet, GetErrorStringA(GetLastError()).c_str());
			return GetLastError();
		}

		nRet = sendto(m_hSocket, (const char*)pData, (int)tDataSize, 0, dwIP, wPort);
		if (nRet <= 0)
		{
			Log_Error("sendto failure, %d(%s)", nRet, GetErrorStringA(GetLastError()).c_str());
			return GetLastError();
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSyncUDPSocket::SendTo(const ST_SOURCE_INFO& pSourceInfo, const void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptSent)
	{
		if (INVALID_SOCKET_ == m_hSocket)
			return EC_INVALID_DATA;
		
		int broadcastEnable = 0;
		int nRet = setsockopt(m_hSocket, SOL_SOCKET_, SO_BROADCAST_, (const char*)&broadcastEnable, sizeof(broadcastEnable));
		if (nRet < 0)
		{
			Log_Error("setsockopt(SOL_SOCKET, SO_BROADCAST) failure, %d(%s)", nRet, GetErrorStringA(GetLastError()).c_str());
			return GetLastError();
		}

		nRet = setsockopt(m_hSocket, SOL_SOCKET_, SO_SNDTIMEO_, (char*)&dwTimeOut, sizeof(dwTimeOut));
		if (SOCKET_ERROR_ == nRet)
			Log_Error("setsockopt(m_hSocket, SOL_SOCKET_, SO_SNDTIMEO, %u) failure, %s", dwTimeOut, GetErrorStringA(GetLastError()).c_str());

		nRet = sendto(m_hSocket, (const char*)pBuff, (int)tBufSize, 0, pSourceInfo.dwIP, pSourceInfo.wPort);
		if (SOCKET_ERROR_ == nRet)
		{
			nRet = GetLastError();
			if (EC_TIMEOUT == nRet)
				return EC_TIMEOUT;

			Log_Error("send(0x%08X, size:%u, timeout:%u) failure, %s", m_hSocket, tBufSize, dwTimeOut, GetErrorStringA(GetLastError()).c_str());
			return nRet;
		}

		if (ptSent)
			*ptSent = (size_t)nRet;
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSyncUDPSocket::RecvFrom(void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptRead, ST_SOURCE_INFO* pSourceInfo)
	{
		if (INVALID_SOCKET_ == m_hSocket)
			return EC_INVALID_HANDLE;

		int nRet = setsockopt(m_hSocket, SOL_SOCKET_, SO_RCVTIMEO_, (char*)&dwTimeOut, sizeof(dwTimeOut));
		if (SOCKET_ERROR_ == nRet)
			Log_Warn("setsockopt(m_hSocket, SOL_SOCKET_, SO_RCVTIMEO_, %d) failure, %s", dwTimeOut, GetErrorStringA(GetLastError()).c_str());

		ST_SOURCE_INFO stSourceInfo;
		nRet = recvfrom(m_hSocket, (char*)pBuff, (int)tBufSize, 0, &stSourceInfo);
		if (SOCKET_ERROR_ == nRet)
		{
			nRet = GetLastError();
			if (EC_TIMEOUT == nRet)
				return EC_TIMEOUT;

			//Log_Error("recv(0x%08X, size:%u, timeout:%u) failure, %d(%s)"
			//	, m_hSocket, tBufSize, dwTimeOut, GetLastError(), GetErrorStringA(GetLastError()).c_str());
			return nRet;
		}

		if (0 == nRet)
		{
			nRet = GetLastError();
			Log_Info("Connection(0x%08X) closed by remote host.", m_hSocket);
			return nRet;
		}

		if (ptRead)
			*ptRead = (size_t)nRet;

		if (pSourceInfo)
			*pSourceInfo = stSourceInfo;
		return EC_SUCCESS;
	}
}
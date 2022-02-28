#include "stdafx.h"
#include "SyncTCPSocket.h"
#include "SocketCommon.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CSyncTCPSocket::CSyncTCPSocket(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CSyncTCPSocket::~CSyncTCPSocket()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSyncTCPSocket::Accept(SOCKET hAcceptedSocket)
	{
		m_hSocket = hAcceptedSocket;

		try
		{
			int bReuseAddr = 1;
			if (SOCKET_ERROR_ == core::setsockopt(m_hSocket, SOL_SOCKET_, SO_REUSEADDR_, (char*)&bReuseAddr, sizeof(bReuseAddr)))
				throw core::exception_format("core::setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&bReuseAddr, sizeof(bReuseAddr)) failure, %d", core::GetLastError());
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			int nRet = GetLastError();
			if (INVALID_SOCKET_ != m_hSocket)
			{
				core::closesocket(m_hSocket);
				m_hSocket = INVALID_SOCKET_;
			}
			return nRet == EC_SUCCESS? EC_INTERNAL_ERROR : nRet;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSyncTCPSocket::Connect(const char* pszIP, WORD wPort, DWORD dwTimeOut)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			m_hSocket = core::socket(AF_INET_, SOCK_STREAM_, IPPROTO_TCP_);
			if (INVALID_SOCKET_ == m_hSocket)
			{
				nRet = GetLastError();
				throw exception_format("socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) creation failure, %s", GetErrorStringA(GetLastError()).c_str());
			}

			nRet = SetCommonSockOpt(m_hSocket);
			if (EC_SUCCESS != nRet)
				throw exception_format("SetCommonSockOpt failure, %d", nRet);

			if (SOCKET_ERROR_ == core::connect(m_hSocket, IPAddressFromA(pszIP), wPort))
			{
				nRet = GetLastError();
				throw exception_format("connect(%s, %u, %d) failure, %s", pszIP, wPort, dwTimeOut, GetErrorStringA(GetLastError()).c_str());
			}
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			Close();
			return nRet;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	void CSyncTCPSocket::Close(void)
	{
		if (INVALID_SOCKET_ == m_hSocket)
			return;

		core::shutdown(m_hSocket, SD_BOTH_);
		core::closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET_;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSyncTCPSocket::Send(const void * pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptSent)
	{
		if (INVALID_SOCKET_ == m_hSocket)
			return EC_INVALID_DATA;

		int nRet = core::setsockopt(m_hSocket, SOL_SOCKET_, SO_SNDTIMEO_, (char*)&dwTimeOut, sizeof(dwTimeOut));
		if (SOCKET_ERROR_ == nRet)
			Log_Error("setsockopt(m_hSocket, SOL_SOCKET_, SO_SNDTIMEO, %u) failure, %s", dwTimeOut, GetErrorStringA(GetLastError()).c_str());

		nRet = core::send(m_hSocket, (const char*)pBuff, tBufSize, 0);
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
	ECODE CSyncTCPSocket::Recv(void * pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptRead)
	{
		if (INVALID_SOCKET_ == m_hSocket)
			return EC_INVALID_HANDLE;

		int nRet = core::setsockopt(m_hSocket, SOL_SOCKET_, SO_RCVTIMEO_, (char*)&dwTimeOut, sizeof(dwTimeOut));
		if (SOCKET_ERROR_ == nRet)
			Log_Warn("setsockopt(m_hSocket, SOL_SOCKET_, SO_RCVTIMEO_, %d) failure, %s", dwTimeOut, GetErrorStringA(GetLastError()).c_str());

		nRet = core::recv(m_hSocket, (char*)pBuff, tBufSize, 0);
		if (SOCKET_ERROR_ == nRet)
		{
			nRet = GetLastError();
			if (EC_TIMEOUT == nRet)
				return EC_TIMEOUT;

			Log_Error("recv(0x%08X, size:%u, timeout:%u) failure, %d(%s)"
				, m_hSocket, tBufSize, dwTimeOut, GetLastError(), GetErrorStringA(GetLastError()).c_str());
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
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSyncTCPSocket::Peek(void * pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptRead)
	{
		if (INVALID_SOCKET_ == m_hSocket)
			return EC_INVALID_DATA;

		int nRet = core::setsockopt(m_hSocket, SOL_SOCKET_, SO_RCVTIMEO_, (char*)&dwTimeOut, sizeof(dwTimeOut));
		if (SOCKET_ERROR_ == nRet)
			Log_Error("setsockopt(m_hSocket, SOL_SOCKET_, SO_RCVTIMEO_, %u) failure, %s", dwTimeOut, GetErrorStringA(GetLastError()).c_str());

		nRet = core::recv(m_hSocket, (char*)pBuff, tBufSize, MSG_PEEK_);
		if (SOCKET_ERROR_ == nRet)
		{
			nRet = GetLastError();
			if (EC_TIMEOUT == nRet)
				return EC_TIMEOUT;

			// linux 104: Connection reset by peer
			if (104 == nRet)
				Log_Info("Connection(0x%08X) reset by remote host.", m_hSocket);
			else
				Log_Error("recv(0x%08X, size:%u, timeout:%u, MSG_PEEK_) failure, %d(%s)"
					, m_hSocket, tBufSize, dwTimeOut, GetLastError(), GetErrorStringA(GetLastError()).c_str());
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
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSyncTCPSocket::SendForcely(const void* pSrc, size_t tLength, DWORD dwTimeOut, DWORD dwMTUSize)
	{
		ECODE nRet;
		LPCBYTE pBuffer = (LPCBYTE)pSrc;
		const DWORD dwTimeOutFraction = dwTimeOut / 2;
		try
		{
			DWORD dwTimeOutTick = GetTickCount() + dwTimeOut;

			size_t tTotalSent = 0;
			nRet = Send(pBuffer, MIN(tLength, dwMTUSize), dwTimeOutFraction, &tTotalSent);
			if (EC_SUCCESS != nRet && EC_TIMEOUT != nRet)
				throw exception_format(TEXT("Send failure, %d"), nRet);

			while (tTotalSent < tLength)
			{
				Sleep(0);
				if (dwTimeOutTick < GetTickCount())
					return EC_TIMEOUT;

				size_t tRemained = tLength - tTotalSent;
				size_t tSent = 0;
				nRet = Send(pBuffer + tTotalSent, MIN(tRemained, dwMTUSize), dwTimeOutFraction, &tSent);
				if (EC_SUCCESS != nRet && EC_TIMEOUT != nRet)
					throw exception_format(TEXT("Send failure, %d"), nRet);

				tTotalSent += tSent;
			}
		}
		catch (std::exception & e)
		{
			Log_Error("%s", e.what());
			return nRet;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSyncTCPSocket::RecvForcely(void* pDest, size_t tLength, DWORD dwTimeOut)
	{
		ECODE nRet;
		LPBYTE pBuffer = (LPBYTE)pDest;
		const DWORD dwTimeOutFraction = dwTimeOut / 2;
		try
		{
			DWORD dwTimeOutTick = GetTickCount() + dwTimeOut;

			size_t tTotalRecved = 0;
			nRet = Recv(pBuffer, tLength, dwTimeOutFraction, &tTotalRecved);
			if (EC_SUCCESS != nRet && EC_TIMEOUT != nRet)
				throw exception_format(TEXT("Recv failure, %d"), nRet);

			while (tTotalRecved < tLength)
			{
				Sleep(0);
				if (dwTimeOutTick < GetTickCount())
					return EC_TIMEOUT;

				size_t tRemained = tLength - tTotalRecved;
				size_t tRecved = 0;
				nRet = Recv(pBuffer + tTotalRecved, tRemained, dwTimeOutFraction, &tRecved);
				if (EC_SUCCESS != nRet && EC_TIMEOUT != nRet)
					throw exception_format(TEXT("Recv failure, %d"), nRet);

				tTotalRecved += tRecved;
			}
		}
		catch (std::exception & e)
		{
			Log_Error("%s", e.what());
			return nRet;
		}

		return EC_SUCCESS;
	}
}


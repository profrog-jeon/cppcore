#pragma once

#include "../100_System/Socket.h"
#include "../100_System/SocketDefine.h"

namespace core
{
	struct ISyncSocket
	{
		SOCKET m_hSocket;

		ISyncSocket(void) : m_hSocket(INVALID_SOCKET_) {}
		virtual ~ISyncSocket(void) {}

		virtual ECODE Accept(SOCKET hAcceptedSocket) = 0;
		virtual ECODE Connect(const char* pszIP, WORD wPort, DWORD dwTimeOut) = 0;
		virtual void Close(void) = 0;

		virtual ECODE Send(const void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptSent = NULL) = 0;
		virtual ECODE Recv(void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptRead = NULL) = 0;
		virtual ECODE Peek(void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptRead = NULL) = 0;
	};
}

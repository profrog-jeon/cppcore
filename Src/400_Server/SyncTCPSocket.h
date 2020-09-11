#pragma once

#include "ISyncSocket.h"

namespace core
{
	class CSyncTCPSocket : public ISyncSocket
	{
	public:
		CSyncTCPSocket(void);
		~CSyncTCPSocket();

		ECODE Accept(SOCKET hAcceptedSocket);
		ECODE Connect(const char* pszIP, WORD wPort, DWORD dwTimeOut);
		void Close(void);

		ECODE Send(const void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptSent = NULL);
		ECODE Recv(void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptRead = NULL);
		ECODE Peek(void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptRead = NULL);

		ECODE SendForcely(const void* pSrc, size_t tLength, DWORD dwTimeOut, DWORD dwMTUSize = INFINITE);
		ECODE RecvForcely(void* pDest, size_t tLength, DWORD dwTimeOut);
	};

}

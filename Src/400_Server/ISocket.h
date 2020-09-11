#pragma once

#include "../100_System/100_System.h"

struct ISocket
{
	virtual ~ISocket(void) {}

	virtual int Bind(DWORD dwIP, WORD wPort) = 0;
	virtual int Listen(int nMaxPendingCount) = 0;
	virtual ECODE Accept(const ISocket* pListenSock, core::ST_ACCEPT_INFO * pOutInfo) = 0;
	virtual int Connect(const char* pszIP, WORD wPort) = 0;
	virtual void Close(void) = 0;
	virtual int Send(const void* pData, size_t tDataSize, int nTimeOut) = 0;
	virtual int Recv(void* pData, size_t tDataSize, int nTimeOut) = 0;
	virtual int Peek(void* pData, size_t tDataSize, int nTimeOut) = 0;
};
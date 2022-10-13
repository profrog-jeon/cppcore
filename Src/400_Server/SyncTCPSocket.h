#pragma once

namespace core
{
	class CSyncTCPSocket
	{
		friend class CSyncServer;
		friend class CSyncConnection;

		SOCKET m_hSocket;

	public:
		CSyncTCPSocket(void);
		~CSyncTCPSocket();

		ECODE Assign(SOCKET hAcceptedSocket);
		ECODE Connect(const char* pszIP, WORD wPort, DWORD dwTimeOut);
		void Close(void);

		ECODE Send(const void* pBuff, size_t tBufSize, DWORD dwTimeOut);
		ECODE Recv(void* pBuff, size_t tBufSize, DWORD dwTimeOut);
		ECODE Peek(void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptRead = NULL);
	};

}

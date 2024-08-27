#pragma once

namespace core
{
	class CSyncTCPSocket
	{
		friend class CSyncServer;
		friend class CSyncConnection;

	protected:
		SOCKET m_hSocket;

	public:
		CSyncTCPSocket(void);
		virtual ~CSyncTCPSocket();

		virtual ECODE Assign(SOCKET hAcceptedSocket);
		virtual ECODE Connect(const char* pszIP, WORD wPort, DWORD dwTimeOut);
		virtual void Close(void);

		virtual ECODE Send(const void* pBuff, size_t tBufSize, DWORD dwTimeOut);
		virtual ECODE Recv(void* pBuff, size_t tBufSize, DWORD dwTimeOut);
		virtual ECODE Recv(void* pBuff, size_t tBufSize, size_t* ptRead, DWORD dwTimeOut);
		virtual ECODE Peek(void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptRead = NULL);

	protected:
		virtual ECODE SendWorker(SOCKET hSocket, const void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptSent);
		virtual ECODE RecvWorker(SOCKET hSocket, void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptRead);
		virtual ECODE PeekWorker(SOCKET hSocket, void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptRead);
	};

}

#pragma once

namespace core
{
	class CSyncUDPSocket
	{
		SOCKET m_hSocket;

	public:
		CSyncUDPSocket(void);
		virtual ~CSyncUDPSocket();

		virtual ECODE Listen(WORD wPort);
		virtual ECODE Create(void);
		virtual void Destroy(void);

		virtual ECODE Broadcast(DWORD dwIP, WORD wPort, const void* pData, size_t tDataSize);
		virtual ECODE SendTo(const ST_SOURCE_INFO& pSourceInfo, const void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptSent = NULL);
		virtual ECODE RecvFrom(void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptRead = NULL, ST_SOURCE_INFO* pSourceInfo = NULL);
	};
}

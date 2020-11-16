#pragma once

namespace core
{
	class CSyncUDPSocket
	{
		SOCKET m_hSocket;

	public:
		CSyncUDPSocket(void);
		~CSyncUDPSocket();

		ECODE Listen(WORD wPort);
		ECODE Create(void);
		void Destroy(void);

		ECODE Broadcast(DWORD dwIP, WORD wPort, const void* pData, size_t tDataSize);
		ECODE SendTo(const ST_SOURCE_INFO& pSourceInfo, const void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptSent = NULL);
		ECODE RecvFrom(void* pBuff, size_t tBufSize, DWORD dwTimeOut, size_t* ptRead = NULL, ST_SOURCE_INFO* pSourceInfo = NULL);
	};
}

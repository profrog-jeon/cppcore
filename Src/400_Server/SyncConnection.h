#pragma once

#include "SyncTCPSocket.h"

namespace core
{
	class CSyncConnection
	{
		friend class CSyncServer;

	protected:
		CSyncTCPSocket*	m_pSocket;
		std::string		m_strClientIP;

	public:
						CSyncConnection(CSyncTCPSocket* pSocket);
		virtual			~CSyncConnection();

		virtual ECODE	SetAcceptedSocket(SOCKET hNewSocket)	{	return m_pSocket->Assign(hNewSocket);		}
		virtual CSyncTCPSocket* Raw(void)				{	return m_pSocket;					}

		virtual void	OnConnect(void) = 0;
		virtual void	OnClose(void) = 0;
		virtual void	OnRecv(void) = 0;
	};
}

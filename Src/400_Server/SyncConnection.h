#pragma once

#include "ISyncSocket.h"

namespace core
{
	class CSyncConnection
	{
		friend class CSyncServer;

	protected:
		ISyncSocket*	m_pSocket;
		std::string		m_strClientIP;

	public:
						CSyncConnection(ISyncSocket* pSocket);
		virtual			~CSyncConnection();

		virtual ECODE	Accept(SOCKET hNewSocket)	{	return m_pSocket->Accept(hNewSocket);		}
		virtual ECODE	Accept(ISyncSocket* pSocket);
		virtual ISyncSocket* Raw(void)				{	return m_pSocket;					}

		virtual void	OnConnect(void) = 0;
		virtual void	OnClose(void) = 0;
		virtual void	OnRecv(void) = 0;
	};
}

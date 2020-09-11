#include "stdafx.h"
#include "SyncConnection.h"

namespace core
{
	CSyncConnection::CSyncConnection(ISyncSocket* pSocket)
		: m_pSocket(pSocket)
		, m_strClientIP()
	{
	}

	CSyncConnection::~CSyncConnection()
	{
	}

	ECODE CSyncConnection::Accept(ISyncSocket * pSocket)
	{
		if (NULL == m_pSocket)
			return EC_NOT_CONNECTED;

		SOCKET hSocket = m_pSocket->m_hSocket;
		m_pSocket = pSocket;
		return Accept(hSocket);
	}
}


#include "stdafx.h"
#include "SyncConnection.h"

namespace core
{
	CSyncConnection::CSyncConnection(CSyncTCPSocket* pSocket)
		: m_pSocket(pSocket)
		, m_strClientIP()
	{
	}

	CSyncConnection::~CSyncConnection()
	{
	}
}


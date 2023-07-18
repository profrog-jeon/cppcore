#include "stdafx.h"
#include "SocketForProtocol.h"
#include "Packetizer.h"

namespace core
{
	CSocketForProtocol::CSocketForProtocol(core::CSyncTCPSocket* pSocket)
		: m_pSocket(pSocket)
	{
	}

	CSocketForProtocol::~CSocketForProtocol(void)
	{
	}

	void CSocketForProtocol::Close(void)
	{
		m_pSocket->Close();
	}
}
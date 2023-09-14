#include "stdafx.h"
#include "ProtocolSuper.h"

namespace core
{
	CProtocolSuper::CProtocolSuper(core::CSyncTCPSocket* pSocket)
		: m_pSocket(pSocket)
	{
	}

	CProtocolSuper::~CProtocolSuper(void)
	{
	}

	ECODE CProtocolSuper::Connect(std::string strIP, WORD wPort, DWORD dwTimeOut)
	{
		return m_pSocket->Connect(strIP.c_str(), wPort, dwTimeOut);
	}

	void CProtocolSuper::Close(void)
	{
		m_pSocket->Close();
	}

	ECODE CProtocolSuper::PeekHeader(ST_PACKET_HEADER& header, DWORD dwTimeOut)
	{
		size_t tReadSize = 0;
		ECODE nRet = m_pSocket->Peek(&header, sizeof(header), dwTimeOut, &tReadSize);
		if (EC_SUCCESS != nRet)
			return nRet;

		if (tReadSize < sizeof(header))
			return EC_NOT_READY;

		return EC_SUCCESS;
	}
}
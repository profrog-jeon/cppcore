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

	inline static ECODE SendBinaryWorker(CSyncTCPSocket* pSocket, DWORD dwID, LPCVOID pData, size_t tDataSize, DWORD dwTimeOut)
	{
		std::vector<BYTE> vecPacket;

		const size_t tHeaderSize = sizeof(ST_PACKET_HEADER);
		vecPacket.resize(tHeaderSize + tDataSize);

		ST_PACKET_HEADER* pHeader = (ST_PACKET_HEADER*)vecPacket.data();
		pHeader->dwID = dwID;
		pHeader->dwLen = (DWORD)tDataSize;
		if (tDataSize)
			memcpy(&vecPacket[tHeaderSize], pData, tDataSize);

		return pSocket->Send(vecPacket.data(), vecPacket.size(), dwTimeOut);
	}

	ECODE CProtocolSuper::SendBinary(DWORD dwID, const std::vector<BYTE>& vecData, DWORD dwTimeOut)
	{
		return SendBinaryWorker(m_pSocket, dwID, vecData.data(), vecData.size(), dwTimeOut);
	}

	ECODE CProtocolSuper::SendBinary(DWORD dwID, LPCVOID pData, size_t tDataSize, DWORD dwTimeOut)
	{
		return SendBinaryWorker(m_pSocket, dwID, pData, tDataSize, dwTimeOut);
	}

	ECODE CProtocolSuper::RecvBinary(DWORD dwID, std::vector<BYTE>& vecData, DWORD dwTimeOut)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			ST_PACKET_HEADER header;
			nRet = m_pSocket->Recv(&header, sizeof(header), dwTimeOut);
			if (EC_SUCCESS != nRet)
				throw exception_format(TEXT("header(ID:%u) recving failure, %d"), dwID, nRet);

			nRet = EC_INVALID_DATA;
			if (header.dwID != dwID)
				throw exception_format(TEXT("header(ID:%u) not matched with:%u"), header.dwID, dwID);

			if (header.dwLen)
			{
				vecData.resize(header.dwLen);
				nRet = m_pSocket->Recv(&vecData[0], vecData.size(), dwTimeOut);
				if (EC_SUCCESS != nRet)
					throw exception_format(TEXT("body(ID:%u, Len:%u) recving failure, %d"), header.dwLen, nRet);
			}
		}
		catch (const std::exception& e)
		{
			Log_Error("%s", e.what());
			return nRet;
		}
		return EC_SUCCESS;
	}
}
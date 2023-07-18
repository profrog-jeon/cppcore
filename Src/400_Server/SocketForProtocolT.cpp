#include "stdafx.h"
#include "SocketForProtocol.h"
#include "Packetizer.h"

namespace core
{
	ECODE CSocketForProtocol::Connect(std::tstring strIP, WORD wPort, DWORD dwTimeOut)
	{
		return m_pSocket->Connect(MBSFromTCS(strIP).c_str(), wPort, dwTimeOut);
	}

	ECODE CSocketForProtocol::SendPacket(DWORD dwID, core::IFormatterObject* pPacket, DWORD dwTimeOut)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			std::vector<BYTE> vecPacket;
			nRet = Packetize(dwID, pPacket, vecPacket);
			if (EC_SUCCESS != nRet)
				throw exception_format(TEXT("Packetize failure, %d"), nRet);

			nRet = m_pSocket->Send(vecPacket.data(), vecPacket.size(), dwTimeOut);
			if (EC_SUCCESS != nRet)
				throw exception_format(TEXT("Packet sending failure, %d"), nRet);
		}
		catch (const std::exception& e)
		{
			core::Log_Error("%s", e.what());
			return nRet;
		}

		return EC_SUCCESS;
	}

	ECODE CSocketForProtocol::RecvPacket(DWORD dwID, core::IFormatterObject* pPacket, DWORD dwTimeOut)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			ST_PACKET_HEADER header;
			nRet = m_pSocket->Peek(&header, sizeof(ST_PACKET_HEADER), dwTimeOut);
			if (EC_SUCCESS != nRet)
				throw core::exception_format(TEXT("Header Peeking faliure, %d"), nRet);

			std::vector<BYTE> vecBody;
			vecBody.resize(header.dwLen + sizeof(ST_PACKET_HEADER));
			nRet = m_pSocket->Recv(&vecBody[0], vecBody.size(), dwTimeOut);
			if (EC_SUCCESS != nRet)
				throw core::exception_format(TEXT("Body Recving failure, %d"), nRet);

			nRet = Unpacketize(vecBody, dwID, pPacket);
			if (EC_SUCCESS != nRet)
				throw core::exception_format(TEXT("Unpacketize failure, %d"), nRet);
		}
		catch (core::exception_format& e)
		{
			core::Log_Error("%s", e.what());
			return nRet;
		}

		return EC_SUCCESS;
	}

}
#include "stdafx.h"
#include "UBJsonProtocol.h"
#include "Packetizer.h"

namespace core
{
	ECODE CUBJsonProtocol::SendPacket(DWORD dwID, core::IFormatterObject* pPacket, DWORD dwTimeOut)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			const size_t tHeaderSize = sizeof(ST_PACKET_HEADER);

			std::vector<BYTE> vecContext;

			nRet = EC_WRITE_FAILURE;
			if (!WriteUBJsonToPacket(pPacket, vecContext))
				throw exception_format(TEXT("Packetize failure, %d"), nRet);

			std::vector<BYTE> vecBody;
			vecBody.resize(tHeaderSize + vecContext.size());

			ST_PACKET_HEADER* pHeader = (ST_PACKET_HEADER*)&vecBody[0];
			pHeader->dwID = dwID;
			pHeader->dwLen = (DWORD)vecContext.size();
			if (!vecContext.empty())
				memcpy(&vecBody[tHeaderSize], vecContext.data(), vecContext.size());

			nRet = m_pSocket->Send(vecBody.data(), vecBody.size(), dwTimeOut);
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

	ECODE CUBJsonProtocol::RecvPacket(DWORD dwID, core::IFormatterObject* pPacket, DWORD dwTimeOut)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			ST_PACKET_HEADER header;
			nRet = m_pSocket->Recv(&header, sizeof(header), dwTimeOut);
			if (EC_SUCCESS != nRet)
				throw core::exception_format(TEXT("Length reading faliure, %d"), nRet);

			if (header.dwLen)
			{
				std::vector<BYTE> vecContext;
				vecContext.resize(header.dwLen);
				nRet = m_pSocket->Recv(&vecContext[0], vecContext.size(), dwTimeOut);
				if (EC_SUCCESS != nRet)
					throw core::exception_format(TEXT("Body Recving failure, %d"), nRet);

				nRet = EC_INVALID_DATA;
				if (dwID != header.dwID)
					throw core::exception_format(TEXT("Body ID(try:%u, recved:%u) is not matched"), dwID, header.dwID);

				std::tstring strErrMsg;

				nRet = EC_INVALID_DATA;
				if (!ReadUBJsonFromPacket(pPacket, vecContext, &strErrMsg))
					throw core::exception_format(TEXT("Unpacketize failure, %s"), strErrMsg.c_str());
			}
		}
		catch (core::exception_format& e)
		{
			core::Log_Error("%s", e.what());
			return nRet;
		}

		return EC_SUCCESS;
	}
}

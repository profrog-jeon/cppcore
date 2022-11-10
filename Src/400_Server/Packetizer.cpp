#include "stdafx.h"
#include "Packetizer.h"

namespace core
{
	struct ST_PACKET_METADATA : public IFormatterObject
	{
		ST_PACKET_HEADER header;
		IFormatterObject* pPacket;

		ST_PACKET_METADATA(void)
			: pPacket(nullptr)
		{}

		void OnSync(IFormatter& formatter)
		{
			formatter
				+ sPair(TEXT("Type"), header.dwType)
				+ sPair(TEXT("Len"), header.dwLen)
				;

			if (pPacket)
				formatter + sPair(TEXT("Body"), *pPacket);
		}
	};

	ECODE Packetize(DWORD dwPacketID, IFormatterObject* pPacket, std::vector<BYTE>& outPacket)
	{
		ST_PACKET_METADATA metaPacket;
		metaPacket.pPacket = pPacket;

		if (!core::UTF8::WriteBinToPacket(&metaPacket, outPacket))
			return EC_WRITE_FAILURE;

		if(outPacket.size() < 8)
			return EC_INTERNAL_ERROR;

		DWORD* pType = (DWORD*)&outPacket[0];
		DWORD* pLen = (DWORD*)&outPacket[4];
		*pType = dwPacketID;
		*pLen = outPacket.size() - 8;
		return EC_SUCCESS;
	}

	ECODE Unpacketize(const std::vector<BYTE>& inPacket, ST_PACKET_HEADER* pHeader)
	{
		ST_PACKET_METADATA metaPacket;

		if (!core::UTF8::ReadBinFromPacket(&metaPacket, inPacket))
			return EC_READ_FAILURE;

		*pHeader = metaPacket.header;
		return EC_SUCCESS;
	}

	ECODE Unpacketize(const std::vector<BYTE>& inPacket, DWORD dwPacketID, IFormatterObject* pOutPacket)
	{
		ST_PACKET_METADATA metaPacket;
		metaPacket.pPacket = pOutPacket;

		if (!core::UTF8::ReadBinFromPacket(&metaPacket, inPacket))
			return EC_READ_FAILURE;

		if (dwPacketID != metaPacket.header.dwType)
			return EC_INVALID_DATA;

		return EC_SUCCESS;
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

			nRet = Send(vecPacket.data(), vecPacket.size(), dwTimeOut);
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
			nRet = Peek(&header, sizeof(ST_PACKET_HEADER), dwTimeOut);
			if (EC_SUCCESS != nRet)
				throw core::exception_format(TEXT("Header Peeking faliure, %d"), nRet);

			std::vector<BYTE> vecBody;
			vecBody.resize(header.dwLen + sizeof(ST_PACKET_HEADER));
			nRet = this->Recv(&vecBody[0], vecBody.size(), dwTimeOut);
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
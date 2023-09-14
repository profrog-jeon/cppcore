#include "stdafx.h"
#include "Packetizer.h"

#ifdef UNICODE
#define ST_BIN_PACKET_METADATA		ST_BIN_PACKET_METADATAW
#define ST_STRING_PACKET_METADATA	ST_STRING_PACKET_METADATAW
#else
#define ST_BIN_PACKET_METADATA		ST_BIN_PACKET_METADATAA
#define ST_STRING_PACKET_METADATA	ST_STRING_PACKET_METADATAA
#endif

namespace core
{
	///////////////////////////////////////////////////////////////////////////////////
	// BIN PACKETIZER
	///////////////////////////////////////////////////////////////////////////////////
	struct ST_BIN_PACKET_METADATA : public IFormatterObject
	{
		ST_PACKET_HEADER header;
		IFormatterObject* pPacket;

		ST_BIN_PACKET_METADATA(void)
			: pPacket(nullptr)
		{}

		void OnSync(IFormatter& formatter)
		{
			formatter
				+ sPair(TEXT("ID"), header.dwID)
				+ sPair(TEXT("Len"), header.dwLen)
				;

			if (pPacket)
				formatter + sPair(TEXT("Body"), *pPacket);
		}
	};

	ECODE Packetize(DWORD dwPacketID, IFormatterObject* pPacket, std::vector<BYTE>& outPacket)
	{
		ST_BIN_PACKET_METADATA metaPacket;
		metaPacket.pPacket = pPacket;

		if (!core::WriteBinToPacket(&metaPacket, outPacket))
			return EC_WRITE_FAILURE;

		if(outPacket.size() < 8)
			return EC_INTERNAL_ERROR;

		DWORD* pType = (DWORD*)&outPacket[0];
		DWORD* pLen = (DWORD*)&outPacket[4];
		*pType = dwPacketID;
		*pLen = (DWORD)outPacket.size() - 8;
		return EC_SUCCESS;
	}

#ifdef UNICODE
	ECODE UnpacketizeW(const std::vector<BYTE>& inPacket, ST_PACKET_HEADER* pHeader)
#else
	ECODE UnpacketizeA(const std::vector<BYTE>& inPacket, ST_PACKET_HEADER* pHeader)
#endif
	{
		ST_BIN_PACKET_METADATA metaPacket;

		if (!core::ReadBinFromPacket(&metaPacket, inPacket))
			return EC_READ_FAILURE;

		*pHeader = metaPacket.header;
		return EC_SUCCESS;
	}

	ECODE Unpacketize(const std::vector<BYTE>& inPacket, DWORD dwPacketID, IFormatterObject* pOutPacket)
	{
		ST_BIN_PACKET_METADATA metaPacket;
		metaPacket.pPacket = pOutPacket;

		if (!core::ReadBinFromPacket(&metaPacket, inPacket))
			return EC_READ_FAILURE;

		if (dwPacketID != metaPacket.header.dwID)
			return EC_INVALID_DATA;

		return EC_SUCCESS;
	}
}
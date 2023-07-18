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

#ifdef UNICODE
	ECODE UnpacketizeW(const std::vector<BYTE>& inPacket, ST_PACKET_HEADER* pHeader)
#else
	ECODE UnpacketizeA(const std::vector<BYTE>& inPacket, ST_PACKET_HEADER* pHeader)
#endif
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
}
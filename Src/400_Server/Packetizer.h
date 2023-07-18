#pragma once

#include "../300_Formatter/300_Formatter.h"
#include "SyncTCPSocket.h"

namespace core
{
	struct ST_PACKET_HEADER
	{
		DWORD dwType;
		DWORD dwLen;
	};

	ECODE Packetize(DWORD dwPacketID, IFormatterObject* pPacket, std::vector<BYTE>& outPacket);
	ECODE Unpacketize(const std::vector<BYTE>& inPacket, ST_PACKET_HEADER* pHeader);
	ECODE Unpacketize(const std::vector<BYTE>& inPacket, DWORD dwPacketID, IFormatterObject* pOutPacket);
}

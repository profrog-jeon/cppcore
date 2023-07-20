#pragma once

#include "../300_Formatter/300_Formatter.h"
#include "SyncTCPSocket.h"

namespace core
{
	struct ST_PACKET_HEADER
	{
		DWORD dwID;
		DWORD dwLen;
	};

	// BIN type
	ECODE Packetize(DWORD dwPacketID, IFormatterObjectA* pPacket, std::vector<BYTE>& outPacket);
	ECODE Packetize(DWORD dwPacketID, IFormatterObjectW* pPacket, std::vector<BYTE>& outPacket);
	ECODE UnpacketizeA(const std::vector<BYTE>& inPacket, ST_PACKET_HEADER* pHeader);
	ECODE UnpacketizeW(const std::vector<BYTE>& inPacket, ST_PACKET_HEADER* pHeader);
	ECODE Unpacketize(const std::vector<BYTE>& inPacket, DWORD dwPacketID, IFormatterObjectA* pOutPacket);
	ECODE Unpacketize(const std::vector<BYTE>& inPacket, DWORD dwPacketID, IFormatterObjectW* pOutPacket);
}

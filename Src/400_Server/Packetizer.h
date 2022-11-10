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

	/////////////////////////////////////////////////////////////////////
	// <<RESTRICTION>>
	/////////////////////////////////////////////////////////////////////
	// PACKET should have static ID.
	// Refer to below:
	//  ex) struct ST_SOME_PACKET : public core::IFormatterObject
	//		{
	//			static const int ID = 102;
	//			int nValue;
	//			std::tstring strValue;
	//			void OnSync(core::IFormatter& formatter)
	//			{
	//				formatter
	//					+ core::sPair(TEXT("Int"), nValue)
	//					+ core::sPair(TEXT("String"), strValue)
	//					;
	//			}
	//		};
	/////////////////////////////////////////////////////////////////////
	class CSocketForProtocol : public core::CSyncTCPSocket
	{
	public:
		ECODE SendPacket(DWORD dwID, core::IFormatterObject* pPacket, DWORD dwTimeOut = 60000);
		ECODE RecvPacket(DWORD dwID, core::IFormatterObject* pPacket, DWORD dwTimeOut = 60000);

		template<typename T>
		ECODE SendPacket(T* pPacket, DWORD dwTimeOut = 60000)
		{
			return SendPacket(T::ID, pPacket, dwTimeOut);
		}

		template<typename T>
		ECODE RecvPacket(T* pPacket, DWORD dwTimeOut = 60000)
		{
			return RecvPacket(T::ID, pPacket, dwTimeOut);
		}
	};
}

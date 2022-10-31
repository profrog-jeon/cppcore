#pragma once

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
		template<typename T>
		ECODE SendPacket(T* pPacket, DWORD dwTimeOut = 60000)
		{
			ECODE nRet = EC_SUCCESS;
			try
			{
				std::vector<BYTE> vecPacket;
				nRet = Packetize(T::ID, pPacket, vecPacket);
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

		template<typename T>
		ECODE RecvPacket(T* pPacket, DWORD dwTimeOut = 60000)
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

				nRet = Unpacketize(vecBody, T::ID, pPacket);
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
	};
}

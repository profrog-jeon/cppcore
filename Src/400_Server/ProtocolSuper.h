#pragma once

#include "SyncTCPSocket.h"
#include "Packetizer.h"

namespace core
{
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

	class CProtocolSuper
	{
	protected:
		core::CSyncTCPSocket* m_pSocket;

	public:
		CProtocolSuper(core::CSyncTCPSocket* pSocket);
		virtual ~CProtocolSuper(void);

		virtual ECODE Connect(std::string strIP, WORD wPort, DWORD dwTimeOut);
		virtual void Close(void);

		virtual ECODE PeekHeader(ST_PACKET_HEADER& header, DWORD dwTimeOut = 60000);

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

	protected:
		virtual ECODE SendPacket(DWORD dwID, core::IFormatterObjectA* pPacket, DWORD dwTimeOut) = 0;
		virtual ECODE SendPacket(DWORD dwID, core::IFormatterObjectW* pPacket, DWORD dwTimeOut) = 0;
		virtual ECODE RecvPacket(DWORD dwID, core::IFormatterObjectA* pPacket, DWORD dwTimeOut) = 0;
		virtual ECODE RecvPacket(DWORD dwID, core::IFormatterObjectW* pPacket, DWORD dwTimeOut) = 0;
	};
}

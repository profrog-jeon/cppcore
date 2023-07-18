#pragma once

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
	class CSocketForProtocol
	{
		core::CSyncTCPSocket* m_pSocket;

	public:
		CSocketForProtocol(core::CSyncTCPSocket* pSocket);
		virtual ~CSocketForProtocol(void);

		virtual ECODE Connect(std::string strIP, WORD wPort, DWORD dwTimeOut);
		virtual ECODE Connect(std::wstring strIP, WORD wPort, DWORD dwTimeOut);
		virtual void Close(void);

		virtual ECODE SendPacket(DWORD dwID, core::IFormatterObjectA* pPacket, DWORD dwTimeOut = 60000);
		virtual ECODE SendPacket(DWORD dwID, core::IFormatterObjectW* pPacket, DWORD dwTimeOut = 60000);
		virtual ECODE RecvPacket(DWORD dwID, core::IFormatterObjectA* pPacket, DWORD dwTimeOut = 60000);
		virtual ECODE RecvPacket(DWORD dwID, core::IFormatterObjectW* pPacket, DWORD dwTimeOut = 60000);

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

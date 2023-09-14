#pragma once

#include "ProtocolSuper.h"
#include "Packetizer.h"

namespace core
{
	class CUBJsonProtocol : public CProtocolSuper
	{
	public:
		CUBJsonProtocol(core::CSyncTCPSocket* pSocket)
			: CProtocolSuper(pSocket)
		{}

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

		ECODE SendPacket(DWORD dwID, core::IFormatterObjectA* pPacket, DWORD dwTimeOut = 60000);
		ECODE SendPacket(DWORD dwID, core::IFormatterObjectW* pPacket, DWORD dwTimeOut = 60000);
		ECODE RecvPacket(DWORD dwID, core::IFormatterObjectA* pPacket, DWORD dwTimeOut = 60000);
		ECODE RecvPacket(DWORD dwID, core::IFormatterObjectW* pPacket, DWORD dwTimeOut = 60000);
	};
}


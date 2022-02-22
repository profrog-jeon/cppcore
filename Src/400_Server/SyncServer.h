#pragma once

#include <set>
#include "../200_Common/200_Common.h"
#include "SyncConnection.h"

namespace core
{
	enum E_DISCONNECT_TYPE
	{
		DISCONNECT_TYPE_UNDEFINED					= 0,
		DISCONNECT_TYPE_NO_RESPONSE_ON_CONNECT		,		// pending count=0,
		DISCONNECT_TYPE_CONNECTED_AND_NO_RESPONSE	,		// pending count=100, no accept
		DISCONNECT_TYPE_CONNECTED_AND_CLOSE			,		// pending count=100, accept and close
	};

	struct ST_SYNCSERVER_INIT
	{
		WORD		wPort							;
		WORD		wReserved						;
		DWORD		dwRecvTimeOut					;
		E_DISCONNECT_TYPE nDisconnectType			;
		std::vector<CSyncConnection*>	Connections	;

		ST_SYNCSERVER_INIT(void)
			: wPort(0), wReserved(0), dwRecvTimeOut(10000)
			, nDisconnectType(DISCONNECT_TYPE_NO_RESPONSE_ON_CONNECT)
			, Connections()
		{}
	};

	class CSyncServer
	{
	protected:
		ST_SYNCSERVER_INIT m_stInit;

		TSyncQueue<CSyncConnection*>	m_queReady;
		TSyncQueue<CSyncConnection*>	m_queDisconnected;

		CCriticalSection				m_csConnected;
		std::set<CSyncConnection*>		m_setConnected;
		CThreadPool						m_ThreadPool;

		SOCKET		m_hListenSocket;
		HANDLE		m_hListenThread;
		HANDLE		m_hDisconnectThread;

		bool		m_bContinueLoop;

	public:
		CSyncServer();
		virtual ~CSyncServer();

		virtual ECODE StartUp(const ST_SYNCSERVER_INIT& stInit);
		virtual void ShutDown(void);

		typedef bool (*FP_BROADCAST_CALLBACK)(CSyncConnection* pConnection, void* pContext);	// check send packet or not
		void		Broadcast(std::string strContext, FP_BROADCAST_CALLBACK fpCallback = NULL, void* pContext = NULL);
		void		Broadcast(LPCVOID pData, size_t tSize, FP_BROADCAST_CALLBACK fpCallback = NULL, void* pContext = NULL);

		size_t		MaxConnectionCount(void) { return m_stInit.Connections.size(); }
		size_t		ConnectionCount(void) { return m_setConnected.size(); }

	private:
		int			ListenThread(void * pContext);
		int			DisconnectThread(void * pContext);

		friend int	ConnectionThreadCaller(void* pContext);
		void		ConnectionThread(CSyncConnection* pConnection);
	};
}

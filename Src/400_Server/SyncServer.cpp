#include "stdafx.h"
#include "SyncServer.h"
#include "SocketCommon.h"

namespace core
{
	CSyncServer::CSyncServer()
		: m_stInit()
		, m_queReady()
		, m_setConnected()
		, m_hListenSocket(INVALID_SOCKET_)
		, m_hListenThread(NULL)
		, m_hDisconnectThread(NULL)
		, m_bContinueLoop(false)
	{
	}

	CSyncServer::~CSyncServer()
	{
	}

	ECODE CSyncServer::StartUp(const ST_SYNCSERVER_INIT & stInit)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			m_queReady.Create();
			m_queDisconnected.Create();

			nRet = m_ThreadPool.Init(stInit.Connections.size());
			if (EC_SUCCESS != nRet)
				throw exception_format("ThreadPool initialize failed, %d", nRet);

			m_hListenSocket = core::socket(AF_INET_, SOCK_STREAM_, IPPROTO_TCP_);

			nRet = EC_INTERNAL_ERROR;
			if (INVALID_SOCKET_ == m_hListenSocket)
				throw exception_format("socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) failure, %s", GetErrorStringA(GetLastError()).c_str());

			nRet = SetCommonSockOpt(m_hListenSocket);
			if( EC_SUCCESS != nRet)
				throw exception_format("SetCommonSockOpt failure, %d", nRet);

			nRet = core::bind(m_hListenSocket, INADDR_ANY_, stInit.wPort);
			if (SOCKET_ERROR_ == nRet)
				throw exception_format("bind(m_hListenSocket, (SOCKADDR*)&service, sizeof(service) failure, %s", GetErrorStringA(GetLastError()).c_str());

			size_t i;
			for (i = 0; i < stInit.Connections.size(); i++)
			{
				m_queReady.Push(stInit.Connections[i]);
			}

			m_bContinueLoop = true;
			m_hListenThread = CreateThread(this, &CSyncServer::ListenThread, NULL);
			m_hDisconnectThread = CreateThread(this, &CSyncServer::DisconnectThread, NULL);

			m_stInit = stInit;
		}
		catch (std::exception& e)
		{
			if (SOCKET_ERROR_ == nRet)
				nRet = GetLastError();

			Log_Error("%s", e.what());
			if (INVALID_SOCKET_ != m_hListenSocket)
			{
				core::closesocket(m_hListenSocket);
				m_hListenSocket = INVALID_SOCKET_;
			}
			m_queReady.Destroy();
			m_queDisconnected.Destroy();
			return nRet;
		}

		return EC_SUCCESS;
	}

	void CSyncServer::ShutDown(void)
	{
		if (INVALID_SOCKET_ == m_hListenSocket)
			return;

		m_bContinueLoop = false;
		core::closesocket(m_hListenSocket);
		m_hListenSocket = INVALID_SOCKET_;

		JoinThread(m_hListenThread);
		m_hListenThread = NULL;

		m_queDisconnected.Destroy();
		JoinThread(m_hDisconnectThread);

		for (auto iter = m_setConnected.begin(); iter != m_setConnected.end(); iter++)
			core::closesocket((*iter)->Raw()->m_hSocket);
		m_ThreadPool.Destroy();

		size_t i;
		for (i = 0; i < m_stInit.Connections.size(); i++)
			delete m_stInit.Connections[i];

		m_stInit.Connections.clear();
		m_setConnected.clear();
		m_queReady.Clear();
	}

	void CSyncServer::Broadcast(std::string strContext, FP_BROADCAST_CALLBACK fpCallback, void * pContext)
	{
		CCriticalSection::Owner Lock(m_csConnected);

		for (auto iter = m_setConnected.begin(); iter != m_setConnected.end(); iter++)
		{
			if (NULL == fpCallback)
			{
				(*iter)->Raw()->Send(strContext.c_str(), strContext.length() + 1, 5000);
				continue;
			}

			if (fpCallback((*iter), pContext))
			{
				(*iter)->Raw()->Send(strContext.c_str(), strContext.length() + 1, 5000);
				continue;
			}
		}
	}

	void CSyncServer::Broadcast(LPCVOID pData, size_t tSize, FP_BROADCAST_CALLBACK fpCallback, void* pContext)
	{
		CCriticalSection::Owner Lock(m_csConnected);

		for (auto iter = m_setConnected.begin(); iter != m_setConnected.end(); iter++)
		{
			if (NULL == fpCallback)
			{
				(*iter)->Raw()->Send(pData, tSize, 5000);
				continue;
			}

			if (fpCallback((*iter), pContext))
			{
				(*iter)->Raw()->Send(pData, tSize, 5000);
				continue;
			}
		}
	}

	struct ST_CONNECTION_THREAD_DATA
	{
		CSyncServer* pServer;
		CSyncConnection* pConnection;
	};

	int ConnectionThreadCaller(void* pContext)
	{
		ST_CONNECTION_THREAD_DATA* pData = (ST_CONNECTION_THREAD_DATA*)pContext;
		pData->pServer->ConnectionThread(pData->pConnection);
		delete pData;
		return 0;
	}

	int CSyncServer::ListenThread(void* pContext)
	{
		Log_Info("ListenThread started.");
		ECODE nRet = 0;

		try
		{
			int nPendingCount = 100;
			if( DISCONNECT_TYPE_NO_RESPONSE_ON_CONNECT == m_stInit.nDisconnectType )
				nPendingCount = 0;

			nRet = core::listen(m_hListenSocket, nPendingCount);
			if (EC_SUCCESS != nRet)
				throw exception_format("listen failed, %d", nRet);

			while (m_bContinueLoop)
			{
				if (DISCONNECT_TYPE_CONNECTED_AND_NO_RESPONSE == m_stInit.nDisconnectType &&
					m_queReady.IsEmpty())
				{
					Sleep(100);
					continue;
				}

				ST_ACCEPT_INFO stAcceptInfo;
				SOCKET hNewConnection = core::accept(m_hListenSocket, &stAcceptInfo);
				if (INVALID_SOCKET_ == hNewConnection)
				{
					if (!m_bContinueLoop)
						break;

					throw exception_format("accept(m_hListenSocket, &stAddrInfo, &nAddrInfoLen) failure, %d", GetLastError());
				}

				if (m_queReady.IsEmpty())
				{
					core::closesocket(hNewConnection);
					Log_Warn("Refused by maximum connection(%u) exceed.", m_stInit.Connections.size());
					continue;
				}

				CSyncConnection* pNewConnection;
				if (WAIT_OBJECT_0_ != m_queReady.Pop(&pNewConnection))
				{
					Log_Error("Retrieving ready connection has failed.");
					continue;
				}

				pNewConnection->m_strClientIP = IPAddressFromA(stAcceptInfo.dwIP);
				nRet = pNewConnection->SetAcceptedSocket(hNewConnection);
				if (EC_SUCCESS != nRet)
				{
					Log_Error("Accept failure, %d", nRet);
					m_queReady.Push(pNewConnection);
					continue;
				}

				Log_Info(TEXT("Connection allocated, remaining:%u"), m_queReady.Count());

				ST_CONNECTION_THREAD_DATA* pThreadData = new ST_CONNECTION_THREAD_DATA();
				pThreadData->pServer = this;
				pThreadData->pConnection = pNewConnection;

				while (m_bContinueLoop)
				{
					nRet = m_ThreadPool.CreateThread(ConnectionThreadCaller, pThreadData);
					if (EC_SUCCESS == nRet)
						break;

					Log_Info(TEXT("Waiting ThreadPool for new connection."));
					Sleep(100);
				}

				if (EC_SUCCESS != nRet)
				{
					core::closesocket(hNewConnection);
					throw exception_format("Final connection skipped.");
				}

				{
					CCriticalSection::Owner Lock(m_csConnected);
					m_setConnected.insert(pNewConnection);
				}
			}
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
		}
		Log_Info("ListenThread escaped.");
		return 0;
	}

	int CSyncServer::DisconnectThread(void* pContext)
	{
		CSyncConnection* pConnection = NULL;
		while (WAIT_OBJECT_0_ == m_queDisconnected.Pop(&pConnection) && m_bContinueLoop)
		{
			{
				CCriticalSection::Owner Lock(m_csConnected);
				auto iter = m_setConnected.find(pConnection);
				if(iter != m_setConnected.end() )
					m_setConnected.erase(iter);
			}

			m_queReady.Push(pConnection);
			Log_Info(TEXT("Connection returned, remaining:%u"), m_queReady.Count());
		}
		return 0;
	}

	void CSyncServer::ConnectionThread(CSyncConnection* pConnection)
	{
		pConnection->OnConnect();

		DWORD dwLastRecvTick = GetTickCount();
		char szTest = 0;
		int nRet = 0;
		while (m_bContinueLoop)
		{
			nRet = pConnection->Raw()->Peek(&szTest, 1, 1000);

			DWORD dwElapsedTick = GetTickCount() - dwLastRecvTick;
			if (m_stInit.dwRecvTimeOut < dwElapsedTick)
			{
				Log_Warn(TEXT("[%p] Connection closed by recv timeout:%u(ms) elapsed:%u(ms)")
					, pConnection, m_stInit.dwRecvTimeOut, dwElapsedTick);
				break;
			}

			if (EC_TIMEOUT == nRet)
				continue;

			if (EC_SUCCESS != nRet)
				break;

			dwLastRecvTick = GetTickCount();
			pConnection->OnRecv();
		}
		pConnection->OnClose();

		m_queDisconnected.Push(pConnection);
	}
}

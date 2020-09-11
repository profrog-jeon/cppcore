#include "stdafx.h"
#include "PipePeer.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CPipePeer::CPipePeer(void)
		: m_hSendPipe(NULL)
		, m_hRecvPipe(NULL)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CPipePeer::~CPipePeer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPipePeer::Create(std::tstring strName)
	{
		try
		{
			{
				std::tstring strSendName = strName + TEXT("-for-send");
				m_hSendPipe = CreatePipe(strSendName.c_str());
				if( NULL == m_hSendPipe )
					throw exception_format(TEXT("CreatePipe(%s) has failed"), strSendName.c_str());
			}

			{
				std::tstring strRecvName = strName + TEXT("-for-recv");
				m_hRecvPipe = CreatePipe(strRecvName.c_str());
				if( NULL == m_hRecvPipe )
					throw exception_format(TEXT("CreatePipe(%s) has failed"), strRecvName.c_str());
			}
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			if( m_hRecvPipe )
			{
				DestroyPipe(m_hRecvPipe);
				m_hRecvPipe = NULL;
			}
			if( m_hSendPipe )
			{
				DestroyPipe(m_hSendPipe);
				m_hSendPipe = NULL;
			}
			return EC_INTERNAL_ERROR;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPipePeer::Open(std::tstring strName)
	{
		try
		{
			{
				std::tstring strSendName = strName + TEXT("-for-recv");
				m_hSendPipe = OpenPipe(strSendName.c_str());
				if( NULL == m_hSendPipe )
					throw exception_format(TEXT("CreatePipe(%s) has failed"), strSendName.c_str());
			}

			{
				std::tstring strRecvName = strName + TEXT("-for-send");
				m_hRecvPipe = OpenPipe(strRecvName.c_str());
				if( NULL == m_hRecvPipe )
					throw exception_format(TEXT("CreatePipe(%s) has failed"), strRecvName.c_str());
			}
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			if( m_hRecvPipe )
			{
				DestroyPipe(m_hRecvPipe);
				m_hRecvPipe = NULL;
			}
			if( m_hSendPipe )
			{
				DestroyPipe(m_hSendPipe);
				m_hSendPipe = NULL;
			}
			ECODE nErr = GetLastError();
			if( EC_SUCCESS != nErr )
				return nErr;
			return EC_INTERNAL_ERROR;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	void CPipePeer::Close(void)
	{
		SendWorker(NULL, 0);
		Sleep(1000);
		if( m_hRecvPipe )
		{
			DestroyPipe(m_hRecvPipe);
			m_hRecvPipe = NULL;
		}
		if( m_hSendPipe )
		{
			DestroyPipe(m_hSendPipe);
			m_hSendPipe = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	struct ST_PIPEDATA_HEADER
	{
		static const DWORD cMAGIC = 0x01020304;
		DWORD dwMAGIC;
		DWORD dwSize;
		ST_PIPEDATA_HEADER(void): dwMAGIC(cMAGIC), dwSize(0)	{}
	};

	//////////////////////////////////////////////////////////////////////////
	ECODE CPipePeer::Send(std::tstring strContext)
	{
		std::string strU8Context = UTF8FromTCS(strContext);
		return SendWorker(strU8Context.c_str(), strU8Context.length());
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPipePeer::SendWorker(const void* pContext, size_t tLength)
	{
		ECODE nRet = EC_SUCCESS;

		try
		{
			ST_PIPEDATA_HEADER stHeader;
			stHeader.dwSize = tLength;

			nRet = SendThroughPipe(m_hSendPipe, &stHeader, sizeof(stHeader));
			if( EC_SUCCESS != nRet )
				throw exception_format("SendThroughPipe(header-size:%lu) failure, %d", sizeof(stHeader), nRet);

			if( tLength )
			{
				nRet = SendThroughPipe(m_hSendPipe, pContext, stHeader.dwSize);
				if( EC_SUCCESS != nRet )
					throw exception_format("SendThroughPipe(body-size:%u) failure, %d", stHeader.dwSize, nRet);
			}
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			return nRet;
		}
		
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPipePeer::Recv(std::tstring& strRecved)
	{
		ECODE nRet = EC_SUCCESS;

		try
		{
			ST_PIPEDATA_HEADER stHeader;
			nRet = RecvFromPipe(m_hRecvPipe, &stHeader, sizeof(stHeader));
			if( EC_SUCCESS != nRet )
				throw exception_format("RecvFromPipe(header-size:%lu) failure, %d", sizeof(stHeader), nRet);

			nRet = EC_INVALID_DATA;
			if( ST_PIPEDATA_HEADER::cMAGIC != stHeader.dwMAGIC )
				throw exception_format("Recved pipe data has invalid MAGIC value, %08X", stHeader.dwMAGIC);

			// 10(MB) size limitation
			nRet = EC_INVALID_DATA;
			if( 10*1000*1000 < stHeader.dwSize )
				throw exception_format("Recved pipe data has invalid size value, %u", stHeader.dwSize);

			std::string strU8Context;
			strU8Context.resize(stHeader.dwSize);
			nRet = RecvFromPipe(m_hRecvPipe, (void*)strU8Context.c_str(), stHeader.dwSize);
			if( EC_SUCCESS != nRet )
				throw exception_format("RecvFromPipe(body-size:%u) failure, %d", stHeader.dwSize, nRet);

			strRecved = TCSFromUTF8(strU8Context);
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			return nRet;
		}

		return EC_SUCCESS;
	}
}

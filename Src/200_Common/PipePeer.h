#pragma once

namespace core
{
	class CPipePeerA
	{
		HANDLE m_hSendPipe;
		HANDLE m_hRecvPipe;

	public:
		CPipePeerA(void);
		~CPipePeerA(void);

		ECODE Create(std::string strName);
		ECODE Open(std::string strName);
		void Close(void);

		ECODE Send(std::string strContext);
		ECODE Recv(std::string& strRecved);

	private:
		ECODE SendWorker(const void* pContext, size_t tLength);
	};

	class CPipePeerW
	{
		HANDLE m_hSendPipe;
		HANDLE m_hRecvPipe;

	public:
		CPipePeerW(void);
		~CPipePeerW(void);

		ECODE Create(std::wstring strName);
		ECODE Open(std::wstring strName);
		void Close(void);

		ECODE Send(std::wstring strContext);
		ECODE Recv(std::wstring& strRecved);

	private:
		ECODE SendWorker(const void* pContext, size_t tLength);
	};

#ifdef UNICODE	
#define CPipePeer CPipePeerW
#else
#define CPipePeer CPipePeerA
#endif
}

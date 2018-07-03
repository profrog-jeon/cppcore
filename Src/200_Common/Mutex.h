#pragma once

#include <string>

#include "../__Common/Type.h"
#include "../__Common/Define.h"
#include "Uncopyable.h"

namespace core
{
	class CMutex : private Uncopyable
	{
	public:
		//------------------------------------
		class Owner
		{
		private:
			CMutex&	m_Instance;

		public:
			Owner(CMutex& obj);
			~Owner(void);
		};
		//------------------------------------

	private:
		HANDLE			m_hMutex;

	public:
		CMutex(const char* pszName);
		CMutex(const wchar_t* pszName);
		~CMutex(void);

		void Enter(void);
		void Leave(void);
	};
}

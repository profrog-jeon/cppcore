#pragma once

#include "Uncopyable.h"

namespace core
{
	class CCriticalSection : private Uncopyable
	{
	public:
		//------------------------------------
		class Owner
		{
		private:
			CCriticalSection&	m_Instance;

		public:
			Owner(CCriticalSection& obj);
			~Owner(void);
		};
		//------------------------------------

	private:
		HANDLE			m_hCriticalSection;

	public:
		CCriticalSection(void);
		~CCriticalSection(void);
		void Enter(void);
		void Leave(void);
	};
}

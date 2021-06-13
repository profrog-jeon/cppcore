#pragma once

#include <assert.h>
#include <string>
#include <map>
#include <set>
#include <list>
#include <vector>

#include "../000_String/000_String.h"
#include "FmtTypes.h"
#include "PairSuper.h"

#ifdef UNICODE	
	#define IFormatter				IFormatterW
	#define IFormatterT				IFormatterW
	#define IFormatterObject		IFormatterObjectW
	#define IFormatterObjectT		IFormatterObjectW
#else
	#define IFormatter				IFormatterA
	#define IFormatterT				IFormatterA
	#define IFormatterObject		IFormatterObjectA
	#define IFormatterObjectT		IFormatterObjectA
#endif

namespace core
{
	struct IChannel;
	struct IFormatterA;
	struct IFormatterW;
	struct IFormatterObjectA
	{
		virtual ~IFormatterObjectA()	{}
		void Synchronize(IFormatterA& formatter);
		virtual void OnSync(IFormatterA& formatter) = 0;
	};
	struct IFormatterObjectW
	{
		virtual ~IFormatterObjectW()	{}
		void Synchronize(IFormatterW& formatter);
		virtual void OnSync(IFormatterW& formatter) = 0;
	};

	struct IFormatterA
	{
	protected:
		core::IChannel& m_Channel;

	public:
				IFormatterA(core::IChannel& channel);
		virtual ~IFormatterA();

		IFormatterA& operator+(core::sPairSuperA const& pair)
		{
			pair.pValueHolder->VarDispatch(pair.strKey, *this);
			return *this;
		}

		// Grouping Notification
		virtual size_t _BeginDictionaryGrouping(std::string& strKey, const size_t tSize, bool bAllowMultiKey) = 0;
		virtual void _EndDictionaryGrouping() = 0;

		virtual size_t _BeginArrayGrouping(std::string& strKey, const size_t tSize) = 0;
		virtual void _BeginArrayItem(size_t tIndex, size_t tCount)	{}
		virtual void _EndArrayItem(size_t tIndex, size_t tCount)	{}
		virtual void _EndArrayGrouping() = 0;

		virtual void _BeginObjectGrouping(std::string& strKey) = 0;
		virtual void _EndObjectGrouping() = 0;

		virtual void _BeginRootGrouping() = 0;
		virtual void _EndRootGrouping() = 0;

		virtual core::IFormatterA& Sync(std::string& strKey, std::string* pValue) = 0;
		virtual core::IFormatterA& Sync(std::string& strKey, std::wstring* pValue) = 0;
		virtual core::IFormatterA& Sync(std::string& strKey, bool* pValue) = 0;
		virtual core::IFormatterA& Sync(std::string& strKey, char* pValue) = 0;
		virtual core::IFormatterA& Sync(std::string& strKey, short* pValue) = 0;
		virtual core::IFormatterA& Sync(std::string& strKey, int32_t* pValue) = 0;
		virtual core::IFormatterA& Sync(std::string& strKey, int64_t* pValue) = 0;
		virtual core::IFormatterA& Sync(std::string& strKey, BYTE* pValue) = 0;
		virtual core::IFormatterA& Sync(std::string& strKey, WORD* pValue) = 0;
		virtual core::IFormatterA& Sync(std::string& strKey, DWORD* pValue) = 0;
		virtual core::IFormatterA& Sync(std::string& strKey, QWORD* pValue) = 0;
		virtual core::IFormatterA& Sync(std::string& strKey, float* pValue) = 0;
		virtual core::IFormatterA& Sync(std::string& strKey, double* pValue) = 0;

		template<typename K, typename V>
		core::IFormatterA& Sync(std::string& strKey, std::map<K, V>* pMapObject)
		{
			const size_t tSize = _BeginDictionaryGrouping(strKey, pMapObject->size(), false);

			if( tSize != pMapObject->size() )
			{
				pMapObject->clear();

				size_t i;
				for(i=0; i<tSize; i++)
				{
					std::string strTempKey = strKey;
					V tempValue;
					Sync(strTempKey, &tempValue);

					K tempKey = ValueFrom<K>(strTempKey);
					pMapObject->insert(std::make_pair(tempKey, tempValue));
				}
			}
			else
			{
				typename std::map<K, V>::iterator iter;
				for(iter = pMapObject->begin(); iter!=pMapObject->end(); iter++)
				{
					std::string strTempKey = StringFromA(iter->first);
					Sync(strTempKey, &iter->second);
				}
			}
			_EndDictionaryGrouping();
			return *this;
		}

		template<typename K, typename V>
		core::IFormatterA& Sync(std::string& strKey, std::multimap<K, V>* pMapObject)
		{
			const size_t tSize = _BeginDictionaryGrouping(strKey, pMapObject->size(), true);

			if( tSize != pMapObject->size() )
			{
				pMapObject->clear();

				size_t i;
				for(i=0; i<tSize; i++)
				{
					std::string strTempKey = strKey;
					V tempValue;
					Sync(strTempKey, &tempValue);

					K tempKey = ValueFrom<K>(strTempKey);
					pMapObject->insert(std::make_pair(tempKey, tempValue));
				}
			}
			else
			{
				typename std::multimap<K, V>::iterator iter;
				for(iter=pMapObject->begin(); iter!=pMapObject->end(); iter++)
				{
					std::string strTempKey = StringFromA(iter->first);
					Sync(strTempKey, &iter->second);
				}
			}
			_EndDictionaryGrouping();
			return *this;
		}

		template<typename T>
		core::IFormatterA& Sync(std::string& strKey, std::list<T>* pListObject)
		{
			const size_t tSize = _BeginArrayGrouping(strKey, pListObject->size());
			pListObject->resize(tSize);

			typename std::list<T>::iterator iter = pListObject->begin();
			size_t i;
			for(i=0; i<tSize; i++)
			{
				_BeginArrayItem(i, tSize);
				Sync(strKey, &(*iter));
				_EndArrayItem(i, tSize);
				iter++;
			}
			_EndArrayGrouping();
			return *this;
		}

		template<typename T>
		core::IFormatterA& Sync(std::string& strKey, std::vector<T>* pVecObject)
		{
			const size_t tSize = _BeginArrayGrouping(strKey, pVecObject->size());
			pVecObject->resize(tSize);

			size_t i;
			for(i=0; i<tSize; i++)
			{
				_BeginArrayItem(i, tSize);
				Sync(strKey, &(*pVecObject)[i]);
				_EndArrayItem(i, tSize);
			}

			_EndArrayGrouping();
			return *this;
		}

		template<typename T>
		core::IFormatterA& Sync(std::string& strKey, std::vector<T*>* pVecObject)
		{
			const size_t tSize = _BeginArrayGrouping(strKey, pVecObject->size());
			pVecObject->resize(tSize);

			size_t i;
			for (i = 0; i < tSize; i++)
			{
				_BeginArrayItem(i, tSize);
				Sync(strKey, (*pVecObject)[i]);
				_EndArrayItem(i, tSize);
			}

			_EndArrayGrouping();
			return *this;
		}

		template<typename T>
		core::IFormatterA& Sync(std::string& strKey, std::set<T>* pSetObject)
		{
			const size_t tSize = _BeginArrayGrouping(strKey, pSetObject->size());
			if( tSize != pSetObject->size() )
			{
				pSetObject->clear();

				size_t i;
				for(i=0; i<tSize; i++)
				{
					_BeginArrayItem(i, tSize);
					T tempValue;
					Sync(strKey, &tempValue);
					pSetObject->insert(tempValue);
					_EndArrayItem(i, tSize);
				}
			}
			else
			{
				size_t i;
				typename std::set<T>::iterator iter = pSetObject->begin();
				for(i=0, iter = pSetObject->begin(); iter != pSetObject->end(); i++,iter++)
				{
					_BeginArrayItem(i, tSize);
					T refValue = *iter;
					Sync(strKey, &refValue);
					_EndArrayItem(i, tSize);
				}
			}
			_EndArrayGrouping();
			return *this;
		}

		template<typename T, typename OP>
		core::IFormatterA& Sync(std::string& strKey, std::set<T,OP>* pSetObject)
		{
			const size_t tSize = _BeginArrayGrouping(strKey, pSetObject->size());
			if( pSetObject->empty() )
			{
				size_t i;
				for(i=0; i<tSize; i++)
				{
					_BeginArrayItem(i, tSize);
					T tempValue;
					Sync(strKey, &tempValue);
					pSetObject->insert(tempValue);
					_EndArrayItem(i, tSize);
				}
			}
			else
			{
				size_t i;
				typename std::set<T,OP>::iterator iter = pSetObject->begin();
				for(i=0, iter = pSetObject->begin(); iter != pSetObject->end(); i++,iter++)
				{
					_BeginArrayItem(i, tSize);
					T refValue = *iter;
					Sync(strKey, &refValue);
					_EndArrayItem(i, tSize);
				}
			}
			_EndArrayGrouping();
			return *this;
		}

		core::IFormatterA& Sync(std::string& strKey, IFormatterObjectA* pObject)
		{
			_BeginObjectGrouping(strKey);
			pObject->OnSync(*this);
			_EndObjectGrouping();
			return *this;
		}

		core::IFormatterA& Sync(std::string& strKey, IFormatterObjectA** pObject)
		{
			_BeginObjectGrouping(strKey);
			(*pObject)->OnSync(*this);
			_EndObjectGrouping();
			return *this;
		}
	};

	struct IFormatterW
	{
	protected:
		core::IChannel& m_Channel;

	public:
				IFormatterW(core::IChannel& channel);
		virtual ~IFormatterW();

		IFormatterW& operator+(core::sPairSuperW const& pair)
		{
			pair.pValueHolder->VarDispatch(pair.strKey, *this);
			return *this;
		}

		// Grouping Notification
		virtual size_t _BeginDictionaryGrouping(std::wstring& strKey, const size_t tSize, bool bAllowMultiKey) = 0;
		virtual void _EndDictionaryGrouping() = 0;

		virtual size_t _BeginArrayGrouping(std::wstring& strKey, const size_t tSize) = 0;
		virtual void _BeginArrayItem(size_t tIndex, size_t tCount)	{}
		virtual void _EndArrayItem(size_t tIndex, size_t tCount)	{}
		virtual void _EndArrayGrouping() = 0;

		virtual void _BeginObjectGrouping(std::wstring& strKey) = 0;
		virtual void _EndObjectGrouping() = 0;

		virtual void _BeginRootGrouping() = 0;
		virtual void _EndRootGrouping() = 0;

		// A Value Formatting
		virtual core::IFormatterW& Sync(std::wstring& strKey, std::wstring* pString) = 0;
		virtual core::IFormatterW& Sync(std::wstring& strKey, std::string* pString) = 0;
		virtual core::IFormatterW& Sync(std::wstring& strKey, bool* pValue) = 0;
		virtual core::IFormatterW& Sync(std::wstring& strKey, char* pValue) = 0;
		virtual core::IFormatterW& Sync(std::wstring& strKey, short* pValue) = 0;
		virtual core::IFormatterW& Sync(std::wstring& strKey, int32_t* pValue) = 0;
		virtual core::IFormatterW& Sync(std::wstring& strKey, int64_t* pValue) = 0;
		virtual core::IFormatterW& Sync(std::wstring& strKey, BYTE* pValue) = 0;
		virtual core::IFormatterW& Sync(std::wstring& strKey, WORD* pValue) = 0;
		virtual core::IFormatterW& Sync(std::wstring& strKey, DWORD* pValue) = 0;
		virtual core::IFormatterW& Sync(std::wstring& strKey, QWORD* pValue) = 0;
		virtual core::IFormatterW& Sync(std::wstring& strKey, float* pValue) = 0;
		virtual core::IFormatterW& Sync(std::wstring& strKey, double* pValue) = 0;

		template<typename K, typename V>
		core::IFormatterW& Sync(std::wstring& strKey, std::map<K, V>* pMapObject)
		{
			const size_t tSize = _BeginDictionaryGrouping(strKey, pMapObject->size(), false);

			if( tSize != pMapObject->size() )
			{
				pMapObject->clear();

				size_t i;
				for(i=0; i<tSize; i++)
				{
					std::wstring strTempKey = strKey;
					V tempValue;
					Sync(strTempKey, &tempValue);

					K tempKey = ValueFrom<K>(strTempKey);
					pMapObject->insert(std::make_pair(tempKey, tempValue));
				}
			}
			else
			{
				typename std::map<K, V>::iterator iter;
				for(iter = pMapObject->begin(); iter!=pMapObject->end(); iter++)
				{
					std::wstring strTempKey = StringFromW(iter->first);
					Sync(strTempKey, &iter->second);
				}
			}
			_EndDictionaryGrouping();
			return *this;
		}

		template<typename K, typename V>
		core::IFormatterW& Sync(std::wstring& strKey, std::multimap<K, V>* pMapObject)
		{
			const size_t tSize = _BeginDictionaryGrouping(strKey, pMapObject->size(), true);

			if( tSize != pMapObject->size() )
			{
				pMapObject->clear();

				size_t i;
				for(i=0; i<tSize; i++)
				{
					std::wstring strTempKey = strKey;
					V tempValue;
					Sync(strTempKey, &tempValue);

					K tempKey = ValueFrom<K>(strTempKey);
					pMapObject->insert(std::make_pair(tempKey, tempValue));
				}
			}
			else
			{
				typename std::multimap<K, V>::iterator iter;
				for(iter = pMapObject->begin(); iter!=pMapObject->end(); iter++)
				{
					std::wstring strTempKey = StringFromW(iter->first);
					Sync(strTempKey, &iter->second);
				}
			}
			_EndDictionaryGrouping();
			return *this;
		}

		template<typename T>
		core::IFormatterW& Sync(std::wstring& strKey, std::list<T>* pListObject)
		{
			const size_t tSize = _BeginArrayGrouping(strKey, pListObject->size());
			pListObject->resize(tSize);

			typename std::list<T>::iterator iter = pListObject->begin();
			size_t i;
			for(i=0; i<tSize; i++)
			{
				_BeginArrayItem(i, tSize);
				Sync(strKey, &(*iter));
				_EndArrayItem(i, tSize);
				iter++;
			}
			_EndArrayGrouping();
			return *this;
		}

		template<typename T>
		core::IFormatterW& Sync(std::wstring& strKey, std::vector<T>* pVecObject)
		{
			const size_t tSize = _BeginArrayGrouping(strKey, pVecObject->size());
			pVecObject->resize(tSize);

			size_t i;
			for(i=0; i<tSize; i++)
			{
				_BeginArrayItem(i, tSize);
				Sync(strKey, &(*pVecObject)[i]);
				_EndArrayItem(i, tSize);
			}

			_EndArrayGrouping();
			return *this;
		}

		template<typename T>
		core::IFormatterW& Sync(std::wstring& strKey, std::vector<T*>* pVecObject)
		{
			const size_t tSize = _BeginArrayGrouping(strKey, pVecObject->size());
			pVecObject->resize(tSize);

			size_t i;
			for (i = 0; i < tSize; i++)
			{
				_BeginArrayItem(i, tSize);
				Sync(strKey, (*pVecObject)[i]);
				_EndArrayItem(i, tSize);
			}

			_EndArrayGrouping();
			return *this;
		}

		template<typename T>
		core::IFormatterW& Sync(std::wstring& strKey, std::set<T>* pSetObject)
		{
			const size_t tSize = _BeginArrayGrouping(strKey, pSetObject->size());
			if( tSize != pSetObject->size() )
			{
				pSetObject->clear();

				size_t i;
				for(i=0; i<tSize; i++)
				{
					_BeginArrayItem(i, tSize);
					T tempValue;
					Sync(strKey, &tempValue);
					pSetObject->insert(tempValue);
					_EndArrayItem(i, tSize);
				}
			}
			else
			{
				size_t i;
				typename std::set<T>::iterator iter = pSetObject->begin();
				for(i=0,iter = pSetObject->begin(); iter != pSetObject->end(); i++,iter++)
				{
					_BeginArrayItem(i, tSize);
					T tempValue = *iter;
					Sync(strKey, &tempValue);
					_EndArrayItem(i, tSize);
				}
			}			
			_EndArrayGrouping();
			return *this;
		}

		template<typename T, typename OP>
		core::IFormatterW& Sync(std::wstring& strKey, std::set<T,OP>* pSetObject)
		{
			const size_t tSize = _BeginArrayGrouping(strKey, pSetObject->size());
			if( pSetObject->empty() )
			{
				size_t i;
				for(i=0; i<tSize; i++)
				{
					_BeginArrayItem(i, tSize);
					T tempValue;
					Sync(strKey, &tempValue);
					pSetObject->insert(tempValue);
					_EndArrayItem(i, tSize);
				}
			}
			else
			{
				size_t i;
				typename std::set<T,OP>::iterator iter = pSetObject->begin();
				for(i=0,iter = pSetObject->begin(); iter != pSetObject->end(); i++,iter++)
				{
					_BeginArrayItem(i, tSize);
					T refValue = *iter;
					Sync(strKey, &refValue);
					_EndArrayItem(i, tSize);
				}
			}
			_EndArrayGrouping();
			return *this;
		}

		core::IFormatterW& Sync(std::wstring& strKey, IFormatterObjectW* pObject)
		{
			_BeginObjectGrouping(strKey);
			pObject->OnSync(*this);
			_EndObjectGrouping();
			return *this;
		}

		core::IFormatterW& Sync(std::wstring& strKey, IFormatterObjectW** pObject)
		{
			_BeginObjectGrouping(strKey);
			(*pObject)->OnSync(*this);
			_EndObjectGrouping();
			return *this;
		}
	};
}

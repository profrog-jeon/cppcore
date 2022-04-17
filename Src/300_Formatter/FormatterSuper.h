#pragma once

#include "IChannel.h"
#include "Interface.h"
#include "PairSuper.h"

#ifdef UNICODE
#define CFormatterSuper CFormatterSuperW
#else
#define CFormatterSuper CFormatterSuperA
#endif

namespace core
{
	class CFormatterSuperA
	{
	protected:
		core::IChannel& m_Channel;

	public:
		CFormatterSuperA(core::IChannel& channel);
		virtual ~CFormatterSuperA();

		virtual void Synchronize(IFormatterObjectA* pObject);

		CFormatterSuperA& operator+(core::sPairSuperA& pair)
		{
			pair.pValueHolder->VarDispatch(pair.strKey, *this);
			return *this;
		}

		// Grouping Notification
		virtual size_t BeginDictionaryGrouping(std::string& strKey, const size_t tSize, bool bAllowMultiKey) = 0;
		virtual void EndDictionaryGrouping() = 0;

		virtual size_t BeginArrayGrouping(std::string& strKey, const size_t tSize) = 0;
		virtual void BeginArrayItem(size_t tIndex, size_t tCount) {}
		virtual void EndArrayItem(size_t tIndex, size_t tCount) {}
		virtual void EndArrayGrouping() = 0;

		virtual void BeginObjectGrouping(std::string& strKey) = 0;
		virtual void EndObjectGrouping() = 0;

		virtual void BeginRootGrouping() = 0;
		virtual void EndRootGrouping() = 0;

		virtual core::CFormatterSuperA& Sync(std::string& strKey, std::string* pValue) = 0;
		virtual core::CFormatterSuperA& Sync(std::string& strKey, std::wstring* pValue) = 0;
		virtual core::CFormatterSuperA& Sync(std::string& strKey, bool* pValue) = 0;
		virtual core::CFormatterSuperA& Sync(std::string& strKey, char* pValue) = 0;
		virtual core::CFormatterSuperA& Sync(std::string& strKey, short* pValue) = 0;
		virtual core::CFormatterSuperA& Sync(std::string& strKey, int32_t* pValue) = 0;
		virtual core::CFormatterSuperA& Sync(std::string& strKey, int64_t* pValue) = 0;
		virtual core::CFormatterSuperA& Sync(std::string& strKey, BYTE* pValue) = 0;
		virtual core::CFormatterSuperA& Sync(std::string& strKey, WORD* pValue) = 0;
		virtual core::CFormatterSuperA& Sync(std::string& strKey, DWORD* pValue) = 0;
		virtual core::CFormatterSuperA& Sync(std::string& strKey, QWORD* pValue) = 0;
		virtual core::CFormatterSuperA& Sync(std::string& strKey, float* pValue) = 0;
		virtual core::CFormatterSuperA& Sync(std::string& strKey, double* pValue) = 0;

		template<typename K, typename V>
		core::CFormatterSuperA& Sync(std::string& strKey, std::map<K, V>* pMapObject)
		{
			const size_t tSize = BeginDictionaryGrouping(strKey, pMapObject->size(), false);

			if (tSize != pMapObject->size())
			{
				pMapObject->clear();

				size_t i;
				for (i = 0; i < tSize; i++)
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
				for (iter = pMapObject->begin(); iter != pMapObject->end(); iter++)
				{
					std::string strTempKey = StringFromA(iter->first);
					Sync(strTempKey, &iter->second);
				}
			}
			EndDictionaryGrouping();
			return *this;
		}

		template<typename K, typename V>
		core::CFormatterSuperA& Sync(std::string& strKey, std::multimap<K, V>* pMapObject)
		{
			const size_t tSize = BeginDictionaryGrouping(strKey, pMapObject->size(), true);

			if (tSize != pMapObject->size())
			{
				pMapObject->clear();

				size_t i;
				for (i = 0; i < tSize; i++)
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
				for (iter = pMapObject->begin(); iter != pMapObject->end(); iter++)
				{
					std::string strTempKey = StringFromA(iter->first);
					Sync(strTempKey, &iter->second);
				}
			}
			EndDictionaryGrouping();
			return *this;
		}

		template<typename T>
		core::CFormatterSuperA& Sync(std::string& strKey, std::list<T>* pListObject)
		{
			const size_t tSize = BeginArrayGrouping(strKey, pListObject->size());
			pListObject->resize(tSize);

			typename std::list<T>::iterator iter = pListObject->begin();
			size_t i;
			for (i = 0; i < tSize; i++)
			{
				BeginArrayItem(i, tSize);
				Sync(strKey, &(*iter));
				EndArrayItem(i, tSize);
				iter++;
			}
			EndArrayGrouping();
			return *this;
		}

		template<typename T>
		core::CFormatterSuperA& Sync(std::string& strKey, std::vector<T>* pVecObject)
		{
			const size_t tSize = BeginArrayGrouping(strKey, pVecObject->size());
			pVecObject->resize(tSize);

			size_t i;
			for (i = 0; i < tSize; i++)
			{
				BeginArrayItem(i, tSize);
				Sync(strKey, &(*pVecObject)[i]);
				EndArrayItem(i, tSize);
			}

			EndArrayGrouping();
			return *this;
		}

		template<typename T>
		core::CFormatterSuperA& Sync(std::string& strKey, std::vector<T*>* pVecObject)
		{
			const size_t tSize = BeginArrayGrouping(strKey, pVecObject->size());
			pVecObject->resize(tSize);

			size_t i;
			for (i = 0; i < tSize; i++)
			{
				BeginArrayItem(i, tSize);
				Sync(strKey, (*pVecObject)[i]);
				EndArrayItem(i, tSize);
			}

			EndArrayGrouping();
			return *this;
		}

		template<typename T>
		core::CFormatterSuperA& Sync(std::string& strKey, std::set<T>* pSetObject)
		{
			const size_t tSize = BeginArrayGrouping(strKey, pSetObject->size());
			if (tSize != pSetObject->size())
			{
				pSetObject->clear();

				size_t i;
				for (i = 0; i < tSize; i++)
				{
					BeginArrayItem(i, tSize);
					T tempValue;
					Sync(strKey, &tempValue);
					pSetObject->insert(tempValue);
					EndArrayItem(i, tSize);
				}
			}
			else
			{
				size_t i;
				typename std::set<T>::iterator iter = pSetObject->begin();
				for (i = 0, iter = pSetObject->begin(); iter != pSetObject->end(); i++, iter++)
				{
					BeginArrayItem(i, tSize);
					T refValue = *iter;
					Sync(strKey, &refValue);
					EndArrayItem(i, tSize);
				}
			}
			EndArrayGrouping();
			return *this;
		}

		template<typename T, typename OP>
		core::CFormatterSuperA& Sync(std::string& strKey, std::set<T, OP>* pSetObject)
		{
			const size_t tSize = BeginArrayGrouping(strKey, pSetObject->size());
			if (pSetObject->empty())
			{
				size_t i;
				for (i = 0; i < tSize; i++)
				{
					BeginArrayItem(i, tSize);
					T tempValue;
					Sync(strKey, &tempValue);
					pSetObject->insert(tempValue);
					EndArrayItem(i, tSize);
				}
			}
			else
			{
				size_t i;
				typename std::set<T, OP>::iterator iter = pSetObject->begin();
				for (i = 0, iter = pSetObject->begin(); iter != pSetObject->end(); i++, iter++)
				{
					BeginArrayItem(i, tSize);
					T refValue = *iter;
					Sync(strKey, &refValue);
					EndArrayItem(i, tSize);
				}
			}
			EndArrayGrouping();
			return *this;
		}

		core::CFormatterSuperA& Sync(std::string& strKey, IFormatterObjectA* pObject)
		{
			BeginObjectGrouping(strKey);
			pObject->OnSync(*this);
			EndObjectGrouping();
			return *this;
		}

		core::CFormatterSuperA& Sync(std::string& strKey, IFormatterObjectA** pObject)
		{
			BeginObjectGrouping(strKey);
			(*pObject)->OnSync(*this);
			EndObjectGrouping();
			return *this;
		}
	};

	class CFormatterSuperW
	{
	protected:
		core::IChannel& m_Channel;

	public:
		CFormatterSuperW(core::IChannel& channel);
		virtual ~CFormatterSuperW();

		virtual void Synchronize(IFormatterObjectW* pObject);

		CFormatterSuperW& operator+(core::sPairSuperW const& pair)
		{
			pair.pValueHolder->VarDispatch(pair.strKey, *this);
			return *this;
		}

		// Grouping Notification
		virtual size_t BeginDictionaryGrouping(std::wstring& strKey, const size_t tSize, bool bAllowMultiKey) = 0;
		virtual void EndDictionaryGrouping() = 0;

		virtual size_t BeginArrayGrouping(std::wstring& strKey, const size_t tSize) = 0;
		virtual void BeginArrayItem(size_t tIndex, size_t tCount) {}
		virtual void EndArrayItem(size_t tIndex, size_t tCount) {}
		virtual void EndArrayGrouping() = 0;

		virtual void BeginObjectGrouping(std::wstring& strKey) = 0;
		virtual void EndObjectGrouping() = 0;

		virtual void BeginRootGrouping() = 0;
		virtual void EndRootGrouping() = 0;

		// A Value Formatting
		virtual core::CFormatterSuperW& Sync(std::wstring& strKey, std::wstring* pString) = 0;
		virtual core::CFormatterSuperW& Sync(std::wstring& strKey, std::string* pString) = 0;
		virtual core::CFormatterSuperW& Sync(std::wstring& strKey, bool* pValue) = 0;
		virtual core::CFormatterSuperW& Sync(std::wstring& strKey, char* pValue) = 0;
		virtual core::CFormatterSuperW& Sync(std::wstring& strKey, short* pValue) = 0;
		virtual core::CFormatterSuperW& Sync(std::wstring& strKey, int32_t* pValue) = 0;
		virtual core::CFormatterSuperW& Sync(std::wstring& strKey, int64_t* pValue) = 0;
		virtual core::CFormatterSuperW& Sync(std::wstring& strKey, BYTE* pValue) = 0;
		virtual core::CFormatterSuperW& Sync(std::wstring& strKey, WORD* pValue) = 0;
		virtual core::CFormatterSuperW& Sync(std::wstring& strKey, DWORD* pValue) = 0;
		virtual core::CFormatterSuperW& Sync(std::wstring& strKey, QWORD* pValue) = 0;
		virtual core::CFormatterSuperW& Sync(std::wstring& strKey, float* pValue) = 0;
		virtual core::CFormatterSuperW& Sync(std::wstring& strKey, double* pValue) = 0;

		template<typename K, typename V>
		core::CFormatterSuperW& Sync(std::wstring& strKey, std::map<K, V>* pMapObject)
		{
			const size_t tSize = BeginDictionaryGrouping(strKey, pMapObject->size(), false);

			if (tSize != pMapObject->size())
			{
				pMapObject->clear();

				size_t i;
				for (i = 0; i < tSize; i++)
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
				for (iter = pMapObject->begin(); iter != pMapObject->end(); iter++)
				{
					std::wstring strTempKey = StringFromW(iter->first);
					Sync(strTempKey, &iter->second);
				}
			}
			EndDictionaryGrouping();
			return *this;
		}

		template<typename K, typename V>
		core::CFormatterSuperW& Sync(std::wstring& strKey, std::multimap<K, V>* pMapObject)
		{
			const size_t tSize = BeginDictionaryGrouping(strKey, pMapObject->size(), true);

			if (tSize != pMapObject->size())
			{
				pMapObject->clear();

				size_t i;
				for (i = 0; i < tSize; i++)
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
				for (iter = pMapObject->begin(); iter != pMapObject->end(); iter++)
				{
					std::wstring strTempKey = StringFromW(iter->first);
					Sync(strTempKey, &iter->second);
				}
			}
			EndDictionaryGrouping();
			return *this;
		}

		template<typename T>
		core::CFormatterSuperW& Sync(std::wstring& strKey, std::list<T>* pListObject)
		{
			const size_t tSize = BeginArrayGrouping(strKey, pListObject->size());
			pListObject->resize(tSize);

			typename std::list<T>::iterator iter = pListObject->begin();
			size_t i;
			for (i = 0; i < tSize; i++)
			{
				BeginArrayItem(i, tSize);
				Sync(strKey, &(*iter));
				EndArrayItem(i, tSize);
				iter++;
			}
			EndArrayGrouping();
			return *this;
		}

		template<typename T>
		core::CFormatterSuperW& Sync(std::wstring& strKey, std::vector<T>* pVecObject)
		{
			const size_t tSize = BeginArrayGrouping(strKey, pVecObject->size());
			pVecObject->resize(tSize);

			size_t i;
			for (i = 0; i < tSize; i++)
			{
				BeginArrayItem(i, tSize);
				Sync(strKey, &(*pVecObject)[i]);
				EndArrayItem(i, tSize);
			}

			EndArrayGrouping();
			return *this;
		}

		template<typename T>
		core::CFormatterSuperW& Sync(std::wstring& strKey, std::vector<T*>* pVecObject)
		{
			const size_t tSize = BeginArrayGrouping(strKey, pVecObject->size());
			pVecObject->resize(tSize);

			size_t i;
			for (i = 0; i < tSize; i++)
			{
				BeginArrayItem(i, tSize);
				Sync(strKey, (*pVecObject)[i]);
				EndArrayItem(i, tSize);
			}

			EndArrayGrouping();
			return *this;
		}

		template<typename T>
		core::CFormatterSuperW& Sync(std::wstring& strKey, std::set<T>* pSetObject)
		{
			const size_t tSize = BeginArrayGrouping(strKey, pSetObject->size());
			if (tSize != pSetObject->size())
			{
				pSetObject->clear();

				size_t i;
				for (i = 0; i < tSize; i++)
				{
					BeginArrayItem(i, tSize);
					T tempValue;
					Sync(strKey, &tempValue);
					pSetObject->insert(tempValue);
					EndArrayItem(i, tSize);
				}
			}
			else
			{
				size_t i;
				typename std::set<T>::iterator iter = pSetObject->begin();
				for (i = 0, iter = pSetObject->begin(); iter != pSetObject->end(); i++, iter++)
				{
					BeginArrayItem(i, tSize);
					T tempValue = *iter;
					Sync(strKey, &tempValue);
					EndArrayItem(i, tSize);
				}
			}
			EndArrayGrouping();
			return *this;
		}

		template<typename T, typename OP>
		core::CFormatterSuperW& Sync(std::wstring& strKey, std::set<T, OP>* pSetObject)
		{
			const size_t tSize = BeginArrayGrouping(strKey, pSetObject->size());
			if (pSetObject->empty())
			{
				size_t i;
				for (i = 0; i < tSize; i++)
				{
					BeginArrayItem(i, tSize);
					T tempValue;
					Sync(strKey, &tempValue);
					pSetObject->insert(tempValue);
					EndArrayItem(i, tSize);
				}
			}
			else
			{
				size_t i;
				typename std::set<T, OP>::iterator iter = pSetObject->begin();
				for (i = 0, iter = pSetObject->begin(); iter != pSetObject->end(); i++, iter++)
				{
					BeginArrayItem(i, tSize);
					T refValue = *iter;
					Sync(strKey, &refValue);
					EndArrayItem(i, tSize);
				}
			}
			EndArrayGrouping();
			return *this;
		}

		core::CFormatterSuperW& Sync(std::wstring& strKey, IFormatterObjectW* pObject)
		{
			BeginObjectGrouping(strKey);
			pObject->OnSync(*this);
			EndObjectGrouping();
			return *this;
		}

		core::CFormatterSuperW& Sync(std::wstring& strKey, IFormatterObjectW** pObject)
		{
			BeginObjectGrouping(strKey);
			(*pObject)->OnSync(*this);
			EndObjectGrouping();
			return *this;
		}
	};
}

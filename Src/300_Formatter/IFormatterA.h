#pragma once

#include "Interface.h"
#include "InterfaceLegacy.h"

namespace core
{
	struct IFormatterA
	{
		template<typename T>
		void Sync(std::string strKey, T& value)
		{
			OnSync(strKey, &value);
		}

		// Just for legacy
		template<typename T>
		IFormatterA& operator+(ST_PAIR_A<T>* pPair)
		{
			OnSync(pPair->strKey, &pPair->Value);
			delete pPair;
			return *this;
		}

	protected:
		virtual size_t OnBeginDictionary(std::string& strKey, const size_t tSize, bool bAllowMultiKey) = 0;
		virtual void OnEndDictionary() = 0;

		virtual size_t OnBeginArray(std::string& strKey, const size_t tSize) = 0;
		virtual void OnBeginArrayItem(size_t tIndex, size_t tCount) {}
		virtual void OnEndArrayItem(size_t tIndex, size_t tCount) {}
		virtual void OnEndArray() = 0;

		virtual void OnBeginObject(std::string& strKey) = 0;
		virtual void OnEndObject() = 0;

		virtual core::IFormatterA& OnSync(std::string& strKey, std::string* pValue) = 0;
		virtual core::IFormatterA& OnSync(std::string& strKey, std::wstring* pValue) = 0;
		virtual core::IFormatterA& OnSync(std::string& strKey, bool* pValue) = 0;
		virtual core::IFormatterA& OnSync(std::string& strKey, char* pValue) = 0;
		virtual core::IFormatterA& OnSync(std::string& strKey, short* pValue) = 0;
		virtual core::IFormatterA& OnSync(std::string& strKey, int32_t* pValue) = 0;
		virtual core::IFormatterA& OnSync(std::string& strKey, int64_t* pValue) = 0;
		virtual core::IFormatterA& OnSync(std::string& strKey, BYTE* pValue) = 0;
		virtual core::IFormatterA& OnSync(std::string& strKey, WORD* pValue) = 0;
		virtual core::IFormatterA& OnSync(std::string& strKey, DWORD* pValue) = 0;
		virtual core::IFormatterA& OnSync(std::string& strKey, QWORD* pValue) = 0;
		virtual core::IFormatterA& OnSync(std::string& strKey, float* pValue) = 0;
		virtual core::IFormatterA& OnSync(std::string& strKey, double* pValue) = 0;
		virtual core::IFormatterA& OnSync(std::string& strKey, std::vector<BYTE>* pvecData) = 0;

		virtual core::IFormatterA& OnSync(std::string& strKey, IFormatterObjectA* pObject) = 0;
		virtual core::IFormatterA& OnSync(std::string& strKey, IFormatterObjectA** pObject) = 0;

		template<typename K, typename V>
		core::IFormatterA& OnSync(std::string& strKey, std::map<K, V>* pMapObject)
		{
			const size_t tSize = OnBeginDictionary(strKey, pMapObject->size(), false);

			if (tSize != pMapObject->size())
			{
				pMapObject->clear();

				size_t i;
				for (i = 0; i < tSize; i++)
				{
					std::string strTempKey = strKey;
					V tempValue;
					OnSync(strTempKey, &tempValue);

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
					OnSync(strTempKey, &iter->second);
				}
			}
			OnEndDictionary();
			return *this;
		}

		template<typename K, typename V>
		core::IFormatterA& OnSync(std::string& strKey, std::multimap<K, V>* pMapObject)
		{
			const size_t tSize = OnBeginDictionary(strKey, pMapObject->size(), true);

			if (tSize != pMapObject->size())
			{
				pMapObject->clear();

				size_t i;
				for (i = 0; i < tSize; i++)
				{
					std::string strTempKey = strKey;
					V tempValue;
					OnSync(strTempKey, &tempValue);

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
					OnSync(strTempKey, &iter->second);
				}
			}
			OnEndDictionary();
			return *this;
		}

		template<typename T>
		core::IFormatterA& OnSync(std::string& strKey, std::list<T>* pListObject)
		{
			const size_t tSize = OnBeginArray(strKey, pListObject->size());
			pListObject->resize(tSize);

			typename std::list<T>::iterator iter = pListObject->begin();
			size_t i;
			for (i = 0; i < tSize; i++)
			{
				OnBeginArrayItem(i, tSize);
				OnSync(strKey, &(*iter));
				OnEndArrayItem(i, tSize);
				iter++;
			}
			OnEndArray();
			return *this;
		}

		template<typename T>
		core::IFormatterA& OnSync(std::string& strKey, std::vector<T>* pVecObject)
		{
			if (1 == sizeof(T))
			{
				OnSync(strKey, (std::vector<BYTE>*)pVecObject);
				return *this;
			}

			const size_t tSize = OnBeginArray(strKey, pVecObject->size());
			pVecObject->resize(tSize);

			size_t i;
			for (i = 0; i < tSize; i++)
			{
				OnBeginArrayItem(i, tSize);
				OnSync(strKey, &(*pVecObject)[i]);
				OnEndArrayItem(i, tSize);
			}

			OnEndArray();
			return *this;
		}

		template<typename T>
		core::IFormatterA& OnSync(std::string& strKey, std::vector<T*>* pVecObject)
		{
			const size_t tSize = OnBeginArray(strKey, pVecObject->size());
			pVecObject->resize(tSize);

			size_t i;
			for (i = 0; i < tSize; i++)
			{
				OnBeginArrayItem(i, tSize);
				OnSync(strKey, (*pVecObject)[i]);
				OnEndArrayItem(i, tSize);
			}

			OnEndArray();
			return *this;
		}

		template<typename T>
		core::IFormatterA& OnSync(std::string& strKey, std::set<T>* pSetObject)
		{
			const size_t tSize = OnBeginArray(strKey, pSetObject->size());
			if (tSize != pSetObject->size())
			{
				pSetObject->clear();

				size_t i;
				for (i = 0; i < tSize; i++)
				{
					OnBeginArrayItem(i, tSize);
					T tempValue;
					OnSync(strKey, &tempValue);
					pSetObject->insert(tempValue);
					OnEndArrayItem(i, tSize);
				}
			}
			else
			{
				size_t i;
				typename std::set<T>::iterator iter = pSetObject->begin();
				for (i = 0, iter = pSetObject->begin(); iter != pSetObject->end(); i++, iter++)
				{
					OnBeginArrayItem(i, tSize);
					T refValue = *iter;
					OnSync(strKey, &refValue);
					OnEndArrayItem(i, tSize);
				}
			}
			OnEndArray();
			return *this;
		}

		template<typename T, typename OP>
		core::IFormatterA& OnSync(std::string& strKey, std::set<T, OP>* pSetObject)
		{
			const size_t tSize = OnBeginArray(strKey, pSetObject->size());
			if (pSetObject->empty())
			{
				size_t i;
				for (i = 0; i < tSize; i++)
				{
					OnBeginArrayItem(i, tSize);
					T tempValue;
					OnSync(strKey, &tempValue);
					pSetObject->insert(tempValue);
					OnEndArrayItem(i, tSize);
				}
			}
			else
			{
				size_t i;
				typename std::set<T, OP>::iterator iter = pSetObject->begin();
				for (i = 0, iter = pSetObject->begin(); iter != pSetObject->end(); i++, iter++)
				{
					OnBeginArrayItem(i, tSize);
					T refValue = *iter;
					OnSync(strKey, &refValue);
					OnEndArrayItem(i, tSize);
				}
			}
			OnEndArray();
			return *this;
		}
	};
}
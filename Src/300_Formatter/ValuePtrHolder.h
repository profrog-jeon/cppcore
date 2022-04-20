#pragma once

#include <vector>
#include <list>
#include <map>

#include "Interface.h"
#include "PairSuper.h"
#include "FormatterSuper.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	struct sValuePtrHolderW : public sValuePtrHolderSuperW
	{
	private:
		T*	m_pValue;

	public:
		sValuePtrHolderW(T* pValue) : m_pValue(pValue)	{}

		void VarDispatch(std::wstring strKey, CFormatterSuperW& formatter)
		{
			formatter.Sync(strKey, m_pValue);
		}
	};

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	struct sValuePtrHolderA : public sValuePtrHolderSuperA
	{
	private:
		T*	m_pValue;

	public:
		sValuePtrHolderA(T* pValue) : m_pValue(pValue)	{}

		void VarDispatch(std::string strKey, CFormatterSuperA& formatter)
		{
			formatter.Sync(strKey, m_pValue);
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// wchar_t helper functions
	template<typename T>
	sValuePtrHolderSuperW* NewValuePtrHolderW(T* pValue)				{	return new sValuePtrHolderW<T>(pValue);					}
	template<typename T>
	sValuePtrHolderSuperW* NewValuePtrHolderW(std::list<T>* pValue)		{	return new sValuePtrHolderW<std::list<T> >(pValue);		}
	template<typename T>
	sValuePtrHolderSuperW* NewValuePtrHolderW(std::vector<T>* pValue)	{	return new sValuePtrHolderW<std::vector<T> >(pValue);	}
	template<typename K, typename V>
	sValuePtrHolderSuperW* NewValuePtrHolderW(std::map<K, V>* pValue)	{	return new sValuePtrHolderW<std::map<K, V> >(pValue);	}

	//////////////////////////////////////////////////////////////////////////
	// char helper functions
	template<typename T>
	sValuePtrHolderSuperA* NewValuePtrHolderA(T* pValue)				{	return new sValuePtrHolderA<T>(pValue);					}
	template<typename T>
	sValuePtrHolderSuperA* NewValuePtrHolderA(std::list<T>* pValue)		{	return new sValuePtrHolderA<std::list<T> >(pValue);		}
	template<typename T>
	sValuePtrHolderSuperA* NewValuePtrHolderA(std::vector<T>* pValue)	{	return new sValuePtrHolderA<std::vector<T> >(pValue);	}
	template<typename K, typename V>
	sValuePtrHolderSuperA* NewValuePtrHolderA(std::map<K, V>* pValue)	{	return new sValuePtrHolderA<std::map<K, V> >(pValue);	}
}

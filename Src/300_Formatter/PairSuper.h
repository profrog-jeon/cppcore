#pragma once

#include <string>

namespace core
{
	class CFormatterSuperA;
	class CFormatterSuperW;

	//////////////////////////////////////////////////////////////////////////
	struct sValuePtrHolderSuperA
	{
		sValuePtrHolderSuperA()		{}
		virtual		~sValuePtrHolderSuperA()	{}

		virtual void VarDispatch(std::string strKey, CFormatterSuperA& formatter) = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	struct sValuePtrHolderSuperW
	{
		sValuePtrHolderSuperW()		{}
		virtual		~sValuePtrHolderSuperW()	{}

		virtual void VarDispatch(std::wstring strKey, CFormatterSuperW& formatter) = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	struct sPairSuperA
	{
		sValuePtrHolderSuperA* pValueHolder;
		std::string strKey;

		sPairSuperA(sValuePtrHolderSuperA* pVH, std::string k) : pValueHolder(pVH), strKey(k)		{}
	};

	//////////////////////////////////////////////////////////////////////////
	struct sPairSuperW
	{
		sValuePtrHolderSuperW* pValueHolder;
		std::wstring strKey;

		sPairSuperW(sValuePtrHolderSuperW* pVH, std::wstring k) : pValueHolder(pVH), strKey(k)		{}
	};
}

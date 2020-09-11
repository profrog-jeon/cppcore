#include "stdafx.h"
#include "PacketSerializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CPacketSerializer::CPacketSerializer(core::IChannel& channel)
		: IFormatter(channel)
		, m_strErrMsg()
		, m_bValidity(false)
	{
		m_bValidity = channel.CheckValidity(m_strErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	CPacketSerializer::~CPacketSerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	inline static void SerializeString(IChannel& channel, std::tstring& refValue)
	{
		std::string refValueA = UTF8FromTCS(refValue);

		WORD wLength = (WORD)refValueA.length();
		channel.OnAccess(&wLength, sizeof(wLength));
		channel.OnAccess((void*)refValueA.c_str(), wLength);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CPacketSerializer::_BeginDictionaryGrouping(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		SerializeString(m_Channel, strKey);

		WORD wCount = (WORD)tSize;
		m_Channel.OnAccess(&wCount, 2);
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::_EndDictionaryGrouping()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CPacketSerializer::_BeginArrayGrouping(std::tstring& strKey, const size_t tSize)
	{
		SerializeString(m_Channel, strKey);

		WORD wCount = (WORD)tSize;
		m_Channel.OnAccess(&wCount, 2);
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::_EndArrayGrouping()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::_BeginObjectGrouping(std::tstring& strKey)
	{
		SerializeString(m_Channel, strKey);
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::_EndObjectGrouping()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::_BeginRootGrouping()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::_EndRootGrouping()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline static void PacketSerializerMetaFunction(IChannel& channel, std::tstring& strKey, T* pValue)
	{
		SerializeString(channel, strKey);
		channel.OnAccess(pValue, sizeof(T));
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketSerializer::Sync(std::tstring& strKey, std::tstring* pValue)
	{
		SerializeString(m_Channel, strKey);
		SerializeString(m_Channel, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT & CPacketSerializer::Sync(std::tstring& strKey, std::ntstring* pValue)
	{
		std::tstring strTempString = TCSFromNTCS(*pValue);
		Sync(strKey, &strTempString);
		*pValue = NTCSFromTCS(strTempString);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketSerializer::Sync(std::tstring& strKey, bool* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketSerializer::Sync(std::tstring& strKey, char* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketSerializer::Sync(std::tstring& strKey, short* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketSerializer::Sync(std::tstring& strKey, int32_t* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketSerializer::Sync(std::tstring& strKey, int64_t* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketSerializer::Sync(std::tstring& strKey, BYTE* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketSerializer::Sync(std::tstring& strKey, WORD* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketSerializer::Sync(std::tstring& strKey, DWORD* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketSerializer::Sync(std::tstring& strKey, QWORD* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketSerializer::Sync(std::tstring& strKey, float* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketSerializer::Sync(std::tstring& strKey, double* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}
}

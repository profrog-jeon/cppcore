 #include "stdafx.h"
#include "PacketDeserializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CPacketDeserializer::CPacketDeserializer(core::IChannel& channel)
		: IFormatter(channel)
		, m_bValidity(false)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CPacketDeserializer::~CPacketDeserializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	inline static void DeserializeString(IChannel& channel, std::tstring& refValue)
	{
		do
		{
			WORD wLength;
			if (2 != channel.Access(&wLength, 2))
				break;

			std::string strTemp;
			strTemp.resize(wLength);
			if (wLength != channel.Access((void*)strTemp.c_str(), wLength))
				break;

			refValue = TCSFromUTF8(strTemp);
		} while (0);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CPacketDeserializer::BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		DeserializeString(m_Channel, strKey);

		WORD wCount = (WORD)tSize;
		if (2 != m_Channel.Access(&wCount, 2))
			return 0;

		return wCount;
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketDeserializer::EndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CPacketDeserializer::BeginArray(std::tstring& strKey, const size_t tSize)
	{
		DeserializeString(m_Channel, strKey);

		WORD wCount = (WORD)tSize;
		if( 2 != m_Channel.Access(&wCount, 2) )
			return 0;

		return wCount;
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketDeserializer::EndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketDeserializer::BeginObject(std::tstring& strKey)
	{
		DeserializeString(m_Channel, strKey);
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketDeserializer::EndObject()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketDeserializer::BeginRoot()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketDeserializer::EndRoot()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline static void PacketDeserializerMetaFunction(IChannel& channel, std::tstring& strKey, T* pValue)
	{
		DeserializeString(channel, strKey);
		channel.Access(pValue, sizeof(T));
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketDeserializer::Sync(std::tstring& strKey, std::tstring* pValue)
	{
		DeserializeString(m_Channel, strKey);
		DeserializeString(m_Channel, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT & CPacketDeserializer::Sync(std::tstring & strKey, std::ntstring * pValue)
	{
		std::tstring strTempString = TCSFromNTCS(*pValue);
		Sync(strKey, &strTempString);
		*pValue = NTCSFromTCS(strTempString);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketDeserializer::Sync(std::tstring& strKey, bool* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketDeserializer::Sync(std::tstring& strKey, char* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketDeserializer::Sync(std::tstring& strKey, short* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketDeserializer::Sync(std::tstring& strKey, int32_t* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketDeserializer::Sync(std::tstring& strKey, int64_t* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketDeserializer::Sync(std::tstring& strKey, BYTE* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketDeserializer::Sync(std::tstring& strKey, WORD* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketDeserializer::Sync(std::tstring& strKey, DWORD* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketDeserializer::Sync(std::tstring& strKey, QWORD* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketDeserializer::Sync(std::tstring& strKey, float* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketDeserializer::Sync(std::tstring& strKey, double* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketDeserializer::Sync(std::tstring& strKey, BYTE* pData, size_t tLen)
	{
		DeserializeString(m_Channel, strKey);
		m_Channel.Access(pData, tLen);
		return *this;
	}
}

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
		DWORD dwLength;
		channel.Access(&dwLength, sizeof(dwLength));

		std::string strTemp;
		strTemp.resize(dwLength);
		channel.Access((void*)strTemp.c_str(), dwLength);

		refValue = TCSFromUTF8(strTemp);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CPacketDeserializer::BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		// Not supported for unknown-key
		assert(false);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketDeserializer::EndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CPacketDeserializer::BeginArray(std::tstring& strKey, const size_t tSize)
	{
		//DeserializeString(m_Channel, strKey);

		DWORD dwCount = (WORD)tSize;
		m_Channel.Access(&dwCount, sizeof(dwCount));
		return dwCount;
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketDeserializer::EndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketDeserializer::BeginObject(std::tstring& strKey)
	{
		//DeserializeString(m_Channel, strKey);
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
		//DeserializeString(channel, strKey);
		channel.Access(pValue, sizeof(T));
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketDeserializer::Sync(std::tstring& strKey, std::tstring* pValue)
	{
		//DeserializeString(m_Channel, strKey);
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
	core::IFormatterT& CPacketDeserializer::Sync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		//DeserializeString(m_Channel, strKey);
		DWORD dwLength;
		m_Channel.Access(&dwLength, sizeof(dwLength));
		pvecData->resize(dwLength);
		m_Channel.Access((void*)pvecData->data(), dwLength);
		return *this;
	}
}

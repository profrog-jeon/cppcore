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

		DWORD dwLength = (DWORD)refValueA.length();
		channel.Access(&dwLength, sizeof(dwLength));
		if(0 < dwLength)
			channel.Access((void*)refValueA.c_str(), dwLength);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CPacketSerializer::BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		// Not supported for unknown-key
		assert(false);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::EndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CPacketSerializer::BeginArray(std::tstring& strKey, const size_t tSize)
	{
		//SerializeString(m_Channel, strKey);

		DWORD dwCount = (DWORD)tSize;
		m_Channel.Access(&dwCount, sizeof(dwCount));
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::EndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::BeginObject(std::tstring& strKey)
	{
		//SerializeString(m_Channel, strKey);
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::EndObject()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::BeginRoot()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::EndRoot()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline static void PacketSerializerMetaFunction(IChannel& channel, std::tstring& strKey, T* pValue)
	{
		//SerializeString(channel, strKey);
		channel.Access(pValue, sizeof(T));
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketSerializer::Sync(std::tstring& strKey, std::tstring* pValue)
	{
		//SerializeString(m_Channel, strKey);
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

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CPacketSerializer::Sync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		//SerializeString(m_Channel, strKey);
		DWORD dwLength = (DWORD)pvecData->size();
		m_Channel.Access(&dwLength, sizeof(dwLength));
		m_Channel.Access((void*)pvecData->data(), dwLength);
		return *this;
	}
}

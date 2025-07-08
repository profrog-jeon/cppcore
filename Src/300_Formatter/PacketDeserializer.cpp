 #include "stdafx.h"
#include "PacketDeserializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CPacketDeserializer::CPacketDeserializer(core::IChannel& channel)
		: CFormatterSuper(channel)

	{
	}

	//////////////////////////////////////////////////////////////////////////
	CPacketDeserializer::~CPacketDeserializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CPacketDeserializer::OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg)
	{
		return true;
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
	size_t CPacketDeserializer::OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		// Not supported for unknown-key
		assert(false);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketDeserializer::OnEndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CPacketDeserializer::OnBeginArray(std::tstring& strKey, const size_t tSize)
	{
		//DeserializeString(m_Channel, strKey);

		DWORD dwCount = (WORD)tSize;
		m_Channel.Access(&dwCount, sizeof(dwCount));
		return dwCount;
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketDeserializer::OnEndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketDeserializer::OnBeginObject(std::tstring& strKey)
	{
		//DeserializeString(m_Channel, strKey);
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketDeserializer::OnEndObject()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketDeserializer::OnBeginRoot(std::tstring& strRootName)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketDeserializer::OnEndRoot()
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
	core::IFormatter& CPacketDeserializer::OnSync(std::tstring& strKey, std::tstring* pValue)
	{
		//DeserializeString(m_Channel, strKey);
		DeserializeString(m_Channel, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketDeserializer::OnSync(std::tstring & strKey, std::ntstring * pValue)
	{
		std::tstring strTempString = TCSFromNTCS(*pValue);
		OnSync(strKey, &strTempString);
		*pValue = NTCSFromTCS(strTempString);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketDeserializer::OnSync(std::tstring& strKey, bool* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketDeserializer::OnSync(std::tstring& strKey, char* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketDeserializer::OnSync(std::tstring& strKey, short* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketDeserializer::OnSync(std::tstring& strKey, int32_t* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketDeserializer::OnSync(std::tstring& strKey, int64_t* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketDeserializer::OnSync(std::tstring& strKey, BYTE* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketDeserializer::OnSync(std::tstring& strKey, WORD* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketDeserializer::OnSync(std::tstring& strKey, DWORD* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketDeserializer::OnSync(std::tstring& strKey, QWORD* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketDeserializer::OnSync(std::tstring& strKey, float* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketDeserializer::OnSync(std::tstring& strKey, double* pValue)
	{
		PacketDeserializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketDeserializer::OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		//DeserializeString(m_Channel, strKey);
		DWORD dwLength;
		m_Channel.Access(&dwLength, sizeof(dwLength));
		pvecData->resize(dwLength);
		m_Channel.Access((void*)pvecData->data(), dwLength);
		return *this;
	}
}

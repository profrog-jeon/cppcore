#include "stdafx.h"
#include "PacketSerializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CPacketSerializer::CPacketSerializer(core::IChannel& channel)
		: CFormatterSuper(channel)

	{
	}

	//////////////////////////////////////////////////////////////////////////
	CPacketSerializer::~CPacketSerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CPacketSerializer::OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg)
	{
		return m_Channel.CheckValidity(strErrMsg);
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
	size_t CPacketSerializer::OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		// Not supported for unknown-key
		assert(false);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::OnEndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CPacketSerializer::OnBeginArray(std::tstring& strKey, const size_t tSize)
	{
		//SerializeString(m_Channel, strKey);

		DWORD dwCount = (DWORD)tSize;
		m_Channel.Access(&dwCount, sizeof(dwCount));
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::OnEndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::OnBeginObject(std::tstring& strKey)
	{
		//SerializeString(m_Channel, strKey);
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::OnEndObject()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::OnBeginRoot(std::tstring& strRootName)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CPacketSerializer::OnEndRoot()
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
	core::IFormatter& CPacketSerializer::OnSync(std::tstring& strKey, std::tstring* pValue)
	{
		//SerializeString(m_Channel, strKey);
		SerializeString(m_Channel, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketSerializer::OnSync(std::tstring& strKey, std::ntstring* pValue)
	{
		std::tstring strTempString = TCSFromNTCS(*pValue);
		OnSync(strKey, &strTempString);
		*pValue = NTCSFromTCS(strTempString);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketSerializer::OnSync(std::tstring& strKey, bool* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketSerializer::OnSync(std::tstring& strKey, char* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketSerializer::OnSync(std::tstring& strKey, short* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketSerializer::OnSync(std::tstring& strKey, int32_t* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketSerializer::OnSync(std::tstring& strKey, int64_t* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketSerializer::OnSync(std::tstring& strKey, BYTE* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketSerializer::OnSync(std::tstring& strKey, WORD* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketSerializer::OnSync(std::tstring& strKey, DWORD* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketSerializer::OnSync(std::tstring& strKey, QWORD* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketSerializer::OnSync(std::tstring& strKey, float* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketSerializer::OnSync(std::tstring& strKey, double* pValue)
	{
		PacketSerializerMetaFunction(m_Channel, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CPacketSerializer::OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		//SerializeString(m_Channel, strKey);
		DWORD dwLength = (DWORD)pvecData->size();
		m_Channel.Access(&dwLength, sizeof(dwLength));
		m_Channel.Access((void*)pvecData->data(), dwLength);
		return *this;
	}
}

#include "stdafx.h"
#include "UTF8FileReader.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CUTF8FileReader::CUTF8FileReader(std::tstring strFilename)
		: IChannel()
		, m_nBomType(BOM_UNDEFINED)
		, m_strFilename(strFilename)
		, m_pFile(NULL)
		, m_strBuffer()
		, m_strContext()
	{
		FILE* pFile = NULL;
		try
		{
			pFile = fopenT(strFilename.c_str(), TEXT("rb"));
			if( pFile == NULL )
				throw exception_format(TEXT("`%s` cannot be opened!!"), strFilename.c_str());

			m_nBomType = ReadBOM(pFile);
		}
		catch(std::exception& e)
		{
			if( pFile )
				fclose(pFile);
			Log_Error("%s", e.what());
			return;
		}

		m_pFile = pFile;
	}

	//////////////////////////////////////////////////////////////////////////
	CUTF8FileReader::~CUTF8FileReader(void)
	{
		if( m_pFile )
		{
			fclose(m_pFile);
			m_pFile = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool CUTF8FileReader::CheckValidity(std::tstring& refStrErrMsg)
	{
		if( m_pFile == NULL )
		{
			refStrErrMsg = Format(TEXT("Read failure `%s`"), m_strFilename.c_str());
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CUTF8FileReader::OnAccess(void* pData, size_t tDataSize)
	{
		if( m_pFile == NULL )
			return 0;

		const size_t tBuffSize = 1024;
		char szBuff[tBuffSize+1];
		size_t tRead = fread(szBuff, 1, tBuffSize, m_pFile);

		if( (int)tRead == -1 && m_strContext.empty() )
			return 0;

		szBuff[tRead] = 0;
		m_strBuffer += szBuff;

		size_t tReadSize = 0;
		m_strContext += TCSFromUTF8(m_strBuffer, &tReadSize);
		if( tReadSize < m_strBuffer.size() )
			m_strBuffer = m_strBuffer.substr(tReadSize);
		else
			m_strBuffer = "";

		const void* pContextData = m_strContext.c_str();
		size_t tContextSize = m_strContext.size() * sizeof(TCHAR);
		if( tDataSize <= tContextSize )
		{
			memcpy(pData, pContextData, tDataSize);			
			std::tstring strTruncatedContext = m_strContext.substr(tDataSize / sizeof(TCHAR));
			m_strContext = strTruncatedContext;
			return tDataSize;
		}

		memcpy(pData, pContextData, tContextSize);
		m_strContext.clear();
		return tContextSize;
	}
}


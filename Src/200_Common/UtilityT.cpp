#include "stdafx.h"
#include "Mutex.h"
#include "Utility.h"
#include "TextFileReader.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	ST_FUNC_LOG::ST_FUNC_LOG(std::tstring strName)
		: m_strName()
	{
		m_strName = MBSFromTCS(strName);
		Log_Info("---------- %s started ----------", m_strName.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	ST_FUNC_CONSOLE_LOG::ST_FUNC_CONSOLE_LOG(std::tstring strName)
		: m_strName()
	{
		m_strName = MBSFromTCS(strName);
		printf("---------- %s started ----------\n", m_strName.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	static inline ECODE ReadFileContentsWorker(LPCTSTR pszFilePath, T& strContents, E_BOM_TYPE nEncodeType)
	{
		strContents.clear();

		CTextFileReader TextFileReader(pszFilePath, nEncodeType);
		if( TextFileReader.IsNotValid() )
			return EC_NO_FILE;

		while(1)
		{
			T strLine;
			if( EC_SUCCESS != TextFileReader.ReadLine(strLine) )
				break;
			strContents += strLine;
		}
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE ReadFileContents(std::tstring strFilePath, std::tstring& strContents, E_BOM_TYPE nEncodeType)
	{
		return ReadFileContentsWorker(strFilePath.c_str(), strContents, nEncodeType);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE ReadFileContentsA(std::tstring strFilePath, std::string& strContents, E_BOM_TYPE nEncodeType)
	{
		return ReadFileContentsWorker(strFilePath.c_str(), strContents, nEncodeType);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE ReadFileContentsW(std::tstring strFilePath, std::wstring& strContents, E_BOM_TYPE nEncodeType)
	{
		return ReadFileContentsWorker(strFilePath.c_str(), strContents, nEncodeType);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE ReadFileContents(std::tstring strFilePath, std::vector<BYTE>& outContents)
	{
		ECODE nRet = EC_SUCCESS;
		HANDLE hFile = NULL;

		try
		{
			outContents.clear();

			hFile = CreateFile(strFilePath.c_str(), GENERIC_READ_, OPEN_EXISTING_, 0);

			nRet = GetLastError();
			if( NULL == hFile )
				throw exception_format(TEXT("CreateFile(%s) failure, %d"), strFilePath.c_str(), nRet);

			DWORD dwSize = 0;
			{
				QWORD qwSize = GetFileSize(hFile);

				nRet = EC_NOT_ENOUGH_MEMORY;
				if( qwSize > 100 * 1000 * 1000 )
					throw exception_format("File size exceed 100(MB)");
				dwSize = (DWORD)qwSize;
			}

			nRet = EC_INVALID_FILE;
			if( 0 == dwSize )
				throw exception_format("File size is zero");

			// if no memory, it will throw exception "bad allocation"
			outContents.resize(dwSize);

			DWORD dwTotalReadSize = 0;
			do 
			{
				DWORD dwReadSize = 0;
				bool bRet = ReadFile(hFile, &outContents[dwTotalReadSize], dwSize - dwTotalReadSize, &dwReadSize);

				nRet = GetLastError();
				if( !bRet )
					throw exception_format("ReadFile failure, try:%u, read:%u", dwSize - dwTotalReadSize, dwReadSize);

				dwTotalReadSize += dwReadSize;
			}	while (dwTotalReadSize < dwSize);

			CloseFile(hFile);
		}
		catch (std::exception& e)
		{
			Log_Error("%s - %s", __FUNCTION__, e.what());
			if( hFile )
				CloseFile(hFile);
			return nRet;
		}
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	static inline ECODE WriteFileContentsWorker(LPCTSTR pszFilePath, const T strContents, bool bWithBOM)
	{
		ECODE nRet = EC_SUCCESS;
		HANDLE hFile = NULL;
		try
		{
			if( strContents.empty() )
				throw exception_format("Contents is EMPTY.");

			hFile = CreateFile(pszFilePath, GENERIC_WRITE_|GENERIC_READ_, CREATE_ALWAYS_, 0, 0644);

			nRet = GetLastError();
			if( NULL == hFile )
				throw exception_format(TEXT("CreateFile(%s, GENERIC_WRITE_, CREATE_ALWAYS_) failure."), pszFilePath);

			if( bWithBOM )
			{
				nRet = WriteBOM(hFile, BOM_UTF8);
				if( EC_SUCCESS != nRet )
					throw exception_format(TEXT("WriteBOM(%s, BOM_UTF8) failure."), pszFilePath);
			}

			DWORD dwWritten = 0;
			DWORD dwTotalWritten = 0;
			size_t tCharSize = sizeof(strContents.at(0));
			size_t tTotalSize = strContents.length() * tCharSize;
			while(dwTotalWritten < tTotalSize)
			{
				DWORD dwRemainedSize = tTotalSize - dwTotalWritten;
				if( !WriteFile(hFile, strContents.c_str() + dwTotalWritten, dwRemainedSize, &dwWritten) )
				{
					nRet = GetLastError();
					throw exception_format(TEXT("WriteFile(%s) failure."), pszFilePath);
				}

				dwTotalWritten += dwWritten;
			}

			CloseFile(hFile);
		}
		catch (std::exception& e)
		{
			Log_Error("%s - %s", __FUNCTION__, e.what());
			if( hFile )
				CloseFile(hFile);
			return nRet;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE WriteFileContents(std::tstring strFilePath, const std::tstring strContents, bool bWithBOM)
	{
		std::string strContentsU8 = UTF8FromTCS(strContents);
		return WriteFileContentsWorker(strFilePath.c_str(), strContentsU8, bWithBOM);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE WriteFileContentsT(std::tstring strFilePath, const std::tstring strContents, bool bWithBOM)
	{
		return WriteFileContentsWorker(strFilePath.c_str(), strContents, bWithBOM);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE WriteFileContentsA(std::tstring strFilePath, const std::string strContents, bool bWithBOM)
	{
		return WriteFileContentsWorker(strFilePath.c_str(), strContents, bWithBOM);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE WriteFileContentsW(std::tstring strFilePath, const std::wstring strContents, bool bWithBOM)
	{
		return WriteFileContentsWorker(strFilePath.c_str(), strContents, bWithBOM);
	}

	//////////////////////////////////////////////////////////////////////////
	static inline ECODE WriteFileContentsBinWorker(std::tstring strFilePath, const void* pContents, size_t tContentsSize)
	{
		ECODE nRet = EC_SUCCESS;
		HANDLE hFile = NULL;

		try
		{
			hFile = CreateFile(strFilePath.c_str(), GENERIC_WRITE_, CREATE_ALWAYS_, FILE_ATTRIBUTE_NORMAL_, 0644);

			nRet = GetLastError();
			if( NULL == hFile )
				throw exception_format(TEXT("CreateFile(%s) failure, %d"), strFilePath.c_str(), nRet);

			LPCBYTE pContentsPos = (LPCBYTE)pContents;
			DWORD dwTotalWrittenSize = 0;
			while (dwTotalWrittenSize < tContentsSize)
			{
				DWORD dwWrittenSize = 0;
				bool bRet = WriteFile(hFile, &pContentsPos[dwTotalWrittenSize], tContentsSize - dwTotalWrittenSize, &dwWrittenSize);

				nRet = GetLastError();
				if( !bRet )
					throw exception_format("WriteFile failure, try:%u, written:%u", tContentsSize - dwTotalWrittenSize, dwWrittenSize);

				dwTotalWrittenSize += dwWrittenSize;
			}

			CloseFile(hFile);
		}
		catch (std::exception& e)
		{
			Log_Error("%s - %s", __FUNCTION__, e.what());
			if( hFile )
				CloseFile(hFile);
			return nRet;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE WriteFileContents(std::tstring strFilePath, const std::vector<BYTE>& vecContents)
	{
		if( vecContents.empty() )
			return EC_NO_DATA;
		return WriteFileContentsBinWorker(strFilePath, &vecContents[0], vecContents.size());
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE WriteFileContents(std::tstring strFilePath, const void* pContents, size_t tContentsSize)
	{
		return WriteFileContentsBinWorker(strFilePath, pContents, tContentsSize);
	}

	//////////////////////////////////////////////////////////////////////////
	struct ST_ENCRYPT_CONTENTS_HEADER
	{
		BYTE btMagic[3];
		BYTE btVersion;
		BYTE btProduct;			// Each product has unique number and password
		BYTE btType;
		BYTE btMode;
		BYTE btPaddingSize;
		BYTE btReserved[8];
		ST_ENCRYPT_CONTENTS_HEADER(void) : btVersion(0), btProduct(0), btType(SYM_CIPHER_TYPE_PLAIN), btMode(SYM_CIPHER_MODE_PLAIN), btPaddingSize(0)
		{
			btMagic[0] = 'S';
			btMagic[1] = 'E';
			btMagic[2] = 'E';
			::memset(btReserved, 0,  sizeof(btReserved));
		}
	};

	//////////////////////////////////////////////////////////////////////////
	ECODE ReadFileContents(std::tstring strFilePath, std::tstring& strContents, std::string strKey, E_SYM_CIPHER_TYPE* pOutType, E_SYM_CIPHER_MODE* pOutMode)
	{
		ECODE nRet = EC_SUCCESS;
		FILE* pFile = NULL;
		try
		{
			pFile = fopenT(strFilePath.c_str(), TEXT("rb"));
			if( NULL == pFile )
				throw exception_format(TEXT("fopenT failure for ReadFileContents(%s)"), strFilePath.c_str());

			ST_ENCRYPT_CONTENTS_HEADER test, header;
			::fread(&header, sizeof(header), 1, pFile);
			if( ::memcmp(test.btMagic, header.btMagic, sizeof(test.btMagic)) )
			{
				fclose(pFile);
				return EC_INVALID_DATA;
			}

			ST_SYM_CIPHER_KEY stKey;
			nRet = GenerateCipherKey((E_SYM_CIPHER_TYPE)header.btType, (E_SYM_CIPHER_MODE)header.btMode, strKey, stKey);
			if( EC_SUCCESS != nRet )
				throw exception_format("GenerateCipherKey(%d,%d,%s) failure, %d", header.btType, header.btMode, strKey.c_str(), nRet);

			::fseek(pFile, 0, SEEK_END);
			long nSize = ::ftell(pFile) - sizeof(header);
			::fseek(pFile, sizeof(header), SEEK_SET);

			if( nSize <= 0 )
				throw exception_format(TEXT("%s Contents is empty"), strFilePath.c_str());

			std::vector<BYTE> vecEncData;
			vecEncData.resize(nSize);
			::fread((void*)&vecEncData[0], 1, vecEncData.size(), pFile);

			size_t tReqSize = DecodeData(stKey, &vecEncData[0], vecEncData.size(), NULL);

			std::string strContentsU8;
			strContentsU8.resize(tReqSize);
			DecodeData(stKey, &vecEncData[0], vecEncData.size(), (LPBYTE)strContentsU8.c_str());

			if( header.btPaddingSize < tReqSize )
				strContentsU8.resize(tReqSize - header.btPaddingSize);
			strContents = TCSFromUTF8(strContentsU8);
			fclose(pFile);

			if( pOutType )
				(*pOutType) = (E_SYM_CIPHER_TYPE)header.btType;

			if( pOutMode )
				(*pOutMode) = (E_SYM_CIPHER_MODE)header.btMode;
		}
		catch (std::exception& e)
		{
			Log_Error("%s - %s", __FUNCTION__, e.what());
			if( pFile )
				fclose(pFile);

			return nRet;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE WriteFileContents(std::tstring strFilePath, const std::tstring strContents, E_SYM_CIPHER_TYPE nType, E_SYM_CIPHER_MODE nMode, std::string strKey)
	{
		ECODE nRet = EC_SUCCESS;
		FILE* pFile = NULL;
		try
		{
			pFile = fopenT(strFilePath.c_str(), TEXT("wb"));
			if( NULL == pFile )
				throw exception_format(TEXT("fopenT failure for WriteFileContents(%s)"), strFilePath.c_str());

			ST_SYM_CIPHER_KEY stKey;
			nRet = GenerateCipherKey(nType, nMode, strKey, stKey);
			if( EC_SUCCESS != nRet )
				throw exception_format("GenerateCipherKey(%d,%d,%s) failure, %d", nType, nMode, strKey.c_str(), nRet);

			std::string strContentsU8 = UTF8FromTCS(strContents);

			std::vector<BYTE> vecEncContents;
			size_t tReqSize = EncodeData(stKey, (LPCBYTE)strContentsU8.c_str(), strContentsU8.size(), NULL);
			vecEncContents.resize(tReqSize);
			EncodeData(stKey, (LPCBYTE)strContentsU8.c_str(), strContentsU8.size(), &vecEncContents[0]);

			ST_ENCRYPT_CONTENTS_HEADER header;
			header.btType = nType;
			header.btMode = nMode;
			header.btPaddingSize = (BYTE)(vecEncContents.size() - strContentsU8.size());
			::fwrite(&header, sizeof(header), 1, pFile);
			::fwrite(&vecEncContents[0], 1, vecEncContents.size(), pFile);

			::fclose(pFile);
		}
		catch (std::exception& e)
		{
			Log_Error("%s - %s", __FUNCTION__, e.what());
			if( pFile )
				::fclose(pFile);
			return nRet;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline std::tstring& MakeAbsolutePathWorker(std::tstring strParentPath, std::tstring& strRelativePath)
	{
		Trim(strRelativePath);
		if( strRelativePath.empty() )
			return strRelativePath;

		// Windows type absolute path like C:\... D:\...
		if( std::tstring::npos != strRelativePath.find(TEXT(':')) )
			return strRelativePath;

		// Linux type absolute path like /root/...
		if( TEXT('/') == strRelativePath.at(0) )
			return strRelativePath;

		strRelativePath = strParentPath + TEXT("/") + strRelativePath;
		return MakeFormalPath(strRelativePath);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring& MakeAbsolutePath(std::tstring strParentPath, std::tstring& strRelativePath)
	{
		return MakeAbsolutePathWorker(strParentPath, strRelativePath);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring MakeAbsolutePath(LPCTSTR pszParentPath, LPCTSTR pszRelativePath)
	{
		std::tstring strRelativePath(pszRelativePath);
		MakeAbsolutePathWorker(pszParentPath, strRelativePath);
		return strRelativePath;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring& MakeAbsPathByModulePath(std::tstring& strRelativePath)
	{
		std::tstring strModulePath = ExtractDirectory(GetFileName());
		MakeAbsolutePathWorker(strModulePath, strRelativePath);
		return strRelativePath;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring MakeAbsPathByModulePath(LPCTSTR pszRelativePath)
	{
		std::tstring strModulePath = ExtractDirectory(GetFileName());
		std::tstring strRelativePath(pszRelativePath);
		MakeAbsolutePathWorker(strModulePath, strRelativePath);
		return strRelativePath;
	}
	
	//////////////////////////////////////////////////////////////////////////
	std::tstring& MakeAbsPathByCurPath(std::tstring& strRelativePath)
	{
		std::tstring strCurrentPath = GetCurrentDirectory();
		MakeAbsolutePathWorker(strCurrentPath, strRelativePath);
		return strRelativePath;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring MakeAbsPathByCurPath(LPCTSTR pszRelativePath)
	{
		std::tstring strCurrentPath = GetCurrentDirectory();
		std::tstring strRelativePath(pszRelativePath);
		MakeAbsolutePathWorker(strCurrentPath, strRelativePath);
		return strRelativePath;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline bool CheckQuotationIsExist(const std::tstring strContext, LPCTSTR pszQutation, int& nBeginPox, int& nEndPos)
	{
		size_t tContextLen = strContext.length();
		LPCTSTR pszContext = strContext.c_str();

		if( tContextLen == 0 )
			return false;

		nBeginPox = SafeFindStr(pszContext, tContextLen, pszQutation);
		nEndPos = SafeReverseFindStr(pszContext, tContextLen, pszQutation);

		if( nBeginPox < 0 )
			return false;
		if( nBeginPox == nEndPos )
			return false;

		int i;
		for(i=0; i<nBeginPox; i++)
		{
			if( !IsWhiteSpace(pszContext[i]) )
				return false;
		}

		for(i=nEndPos+1; i<(int)tContextLen && pszContext[i]; i++)
		{
			if( !IsWhiteSpace(pszContext[i]) )
				return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline bool CheckQuotationIsExist(const std::tstring strContext, int& nBeginPox, int& nEndPos)
	{
		return CheckQuotationIsExist(strContext, TEXT("\""), nBeginPox, nEndPos)
			|| CheckQuotationIsExist(strContext, TEXT("\'"), nBeginPox, nEndPos);
	}

	//////////////////////////////////////////////////////////////////////////
	static inline std::tstring StripQuotationWorker(LPCTSTR pszContext, size_t tContextLen)
	{
		std::tstring strRet(pszContext);
		strRet.resize(tContextLen);
		tContextLen = SafeStrLen(pszContext, tContextLen);
		strRet.resize(tContextLen);

		int nBeginPos, nEndPos;
		if( !CheckQuotationIsExist(strRet, nBeginPos, nEndPos) )
			return pszContext;

		std::tstring strHead = strRet.substr(0, nBeginPos);
		std::tstring strBody = strRet.substr(nBeginPos+1, nEndPos - (nBeginPos+1));
		std::tstring strTail;
		if( (int)strRet.length() >= (nEndPos+1) )
			strTail = strRet.substr(nEndPos+1);

		return strHead + strBody + strTail;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring& StripQuotation(std::tstring& strContext)
	{
		strContext = StripQuotationWorker(strContext.c_str(), strContext.length());
		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring StripQuotation(LPCTSTR pszContext, size_t tContextLen)
	{
		return StripQuotationWorker(pszContext, tContextLen);
	}

	//////////////////////////////////////////////////////////////////////////
	static inline std::tstring WrapQuotationWorker(LPCTSTR pszContext, size_t tContextLen)
	{
		std::tstring strRet(pszContext);
		tContextLen = SafeStrLen(pszContext, tContextLen);
		if( tContextLen == 0 )
			return TEXT("\"\"");

		strRet.resize(tContextLen);

		int nBeginPos, nEndPos;
		if( CheckQuotationIsExist(strRet, nBeginPos, nEndPos) )
			return strRet;

		for(nBeginPos=0; nBeginPos<(int)tContextLen; nBeginPos++)
		{
			if( !IsWhiteSpace(pszContext[nBeginPos]) )
				break;
		}

		nEndPos = nBeginPos + 1;
		for(nEndPos=(int)tContextLen-1; nEndPos>nBeginPos && nEndPos>0; nEndPos--)
		{
			if( !IsWhiteSpace(pszContext[nEndPos]) )
				break;
		}

		std::tstring strHead = strRet.substr(0, nBeginPos);
		std::tstring strBody = strRet.substr(nBeginPos, nEndPos - nBeginPos + 1);
		std::tstring strTail = strRet.substr(nEndPos+1);
		return strHead + TEXT("\"") + strBody + TEXT("\"") + strTail;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring& WrapQuotation(std::tstring& strContext)
	{
		strContext = WrapQuotationWorker(strContext.c_str(), strContext.length());
		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring WrapQuotation(LPCTSTR pszContext, size_t tContextLen)
	{
		return WrapQuotationWorker(pszContext, tContextLen);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring BuildUniqFileName(std::tstring strTempFile)
	{
		if( strTempFile.empty() )
			strTempFile = TEXT("nonamed");

		std::tstring strDirectory = ExtractDirectory(strTempFile);
		std::tstring strFilenameWithoutEXT = ExtractFileNameWithoutExt(strTempFile);
		std::tstring strFileEXT = ExtractFileExt(strTempFile);

		int nPostfix = 0;
		while(IsFileExist(strTempFile.c_str()))
		{
			if( strFileEXT.empty() )
				strTempFile = Format(TEXT("%s_%d"), strFilenameWithoutEXT.c_str(), nPostfix++);
			else
				strTempFile = Format(TEXT("%s_%d.%s"), strFilenameWithoutEXT.c_str(), nPostfix++, strFileEXT.c_str());

			if( !strDirectory.empty() )
				strTempFile = Format(TEXT("%s/%s"), strDirectory.c_str(), strTempFile.c_str());
		}

		return strTempFile;
	}
}

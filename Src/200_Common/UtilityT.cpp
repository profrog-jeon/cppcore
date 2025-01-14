#include "stdafx.h"
#include "Mutex.h"
#include "Utility.h"
#include "INIParser.h"

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
	bool IsFileExist(std::tstring strFile)
	{
		return PathFileExists(strFile);
	}

	//////////////////////////////////////////////////////////////////////////
	void TextCopyWorker(E_BOM_TYPE nEncodeType, LPCBYTE pContext, size_t tFileSize, std::string& strContents);
	void TextCopyWorker(E_BOM_TYPE nEncodeType, LPCBYTE pContext, size_t tFileSize, std::wstring& strContents);
	void TextCopyWorker(E_BOM_TYPE nEncodeType, LPCBYTE pContext, size_t tFileSize, std::tstring& strContents)
	{
		ST_BOM_INFO stBomInfo;
		E_BOM_TYPE nBOMType = ReadBOM(pContext, tFileSize, stBomInfo);

		if (BOM_UNDEFINED != nBOMType)
		{
			pContext += stBomInfo.tSize;
			tFileSize -= stBomInfo.tSize;
		}

		if (BOM_UNDEFINED != nEncodeType)
			nBOMType = nEncodeType;

		switch (nBOMType)
		{
		case BOM_UTF8:
			strContents = TCSFromUTF8((LPCSTR)pContext, tFileSize);
			break;

		case BOM_UTF16:
			strContents = TCSFromUTF16((const WORD*)pContext, tFileSize / 2);
			break;

		case BOM_UTF32:
			strContents = TCSFromUTF32((const DWORD*)pContext, tFileSize / 4);
			break;

		case BOM_UTF16_BE:
		case BOM_UTF32_BE:
			printf("UTF16_BE or UTF32_BE is not implemented yet.\n");
			break;

		default:
			if (core::IsInvalidUTF8((LPCSTR)pContext, tFileSize))
				strContents = TCSFromANSI((LPCSTR)pContext, tFileSize);
			else
				strContents = TCSFromUTF8((LPCSTR)pContext, tFileSize);
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	static inline ECODE ReadFileContents2Worker(LPCTSTR pszFilePath, T& strContents, E_BOM_TYPE nEncodeType)
	{
		ECODE nRet = EC_SUCCESS;
		HANDLE hFile = NULL;

		try
		{
			hFile = CreateFile(pszFilePath, GENERIC_READ_, OPEN_EXISTING_, 0);
			if (NULL == hFile)
			{
				nRet = GetLastError();
				throw exception_format(TEXT("CreateFile(%s, GENERIC_READ_, OPEN_EXISTING_, 0) failure, %d"), pszFilePath, nRet);
			}

			const QWORD qwSize = GetFileSize(hFile);
			if (500 * 1024 * 1024 < qwSize)
			{
				nRet = EC_NOT_ENOUGH_MEMORY;
				throw exception_format(TEXT("%s size:%lu is too big"), pszFilePath, qwSize);
			}

			std::vector<BYTE> vecContext;
			vecContext.resize(qwSize);

			DWORD dwTotalReadSize = 0;
			while (dwTotalReadSize < qwSize)
			{
				const DWORD dwRemainedSize = (DWORD)qwSize - dwTotalReadSize;
				const DWORD dwTryReadSize = std::min<DWORD>(dwRemainedSize, 1024 * 1024);

				DWORD dwReadSize = 0;
				if (!ReadFile(hFile, vecContext.data() + dwTotalReadSize, dwTryReadSize, &dwReadSize))
					throw exception_format(TEXT("ReadFile(%s) failure at pos:%u"), pszFilePath, dwTotalReadSize);

				dwTotalReadSize += dwReadSize;
			}

			CloseFile(hFile);

			TextCopyWorker(nEncodeType, vecContext.data(), vecContext.size(), strContents);
		}
		catch (const std::exception& e)
		{
			Log_Debug("%s", e.what());
			if (hFile)
				CloseFile(hFile);
			if (EC_SUCCESS != nRet)
				return nRet;
			return EC_READ_FAILURE;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	static inline ECODE ReadFileContentsWorker(LPCTSTR pszFilePath, T& strContents, E_BOM_TYPE nEncodeType)
	{
		CMemoryMappedFile MemMappedFile;
		ECODE nRet = MemMappedFile.Create(pszFilePath, PAGE_READONLY_, FILE_MAP_READ_);
		if (EC_SUCCESS != nRet)
			return nRet;

		TextCopyWorker(nEncodeType, MemMappedFile.Ptr(), MemMappedFile.Size(), strContents);
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
				const QWORD qwSize = GetFileSize(hFile);
				const QWORD qwLimit = 1000 * 1000000UL;

				nRet = EC_NOT_ENOUGH_MEMORY;
				if (qwLimit < qwSize)
					throw exception_format("File size exceed 1000(MB), size:%ull", qwSize);
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
	static inline ECODE WriteFileContentsWorker(LPCTSTR pszFilePath, const T strContents, bool bIsUTF8, bool bWithBOM)
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
				E_BOM_TYPE nBOMType = BOM_UNDEFINED;
				switch (sizeof(strContents.at(0)))
				{
				case 4:
					nBOMType = BOM_UTF32;
					break;

				case 2:
					nBOMType = BOM_UTF16;
					break;

				case 1:
				default:
					nBOMType = BOM_UNDEFINED;
					if (bIsUTF8)
						nBOMType = BOM_UTF8;
					break;
				}

				if (BOM_UNDEFINED != nBOMType)
				{
					nRet = WriteBOM(hFile, nBOMType);
					if (EC_SUCCESS != nRet)
						throw exception_format(TEXT("WriteBOM(%s, BOM_UTF8) failure."), pszFilePath);
				}
			}

			DWORD dwWritten = 0;
			DWORD dwTotalWritten = 0;
			DWORD dwCharSize = sizeof(strContents.at(0));
			DWORD dwTotalSize = (DWORD)strContents.length() * dwCharSize;
			while(dwTotalWritten < dwTotalSize)
			{
				DWORD dwRemainedSize = dwTotalSize - dwTotalWritten;
				if( !WriteFile(hFile, strContents.c_str() + dwTotalWritten, dwRemainedSize, &dwWritten) )
				{
					nRet = GetLastError();
					throw exception_format(TEXT("WriteFile(%s) failure."), pszFilePath);
				}

				dwTotalWritten += dwWritten;
			}

			FlushFileBuffers(hFile);
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
		return WriteFileContentsWorker(strFilePath.c_str(), strContentsU8, true, bWithBOM);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE WriteFileContentsT(std::tstring strFilePath, const std::tstring strContents, bool bWithBOM)
	{
		return WriteFileContentsWorker(strFilePath.c_str(), strContents, false, bWithBOM);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE WriteFileContentsA(std::tstring strFilePath, const std::string strContents, bool bWithBOM)
	{
		return WriteFileContentsWorker(strFilePath.c_str(), strContents, false, bWithBOM);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE WriteFileContentsW(std::tstring strFilePath, const std::wstring strContents, bool bWithBOM)
	{
		return WriteFileContentsWorker(strFilePath.c_str(), strContents, false, bWithBOM);
	}

	//////////////////////////////////////////////////////////////////////////
	static inline ECODE WriteFileContentsBinWorker(std::tstring strFilePath, const void* pContents, DWORD dwContentsSize)
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
			while (dwTotalWrittenSize < dwContentsSize)
			{
				DWORD dwWrittenSize = 0;
				bool bRet = WriteFile(hFile, &pContentsPos[dwTotalWrittenSize], dwContentsSize - dwTotalWrittenSize, &dwWrittenSize);

				nRet = GetLastError();
				if( !bRet )
					throw exception_format("WriteFile failure, try:%u, written:%u", dwContentsSize - dwTotalWrittenSize, dwWrittenSize);

				dwTotalWrittenSize += dwWrittenSize;
			}

			FlushFileBuffers(hFile);
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
		return WriteFileContentsBinWorker(strFilePath, &vecContents[0], (DWORD)vecContents.size());
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE WriteFileContents(std::tstring strFilePath, const void* pContents, size_t tContentsSize)
	{
		return WriteFileContentsBinWorker(strFilePath, pContents, (DWORD)tContentsSize);
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
		HANDLE hFile = NULL;
		try
		{
			hFile = CreateFile(strFilePath.c_str(), GENERIC_READ_, OPEN_EXISTING_, 0);
			if( NULL == hFile )
				throw exception_format(TEXT("ReadFileContents: CreateFile(%s, GENERIC_READ_, OPEN_EXISTING_) failure"), strFilePath.c_str());

			ST_ENCRYPT_CONTENTS_HEADER test, header;
			DWORD dwReadSize = 0;
			if (!ReadFile(hFile, &header, sizeof(header), &dwReadSize) || dwReadSize < sizeof(header))
				throw exception_format(TEXT("ReadFileContents: ReadFile(size:%u, read:%u) header read failure"), sizeof(header), dwReadSize);

			if (::memcmp(test.btMagic, header.btMagic, sizeof(test.btMagic)))
				return EC_INVALID_DATA;

			ST_SYM_CIPHER_KEY stKey;
			nRet = GenerateCipherKey((E_SYM_CIPHER_TYPE)header.btType, (E_SYM_CIPHER_MODE)header.btMode, strKey, stKey);
			if( EC_SUCCESS != nRet )
				throw exception_format("ReadFileContents: GenerateCipherKey(%d,%d,%s) failure, %d", header.btType, header.btMode, strKey.c_str(), nRet);

			QWORD qwSize = GetFileSize(hFile);
			if( qwSize <= sizeof(header))
				throw exception_format(TEXT("ReadFileContents: %s Contents is empty"), strFilePath.c_str());

			qwSize -= sizeof(header);

			std::vector<BYTE> vecEncData;
			vecEncData.resize((size_t)qwSize);

			dwReadSize = 0;
			if( !ReadFile(hFile, (void*)&vecEncData[0], (DWORD)vecEncData.size(), &dwReadSize) || dwReadSize < vecEncData.size() )
				throw exception_format(TEXT("ReadFileContents: ReadFile(size:%u, read:%u) content read failure"), vecEncData.size(), dwReadSize);

			size_t tReqSize = DecodeData(stKey, &vecEncData[0], vecEncData.size(), NULL);

			std::string strContentsU8;
			strContentsU8.resize(tReqSize);
			DecodeData(stKey, &vecEncData[0], vecEncData.size(), (LPBYTE)strContentsU8.c_str());

			if( header.btPaddingSize < tReqSize )
				strContentsU8.resize(tReqSize - header.btPaddingSize);
			strContents = TCSFromUTF8(strContentsU8);
			CloseFile(hFile);

			if( pOutType )
				(*pOutType) = (E_SYM_CIPHER_TYPE)header.btType;

			if( pOutMode )
				(*pOutMode) = (E_SYM_CIPHER_MODE)header.btMode;
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
	ECODE WriteFileContents(std::tstring strFilePath, const std::tstring strContents, E_SYM_CIPHER_TYPE nType, E_SYM_CIPHER_MODE nMode, std::string strKey)
	{
		ECODE nRet = EC_SUCCESS;
		HANDLE hFile = NULL;
		try
		{
			hFile = CreateFile(strFilePath.c_str(), GENERIC_WRITE_, CREATE_ALWAYS_, 0);
			if( NULL == hFile )
				throw exception_format(TEXT("WriteFileContents: CreateFile(%s, GENERIC_WRITE_, CREATE_ALWAYS_) failure"), strFilePath.c_str());

			ST_SYM_CIPHER_KEY stKey;
			nRet = GenerateCipherKey(nType, nMode, strKey, stKey);
			if( EC_SUCCESS != nRet )
				throw exception_format("WriteFileContents: GenerateCipherKey(%d,%d,%s) failure, %d", nType, nMode, strKey.c_str(), nRet);

			std::string strContentsU8 = UTF8FromTCS(strContents);

			std::vector<BYTE> vecEncContents;
			size_t tReqSize = EncodeData(stKey, (LPCBYTE)strContentsU8.c_str(), strContentsU8.size(), NULL);
			if( 0 == tReqSize )
				throw exception_format("WriteFileContents: EncodeData(%u) reqSize:%u failure", strContents.size(), tReqSize);

			vecEncContents.resize(tReqSize);
			EncodeData(stKey, (LPCBYTE)strContentsU8.c_str(), strContentsU8.size(), &vecEncContents[0]);

			ST_ENCRYPT_CONTENTS_HEADER header;
			header.btType = nType;
			header.btMode = nMode;
			header.btPaddingSize = (BYTE)(vecEncContents.size() - strContentsU8.size());

			DWORD dwWritten = 0;
			if (!WriteFile(hFile, &header, sizeof(header), &dwWritten) || dwWritten < sizeof(header))
				throw exception_format("WriteFileContents: WriteFile(try:%u, written:%u) has failed", sizeof(header), dwWritten);

			if (!WriteFile(hFile, &vecEncContents[0], (DWORD)vecEncContents.size(), &dwWritten) || dwWritten < vecEncContents.size())
				throw exception_format("WriteFileContents: WriteFile(try:%u, written:%u) has failed", vecEncContents.size(), dwWritten);

			FlushFileBuffers(hFile);
			CloseFile(hFile);
		}
		catch (std::exception& e)
		{
			Log_Error("%s - %s", __FUNCTION__, e.what());
			if (hFile)
				CloseFile(hFile);
			return nRet;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring MakeRelativePath(std::tstring strFullDirectory, std::tstring strDefaultDirectory)
	{
		std::tstring strDest = MakeFormalPath(strFullDirectory);
		std::tstring strPrev = MakeFormalPath(strDefaultDirectory);

		if (strDest.length() > strPrev.length())
		{
			strDest = strDest.substr(strPrev.length());
		}
		else
		{
			strDest = TEXT("");
		}

		std::tstring strTemp = TEXT("/");
		while (strDest.at(0) == strTemp.at(0))
			strDest = strDest.substr(1);

		return strDest;
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

		nBeginPox = (int)SafeFindStr(pszContext, tContextLen, pszQutation);
		nEndPos = (int)SafeReverseFindStr(pszContext, tContextLen, pszQutation);

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

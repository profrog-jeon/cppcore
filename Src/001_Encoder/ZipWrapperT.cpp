#include "stdafx.h"
#include "ZipWrapper.h"
#include "../../Inc/zip.h"
#include <stack>

namespace core
{
	ECODE CZipWrapper::Create(std::tstring strFile)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			std::string strZipFile = MBSFromTCS(strFile);

			m_pZip = zip_open(strZipFile.c_str(), ZIP_CREATE | ZIP_TRUNCATE, &nRet);
			if (NULL == m_pZip)
				throw exception_format("zip_open(%s) failure, %s", strZipFile.c_str(), GetLastErrorStr());
		}
		catch (std::exception & e)
		{
			Log_Error("%s", e.what());
			return nRet;
		}

		return EC_SUCCESS;
	}

	ECODE CZipWrapper::Open(std::tstring strFile)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			std::string strZipFile = MBSFromTCS(strFile);

			m_pZip = zip_open(strZipFile.c_str(), ZIP_CREATE, &nRet);
			if (NULL == m_pZip)
				throw exception_format("zip_open(%s) failure, err:%d, errstr:%s", strZipFile.c_str(), nRet, GetLastErrorStr());
		}
		catch (std::exception & e)
		{
			Log_Error("%s", e.what());
			return nRet;
		}

		return EC_SUCCESS;
	}

	ECODE CZipWrapper::QueryFiles(std::tstring strSrcPattern, std::vector<ST_ZIP_ENTRY>& vecEntries)
	{
		try
		{
			const size_t tCount = zip_get_num_entries(m_pZip, 0);

			size_t i;
			for (i = 0; i < tCount; i++)
			{
				LPCSTR pszSourceFileU8 = zip_get_name(m_pZip, i, 0);
				if (NULL == pszSourceFileU8)
					continue;

				std::tstring strSourceFile = TCSFromUTF8(pszSourceFileU8);
				if (!core::StrCmpWithWildcard(strSourceFile, strSrcPattern))
					continue;

				zip_stat_t stStat;
				zip_stat_index(m_pZip, i, 0, &stStat);

				ST_ZIP_ENTRY stOutEntry;
				stOutEntry.strFile = strSourceFile;
				stOutEntry.qwIndex = stStat.index;
				stOutEntry.qwSize = stStat.size;
				stOutEntry.qwCompressedSize = stStat.comp_size;
				stOutEntry.qwModificationTime = stStat.mtime;
				stOutEntry.dwCRC = stStat.crc;

				vecEntries.push_back(stOutEntry);
			}
		}
		catch (std::exception & e)
		{
			Log_Error("%s", e.what());
			return EC_INTERNAL_ERROR;
		}

		return EC_SUCCESS;
	}

	ECODE CZipWrapper::AddFile(std::tstring strZipPath, std::tstring strSrcFile, LPCSTR pszPassword)
	{
		zip_source_t* pSource = NULL;
		zip_int64_t nIndex = 0;
		try
		{
			std::string strZipPathU8 = UTF8FromTCS(strZipPath);

			std::string strExistingFileA = MBSFromTCS(strSrcFile);
			pSource = zip_source_file(m_pZip, strExistingFileA.c_str(), 0, -1);
			if (NULL == pSource)
				throw exception_format("zip_source_file(path:%s, exist:%s) failure, %s", strZipPathU8.c_str(), strExistingFileA.c_str(), GetLastErrorStr());

			nIndex = zip_file_add(m_pZip, strZipPathU8.c_str(), pSource, ZIP_FL_ENC_UTF_8 | ZIP_FL_OVERWRITE);
			if (nIndex < 0)
				throw exception_format("zip_file_add(path:%s, exist:%s) failure, %s", strZipPathU8.c_str(), strExistingFileA.c_str(), GetLastErrorStr());

			if (pszPassword)
			{
				if (0 == zip_file_set_encryption(m_pZip, nIndex, ZIP_EM_AES_256, pszPassword))
					Log_Info(TEXT("%s password applied"), strZipPath.c_str());
				else
					Log_Error("Password applying failed, %s", zip_strerror(m_pZip));
			}

			Log_Info(TEXT("%s added."), strZipPath.c_str());
		}
		catch (std::exception & e)
		{
			Log_Error("%s", e.what());
			return EC_INTERNAL_ERROR;
		}
		return EC_SUCCESS;
	}

	ECODE CZipWrapper::AddFileByMemory(std::tstring strZipPath, const void* pData, size_t tDataSize, LPCSTR pszPassword)
	{
		zip_source_t* pSource = NULL;
		zip_int64_t nIndex = 0;
		try
		{
			std::string strZipPathU8 = UTF8FromTCS(strZipPath);
			std::string strZipPathDirU8 = ExtractDirectory(strZipPathU8);

			char* pBuffer = new(std::nothrow) char[tDataSize];
			if (NULL == pBuffer)
				throw exception_format("zip_add_dir(path:%s) not enough memory size:%u", strZipPathDirU8.c_str(), tDataSize);

			memcpy(pBuffer, pData, tDataSize);
			pSource = zip_source_buffer(m_pZip, pBuffer, tDataSize, true);
			if (NULL == pSource)
				throw exception_format("zip_source_buffer(path:%s, size:%u) failure, %s", strZipPathU8.c_str(), tDataSize, GetLastErrorStr());

			nIndex = zip_file_add(m_pZip, strZipPathU8.c_str(), pSource, ZIP_FL_ENC_UTF_8 | ZIP_FL_OVERWRITE);
			if (nIndex < 0)
				throw exception_format("zip_file_add(path:%s, size:%u) failure, %s", strZipPathU8.c_str(), tDataSize, GetLastErrorStr());

			if (pszPassword)
				zip_file_set_encryption(m_pZip, nIndex, ZIP_EM_AES_256, pszPassword);

			Log_Info(TEXT("%s added."), strZipPath.c_str());
		}
		catch (std::exception & e)
		{
			Log_Error("%s", e.what());
			if (pSource)
				zip_source_free(pSource);
			return EC_INTERNAL_ERROR;
		}

		return EC_SUCCESS;
	}

	ECODE CZipWrapper::DelFile(std::tstring strSrcPattern, LPCSTR pszPassword)
	{
		zip_source_t* pSource = NULL;
		zip_int64_t nIndex = 0;
		try
		{
			std::string strZipPathDirU8 = UTF8FromTCS(strSrcPattern);

			size_t tCount = zip_get_num_entries(m_pZip, 0);

			std::stack<size_t> stkIndex;
			size_t i;
			for (i = 0; i < tCount; i++)
			{
				std::string strName = zip_get_name(m_pZip, i, 0);
				if (!core::StrCmpWithWildcard(strName, strZipPathDirU8))
					continue;

				Log_Info("%u:%s will be deleted.", i, strName.c_str());
				stkIndex.push(i);
			}

			while (!stkIndex.empty())
			{
				size_t tIndex = stkIndex.top();
				if (pszPassword)
					zip_file_set_encryption(m_pZip, tIndex, ZIP_EM_AES_256, pszPassword);
				zip_delete(m_pZip, tIndex);
				stkIndex.pop();
			}

			size_t tIndex = zip_name_locate(m_pZip, strZipPathDirU8.c_str(), 0);
			if (-1 == tIndex)
				return EC_NO_FILE;

		}
		catch (std::exception & e)
		{
			Log_Error("%s", e.what());
			if (pSource)
				zip_source_free(pSource);
			return EC_INTERNAL_ERROR;
		}

		return EC_SUCCESS;
	}

	static ECODE WriteToFile(std::tstring strTargetFile, const void* pData, size_t tDataSize)
	{
		HANDLE hFile = CreateFile(strTargetFile.c_str(), FILE_APPEND_DATA_, OPEN_ALWAYS_, 0);
		if (NULL == hFile)
			return GetLastError();

		DWORD dwWritten = 0;
		if (!WriteFile(hFile, pData, tDataSize, &dwWritten))
			return EC_WRITE_FAILURE;

		FlushFileBuffers(hFile);
		CloseFile(hFile);
		return EC_SUCCESS;
	}

	ECODE CZipWrapper::ExtractToDirectory(std::tstring strSrcPattern, std::tstring strTargetDir, LPCSTR pszPassword)
	{
		try
		{
			if (pszPassword)
				zip_set_default_password(m_pZip, pszPassword);

			const size_t tCount = zip_get_num_entries(m_pZip, 0);
			std::vector<BYTE> vecBuffer;

			size_t i;
			for (i = 0; i < tCount; i++)
			{
				LPCSTR pszSourceFileU8 = zip_get_name(m_pZip, i, 0);
				if (NULL == pszSourceFileU8)
					continue;

				std::tstring strSourceFile = TCSFromUTF8(pszSourceFileU8);
				if (!core::StrCmpWithWildcard(strSourceFile, strSrcPattern))
					continue;

				zip_file_t* pFile = zip_fopen_index(m_pZip, i, 0);
				if (NULL == pFile)
				{
					Log_Warn(TEXT("Reading failure, %s"), strSourceFile.c_str());
					continue;
				}

				zip_stat_t stStat;
				zip_stat_index(m_pZip, i, 0, &stStat);

				std::tstring strTargetFile = strTargetDir + TEXT("/") + strSourceFile;
				std::tstring strTargetDir = ExtractDirectory(strTargetFile);

				CreateDirectoryRecursively(strTargetDir);
				DeleteFile(strTargetDir.c_str());
				vecBuffer.resize(1024 * 1024);

				size_t tTotalRead = 0;
				while (1)
				{
					zip_int64_t nRead = zip_fread(pFile, &vecBuffer[0], vecBuffer.size());
					if (0 == nRead)
						break;
					WriteToFile(strTargetFile, &vecBuffer[0], nRead);

					tTotalRead += nRead;
					Log_Info(TEXT("%s extracted (%u/%u)"), strTargetFile.c_str(), tTotalRead, stStat.size);
				}

				zip_fclose(pFile);
			}
		}
		catch (std::exception & e)
		{
			Log_Error("%s", e.what());
			return EC_INTERNAL_ERROR;
		}

		return EC_SUCCESS;
	}

	ECODE ZipFromFile(std::tstring strZipFile, std::tstring strPathInZip, std::tstring strFile, LPCSTR pszPassword)
	{
		ECODE nRet = EC_SUCCESS;
		CZipWrapper zip;
		try
		{
			nRet = zip.Open(strZipFile.c_str());
			if (EC_SUCCESS != nRet)
				throw exception_format(TEXT("Zip.Open(%s) failure, %d"), strZipFile.c_str(), nRet);

			nRet = zip.AddFile(strPathInZip, strFile, pszPassword);
			if (EC_SUCCESS != nRet)
				throw exception_format(TEXT("Zip.AddFile(%s, %s) failure, %d"), strPathInZip.c_str(), strFile.c_str(), nRet);

			zip.FlushAndClose();
		}
		catch (std::exception & e)
		{
			Log_Error("%s", e.what());
			zip.DiscardAndClose();
			return nRet;
		}
		return EC_SUCCESS;
	}

	ECODE ZipFromBuffer(std::tstring strZipFile, std::tstring strPathInZip, const void* pData, size_t tDataSize, LPCSTR pszPassword)
	{
		ECODE nRet = EC_SUCCESS;
		CZipWrapper zip;
		try
		{
			nRet = zip.Open(strZipFile.c_str());
			if (EC_SUCCESS != nRet)
				throw exception_format(TEXT("Zip.Open(%s) failure, %d"), strZipFile.c_str(), nRet);

			nRet = zip.AddFileByMemory(strPathInZip, pData, tDataSize, pszPassword);
			if (EC_SUCCESS != nRet)
				throw exception_format(TEXT("Zip.AddFileByMemory(%s, size:%u) failure, %d"), strPathInZip.c_str(), tDataSize, nRet);

			zip.FlushAndClose();
		}
		catch (std::exception & e)
		{
			Log_Error("%s", e.what());
			zip.DiscardAndClose();
			return nRet;
		}
		return EC_SUCCESS;
	}

	ECODE ZipFromFiles(std::tstring strZipFile, const std::vector<std::tstring>& vecPathInZip, const std::vector<std::tstring>& vecFile, LPCSTR pszPassword)
	{
		ECODE nRet = EC_SUCCESS;
		if (vecFile.empty())
			return EC_NO_FILE;

		CZipWrapper zip;
		try
		{
			nRet = zip.Open(strZipFile.c_str());
			if (EC_SUCCESS != nRet)
				throw exception_format(TEXT("Zip.Open(%s) failure, %d"), strZipFile.c_str(), nRet);

			size_t i;
			for (i = 0; i < vecFile.size() && i < vecPathInZip.size(); i++)
			{
				nRet = zip.AddFile(vecPathInZip[i], vecFile[i], pszPassword);
				if (EC_SUCCESS != nRet)
				{
					Log_Error(TEXT("Zip.AddFile(%s) failure, %d"), vecFile[i].c_str(), nRet);
					continue;
				}

				Log_Info(TEXT("%s added."), vecFile[i].c_str());
			}


			zip.FlushAndClose();
		}
		catch (std::exception & e)
		{
			Log_Error("%s", e.what());
			zip.DiscardAndClose();
			return nRet;
		}
		return EC_SUCCESS;
	}
}

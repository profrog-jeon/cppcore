#include "stdafx.h"
#include "ExeParserSuper.h"
#include "KernelObject.h"

namespace core
{
	ECODE CExeParserSuper::Open(std::tstring strFilePath)
	{
		return Open(strFilePath.c_str());
	}

	ECODE CExeParserSuper::Open(LPCTSTR pszFilePath)
	{
		ECODE nRet = EC_SUCCESS;
		HANDLE hFile = NULL;
		try
		{
			hFile = CreateFile(pszFilePath, GENERIC_READ_, OPEN_EXISTING_, 0);
			if (NULL == hFile)
				throw exception_format(TEXT("%s CreateFile failure"), pszFilePath);

			nRet = OpenWorker(hFile);
			if (EC_SUCCESS != nRet)
				throw exception_format(TEXT("%s Open failure"), pszFilePath);

			CloseFile(hFile);
			hFile = NULL;
		}
		catch (std::exception& e)
		{
			if (hFile)
				CloseFile(hFile);
			Close();
			return nRet;
		}

		return EC_SUCCESS;
	}
}

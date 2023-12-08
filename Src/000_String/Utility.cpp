#include "stdafx.h"
#include "Utility.h"
#include "STLString.h"
#include "ASCII.h"
#include "NumericString.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	bool IsReadableChar(TCHAR cChar)
	{
		if (0xFF < cChar)
			return true;

		return g_bTextAscii[cChar] != 0;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline size_t FindDirectoryIndex(std::tstring strFilename)
	{
		if( strFilename.empty() )
			return 0;

		size_t tSlashIndex = strFilename.rfind(TEXT('/'));
		if( std::tstring::npos != tSlashIndex )
			tSlashIndex ++;
		else
			tSlashIndex = 0;

		size_t tRevSlashIndex = strFilename.rfind(TEXT('\\'));
		if( std::tstring::npos != tRevSlashIndex )
			tRevSlashIndex ++;
		else
			tRevSlashIndex = 0;

		size_t tDirIndex = MAX(tSlashIndex, tRevSlashIndex);
		if (tDirIndex)
			return tDirIndex;

		size_t tColonIndex = strFilename.rfind(TEXT(':'));
		if (std::tstring::npos != tColonIndex)
			return tColonIndex + 1;

		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring ExtractDirectory(std::tstring strFilename)
	{
		size_t tIndex = FindDirectoryIndex(strFilename);
		if( 0 == tIndex )
			return TEXT("");

		return strFilename.substr(0, tIndex-1);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring ExtractFileName(std::tstring strFilename)
	{
		size_t tIndex = FindDirectoryIndex(strFilename);
		if( 0 == tIndex )
			return strFilename;

		return strFilename.substr(tIndex);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring ExtractFileNameWithoutExt(std::tstring strFilename)
	{
		size_t tIndex = FindDirectoryIndex(strFilename);
		size_t tDotIndex = strFilename.rfind(TEXT('.'));

		if( std::tstring::npos == tDotIndex )
			return strFilename.substr(tIndex);

		if( tDotIndex < tIndex )
			return strFilename.substr(tIndex);

		return strFilename.substr(tIndex, tDotIndex - tIndex);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring ExtractFileExt(std::tstring strFilename)
	{
		size_t tIndex = FindDirectoryIndex(strFilename);
		size_t tDotIndex = strFilename.rfind(TEXT('.'));

		if( std::tstring::npos == tDotIndex )
			return TEXT("");

		if( tDotIndex < tIndex )
			return TEXT("");

		return strFilename.substr(tDotIndex+1);
	}
    
    //////////////////////////////////////////////////////////////////////////
    std::tstring& MakeFormalPath(std::tstring& strInformalPath)
    {
        Trim(strInformalPath);
        if( strInformalPath.empty() )
            return strInformalPath;
        
        Replace(strInformalPath, TEXT("\\"), TEXT("/"));
        
        std::tstring strOld;
        do
        {
            strOld = strInformalPath;
            Replace(strInformalPath, TEXT("//"), TEXT("/"));
        }    while(strOld != strInformalPath);
        
        std::deque<std::tstring> deqDir;
        
        size_t tOldPos = 0;
        size_t tPos = 0;
        while(tPos != std::tstring::npos)
        {
            tPos = strInformalPath.find(TEXT("/"), tOldPos+1);
            
            std::tstring strCurDir;
            if( tPos == std::tstring::npos )
                strCurDir = strInformalPath.substr(tOldPos);
            else
                strCurDir = strInformalPath.substr(tOldPos, tPos - tOldPos);
            
            if( TEXT("/..") == strCurDir )
            {
                if( !deqDir.empty() )
                    deqDir.pop_back();
            }
            else if( TEXT("/.") == strCurDir )
                ;    // do nothing
            else
                deqDir.push_back(strCurDir);
            
            tOldPos = tPos;
        }
        
        strInformalPath = TEXT("");
        while(!deqDir.empty())
        {
            strInformalPath += deqDir.front();
            deqDir.pop_front();
        }
        
        return strInformalPath;
    }

	//////////////////////////////////////////////////////////////////////////
	std::tstring MakeFormalPath(LPCTSTR pszInformalPath)
	{
		std::tstring strInformalPath(pszInformalPath);
		return MakeFormalPath(strInformalPath);
	}
	
	//////////////////////////////////////////////////////////////////////////
	static inline std::tstring MakeHexDumpStrWorker(LPCBYTE pData, size_t tSize)
	{
		std::tstring strRet;

		std::tstring strChar;
		std::tstring strHexArray;

		for (size_t i=0; i<tSize; i++)
		{
			const BYTE btHex = pData[i];
			if (g_bDisplayableAscii[btHex])
				strChar += core::Format(TEXT("%c"), btHex);
			else
				strChar += TEXT(".");

			TCHAR szHex[5] = {
				g_cHexCharTable[(btHex >> 4) & 0x0F],
				g_cHexCharTable[(btHex >> 0) & 0x0F],
				0
			};

			const size_t tColIndex = (i + 1) & 0xF;
			switch (tColIndex)
			{
			case 0:
				szHex[2] = TEXT('\r');
				szHex[3] = TEXT('\n');
				strHexArray += szHex;
				strRet += strChar + TEXT("\t") + strHexArray;
				strChar.clear();
				strHexArray.clear();
				break;

			case 8:
				szHex[2] = TEXT(' ');
				szHex[3] = TEXT(' ');
				strHexArray += szHex;
				break;

			default:
				szHex[2] = TEXT(' ');
				szHex[3] = 0;
				strHexArray += szHex;
				break;
			}
		}

		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring MakeHexDumpStr(LPCBYTE pData, size_t tSize)
	{
		return MakeHexDumpStrWorker(pData, tSize);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring MakeHexDumpStr(std::tstring strHexDump)
	{
		std::vector<BYTE> vecHex;
		vecHex.resize(strHexDump.size() / 2);
		for (size_t i = 0; i < vecHex.size(); i++)
			HexFromString(&vecHex[i], 1, strHexDump.substr(i * 2, 2));

		return MakeHexDumpStrWorker(vecHex.data(), vecHex.size());
	}
}

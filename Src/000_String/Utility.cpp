#include "stdafx.h"
#include "Utility.h"
#include "STLString.h"

namespace core
{
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

		size_t tColonIndex = strFilename.rfind(TEXT(':'));
		if( std::tstring::npos != tColonIndex )
			tColonIndex ++;
		else
			tColonIndex = 0;

		return MAX(tColonIndex, MAX(tSlashIndex, tRevSlashIndex));
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
}

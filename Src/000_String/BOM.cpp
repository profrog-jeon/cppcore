#include "stdafx.h"
#include "BOM.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	const ST_BOM_INFO gc_BomInfo[BOM_COUNT]	=
	{
		{	4,	{	0x00, 0x00, 0xFE, 0xFF			}	},
		{	4,	{	0xFF, 0xFE, 0x00, 0x00			}	},
		{	2,	{	0xFE, 0xFF						}	},
		{	2,	{	0xFF, 0xFE						}	},
		{	3,	{	0xEF, 0xBB, 0xBF				}	},				
		{	4,	{	0xDD, 0x73, 0x66, 0x73			}	},
		{	4,	{	0x84, 0x31, 0x95, 0x33			}	},		
		{	3,	{	0xF7, 0x64, 0x4C				}	},
		{	3,	{	0x0E, 0xFE, 0xFF				}	},
		{	3,	{	0xFB, 0xEE, 0x28				}	},
	};

	//////////////////////////////////////////////////////////////////////////
	E_BOM_TYPE ReadBOM(FILE* pFile)
	{
		BYTE btBOM[5] = { 0, };
		size_t tReadSize = fread(btBOM, 1, 5, pFile);
		if( (0 == tReadSize) || (tReadSize == -1) )
			return BOM_UNDEFINED;

		ST_BOM_INFO stBOM = { 0, };
		E_BOM_TYPE nType = ReadBOM(btBOM, tReadSize, stBOM);
		if( BOM_UNDEFINED == nType )
		{
			fseek(pFile, 0, SEEK_SET);
			return BOM_UNDEFINED;
		}

		fseek(pFile, stBOM.tSize, SEEK_SET);
		return nType;
	}

	//////////////////////////////////////////////////////////////////////////
	E_BOM_TYPE ReadBOM(const unsigned char* pszContents, size_t tLength, ST_BOM_INFO& outInfo)
	{
		BYTE btBOM[5] = { 0, };
		size_t tTestLength = tLength < 5? tLength : 5;
		::memcpy(btBOM, pszContents, tTestLength);

		size_t i, j;
		for(i=0; i<BOM_COUNT; i++)
		{
			const ST_BOM_INFO& aBOM = gc_BomInfo[i];

			if( aBOM.tSize > tTestLength )
				continue;

			bool bMatched = true;
			for(j=0; j<aBOM.tSize; j++)
			{
				if( aBOM.btSeq[j] == btBOM[j] )
					continue;

				bMatched = false;
				break;
			}

			if( !bMatched )
				continue;

			outInfo = aBOM;
			return (E_BOM_TYPE)i;
		}

		return BOM_UNDEFINED;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE WriteBOM(FILE* pFile, E_BOM_TYPE nType)
	{
		if( (nType >= BOM_COUNT) || ((int)nType < 0) )
			return EC_INVALID_BOM_TYPE;

		const ST_BOM_INFO& stBOM = gc_BomInfo[nType];
		size_t tWrittenSize = fwrite(stBOM.btSeq, 1, stBOM.tSize, pFile);
		if( tWrittenSize != stBOM.tSize )
			return EC_WRITE_FAILURE;

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	const ST_BOM_INFO* GetBOMInfo(E_BOM_TYPE nType)
	{
		if( (int)nType < 0 )
			return NULL;
		if( nType >= BOM_COUNT )
			return NULL;

		return &gc_BomInfo[nType];
	}
}

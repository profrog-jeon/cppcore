#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, SharedMemoryTest)
{
	const DWORD dwKey = 123;
	const DWORD dwSize = 1024 * 1024;

	CSharedMemory SharedMem, SharedMem2;
	EXPECT_EQ(EC_SUCCESS, SharedMem.Create(dwKey, dwSize, PAGE_READWRITE_, FILE_MAP_READ_|FILE_MAP_WRITE_) );
	EXPECT_EQ(EC_SUCCESS, SharedMem2.Create(dwKey, dwSize, PAGE_READONLY_, FILE_MAP_READ_) );	

	LPSTR pszContext = (LPSTR)SharedMem.Ptr();
	LPSTR pszContext2 = (LPSTR)SharedMem2.Ptr();
	if( pszContext && pszContext2 )
	{
		LPCSTR pszText = "Hello world?";
		SafeStrCpy(pszContext, dwSize, pszText);

		EXPECT_STREQ(pszContext2, pszText);
	}

	SharedMem.Destroy();
	SharedMem2.Destroy();
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, MemoryMappedFileTest)
{
	CMemoryMappedFile MemMappedFile;
	EXPECT_EQ(EC_SUCCESS, MemMappedFile.Create(TEXT("../../Build/Test/MemoryMappedFileTest/document1.xml"), PAGE_READONLY_, FILE_MAP_READ_) );
	EXPECT_FALSE(MemMappedFile.Size() == 0);
}

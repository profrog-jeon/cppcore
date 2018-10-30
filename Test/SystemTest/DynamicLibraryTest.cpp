#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, DynamicLibraryLoadTest)
{
#ifdef _MSC_VER
	HANDLE hModule = LoadLibrary(TEXT("./DummyDynamicLibrary.dll"));
#endif

#ifdef __APPLE__
    HANDLE hModule = LoadLibrary(TEXT("./libDummyDynamicLibrary.dylib"));
#endif
    
#ifdef __linux__
	HANDLE hModule = LoadLibrary(TEXT("./libDummyDynamicLibrary.so"));
#endif

	ASSERT_NE(hModule, (HANDLE)NULL);

	typedef int (*FP_SomeFunc)(int, int);
	FP_SomeFunc pSomeFunc = (FP_SomeFunc)GetProcAddress(hModule, "SomeFunc");
	ASSERT_NE(pSomeFunc, (FP_SomeFunc)NULL);
	EXPECT_EQ(199, pSomeFunc(100, 99));

	int* pnSomeVariable = (int*)GetProcAddress(hModule, "nSomeVariable");
	ASSERT_NE(pnSomeVariable, (int*)NULL);
	EXPECT_EQ(15, *pnSomeVariable);

	EXPECT_EQ((void*)NULL, GetProcAddress(hModule, "NonExistFunc"));

	FreeLibrary(hModule);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, ModuleNameTest)
{
	std::tstring strModuleFileName;
#ifdef _MSC_VER
    strModuleFileName = TEXT("./DummyDynamicLibrary.dll");
#endif
    
#ifdef __APPLE__
    strModuleFileName = TEXT("./libDummyDynamicLibrary.dylib");
#endif
    
#ifdef __linux__
    strModuleFileName = TEXT("./libDummyDynamicLibrary.so");
#endif

	HANDLE hModule = LoadLibrary(strModuleFileName.c_str());

	ASSERT_NE(hModule, (HANDLE)NULL);

	typedef ECODE (*FP_GetName)(char*, size_t);
	FP_GetName fpGetName = (FP_GetName)GetProcAddress(hModule, "GetName");

	const size_t tNameSize = 1024;
	char szName[tNameSize] = { 0, };
	EXPECT_EQ(EC_SUCCESS, fpGetName(szName, tNameSize));
	FreeLibrary(hModule);

	std::tstring strModuleName = ExtractFileNameWithoutExt(strModuleFileName);
	EXPECT_EQ(strModuleName, ExtractFileNameWithoutExt(TCSFromMBS(szName)));
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, RenamedModuleNameTest)
{
    std::tstring strModuleFileName, strNewModuleFileName;
#ifdef _MSC_VER
    strModuleFileName = TEXT("./DummyDynamicLibrary.dll");
    strNewModuleFileName = TEXT("TestDynamicLibrary.dll");
#endif
#ifdef __APPLE__
    strModuleFileName = TEXT("./libDummyDynamicLibrary.dylib");
    strNewModuleFileName = TEXT("libTestDynamicLibrary.dylib");
#endif
#ifdef __linux__
    strModuleFileName = TEXT("./libDummyDynamicLibrary.so");
    strNewModuleFileName = TEXT("./libTestDynamicLibrary.so");
#endif
    
    ASSERT_TRUE(CopyFile(strModuleFileName.c_str(), strNewModuleFileName.c_str()));
    
    HANDLE hModule = LoadLibrary(strNewModuleFileName.c_str());
    
    ASSERT_NE(hModule, (HANDLE)NULL);
    
    typedef ECODE (*FP_GetName)(char*, size_t);
    FP_GetName fpGetName = (FP_GetName)GetProcAddress(hModule, "GetName");
    
    const size_t tNameSize = 1024;
    char szName[tNameSize] = { 0, };
    EXPECT_EQ(EC_SUCCESS, fpGetName(szName, tNameSize));
    FreeLibrary(hModule);
    
    std::tstring strModuleName = ExtractFileNameWithoutExt(strNewModuleFileName);
    EXPECT_EQ(strModuleName, ExtractFileNameWithoutExt(TCSFromMBS(szName)));
    
    DeleteFile(strNewModuleFileName.c_str());
}

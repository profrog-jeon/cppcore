#pragma once

#include <string>

#ifdef UNICODE
#define StringFrom		StringFromW
#else
#define StringFrom		StringFromA
#endif

namespace core
{
	std::string StringFromA(char nValue				);
	std::string StringFromA(short nValue			);
	std::string StringFromA(int32_t nValue			);
	std::string StringFromA(int64_t nValue			);
	std::string StringFromA(uint32_t nValue			);
	std::string StringFromA(BYTE uValue				);
	std::string StringFromA(WORD uValue				);
	std::string StringFromA(DWORD uValue			);
	std::string StringFromA(QWORD uValue			);
	std::string StringFromA(float fValue			);
	std::string StringFromA(double dValue			);
	std::string StringFromA(bool bValue				);
	std::string StringFromA(std::string strValue	);

	std::wstring StringFromW(char nValue			);
	std::wstring StringFromW(short nValue			);
	std::wstring StringFromW(int32_t nValue			);
	std::wstring StringFromW(int64_t nValue			);
	std::wstring StringFromW(uint32_t nValue		);
	std::wstring StringFromW(BYTE uValue			);
	std::wstring StringFromW(WORD uValue			);
	std::wstring StringFromW(DWORD uValue			);
	std::wstring StringFromW(QWORD uValue			);
	std::wstring StringFromW(float fValue			);
	std::wstring StringFromW(double dValue			);
	std::wstring StringFromW(bool bValue			);
	std::wstring StringFromW(std::wstring strValue	);

	bool		BoolFrom(std::string strNum);
	char		CharFrom(std::string strNum);
	short		ShortFrom(std::string strNum);
	int			IntFrom(std::string strNum);
	int32_t		Int32From(std::string strNum);
	int64_t		Int64From(std::string strNum);
	BYTE		BYTEFrom(std::string strNum);
	WORD		WORDFrom(std::string strNum);
	DWORD		DWORDFrom(std::string strNum);
	QWORD		QWORDFrom(std::string strNum);
	float		FloatFrom(std::string strNum);
	double		DoubleFrom(std::string strNum);

	bool		BoolFrom(std::wstring strNum);
	char		CharFrom(std::wstring strNum);
	short		ShortFrom(std::wstring strNum);
	int			IntFrom(std::wstring strNum);
	int32_t		Int32From(std::wstring strNum);
	int64_t		Int64From(std::wstring strNum);
	BYTE		BYTEFrom(std::wstring strNum);
	WORD		WORDFrom(std::wstring strNum);
	DWORD		DWORDFrom(std::wstring strNum);
	QWORD		QWORDFrom(std::wstring strNum);
	float		FloatFrom(std::wstring strNum);
	double		DoubleFrom(std::wstring strNum);

	template<typename T>	T ValueFrom(std::string strNum);
	template<typename T>	T ValueFrom(std::wstring strNum);
}

#pragma once

#ifdef UNICODE
#define sPair			sPairW
#else
#define sPair			sPairA
#endif

namespace core
{
	template<typename T>
	struct ST_PAIR_W
	{
		std::wstring strKey;
		T& Value;

		ST_PAIR_W(std::wstring k, T& v)
			: strKey(k), Value(v)
		{
		}
	};

	template<typename T>
	struct ST_PAIR_A
	{
		std::string strKey;
		T& Value;

		ST_PAIR_A(std::string k, T& v)
			: strKey(k), Value(v)
		{
		}
	};

	template<typename T>
	ST_PAIR_W<T>* sPairW(std::wstring k, T& v)
	{
		return new ST_PAIR_W<T>(k, v);
	}

	template<typename T>
	ST_PAIR_A<T>* sPairA(std::string k, T& v)
	{
		return new ST_PAIR_A<T>(k, v);
	}
}
